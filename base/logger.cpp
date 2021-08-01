//
// Created by 蒋澳然 on 2021/7/29.
// email: 875691208@qq.com
// $desc
//

#include "logger.h"
#include "vector"
#include "sync.h"
#include "thread.h"
#include "atomic"
#include "file_writer.h"

class async_stream_writer;

#define FWRITER kish::singleton<async_stream_writer>::instance()

__thread char t_errno_buf[512];

// todo: 用配置文件的方式来读取log文件
const char *GLO_LOG_NAME = "WebKish.log";
kish::log_level GLO_LOG_LEVEL = kish::LL_TRACE;

namespace kish {

    void set_logconf(log_level level, const char *filename) {
        GLO_LOG_LEVEL = level;
        if (filename) {
            GLO_LOG_NAME = filename;
        }
    }

}

void log_stdout(const char *data, size_t len);

void log_stderr(const char *data, size_t len);

void log_file(const char *data, size_t len);

// fixme: 该类偶发数据越界问题 vector
class async_stream_writer : kish::noncopyable {
public:

    async_stream_writer() : async_stream_writer(std::string(GLO_LOG_NAME)) {}

    explicit async_stream_writer(std::string fln) : async_stream_writer(std::move(fln), 8, 2) {}

    async_stream_writer(std::string fln, int bufn, int interval)
            : filename(std::move(fln)),
              buf_num(bufn),
              flush_interval(interval),
              fwrite_thread(std::bind(&async_stream_writer::fwrite_routine, this), "thread-fwrite", true),
              writer(filename.c_str()),
              work_buf(new async_buffer),
              aid_buf(new async_buffer) {
        // todo: 保留两倍的数量，property？
        buffers.reserve(buf_num * 2);
        fwrite_thread.start();
    }

    ~async_stream_writer() {
        stop_write();
    }

    void stop_write() {
        stop = true;
        condit.notify_one();
        fwrite_thread.join();
    }

    size_t append(const char *data, size_t len);

private:
    typedef kish::sized_buffer<kish::KLARGE_BUFFER> async_buffer;
    typedef std::shared_ptr<async_buffer> async_buf_ptr;
    typedef std::vector<async_buf_ptr> buffer_group;

    std::string filename;
    int buf_num;
    // 刷新间隔
    int flush_interval;
    kish::thread fwrite_thread;
    kish::file_writer writer;
    std::atomic_bool stop{false};

    // 采用双缓冲区和全局缓冲数组堆积算法
    // 关键方法在append和fwrite_routine中
    buffer_group buffers{};
    async_buf_ptr work_buf;
    async_buf_ptr aid_buf;

    kish::mutex_lock locker{};
    kish::mutex_cond condit{locker};

private:
    // 最大堆积buffer数量
    static const int KMAX_ACC_BUF_NUM = 20;

private:
    void fwrite_routine();

};

size_t async_stream_writer::append(const char *data, size_t len) {
    // ⚠️!!!检查线程是否已经退出!!⚠️
    // ⚠️重要，有可能log线程退出了，仍然有其他线程调用logger⚠️
//    if (stop) return 0;
    kish::mutex_lockguard lck(locker);
    if (work_buf->avail() >= len) {
        work_buf->append(data, len);
    } else {
        // 压入一个已满数据的缓冲区
        buffers.emplace_back(work_buf);
        work_buf.reset();

        if (aid_buf) {
            work_buf = std::move(aid_buf);
            aid_buf.reset();
        } else {
            work_buf = std::make_shared<async_buffer>();
        }
        work_buf->append(data, len);
        condit.notify_one();
    }
    // fixme: check really written len bytes?
    return len;
}

void async_stream_writer::fwrite_routine() {
    buffer_group bufs_inthread;
    bufs_inthread.reserve(buf_num * 2);

    async_buf_ptr feedback_for_work(new async_buffer);
    async_buf_ptr feedback_for_aid(new async_buffer);

    while (!stop) {
        {
            kish::mutex_lockguard lck(locker);
            if (!stop && buffers.empty()) {
                condit.wait_for(flush_interval);
            }
            if (stop) break;    // stop should firstly check
            // ⚠️注意，还要压一个当前未满的缓冲区
            buffers.emplace_back(work_buf);
            work_buf.reset();

            work_buf = std::move(feedback_for_work);
            bufs_inthread.swap(buffers);
            if (!aid_buf) {
                aid_buf = std::move(feedback_for_aid);
            }

        }

        if (bufs_inthread.size() > KMAX_ACC_BUF_NUM) {
            // 保留前2个缓冲区指针，留给feedback用
            bufs_inthread.erase(bufs_inthread.begin() + 2, bufs_inthread.end());
        }

        // ⚠️高耗时代码⚠️
        for (const async_buf_ptr &ptr: bufs_inthread) {
            if (ptr->length() > 0) {
                writer.append(ptr->data(), ptr->length());
                ptr->clear();
            }
        }

        // 更新两个feedback指针
        if (!feedback_for_work) {
            assert(!bufs_inthread.empty());
            feedback_for_work = bufs_inthread.back();
            bufs_inthread.pop_back();
        }

        // 注意第二个要多加一个条件判断，因为append方法中，emplace_back一个就会唤起本线程
        if (!feedback_for_aid && !bufs_inthread.empty()) {
            feedback_for_aid = bufs_inthread.back();
            bufs_inthread.pop_back();
        }

        bufs_inthread.clear();
        writer.flush();
    }
    writer.flush();
}


const char *kish::logger::level_str[LL_LEVEL_NUM] = {"TRACE", "DEBUG", "INFO", "WARN", "FATAL", "RECOR"};

// 2021-08-02 02:04:47 tid-136642 LL_INFO[no error] in main() function --- example_main.cpp:138
kish::logger::logger(const char *filename, int line, kish::log_level level)
        : filnm(filename),
          ln(line),
          lv(level),
          saved_errno(errno) {
    bzero(t_errno_buf, sizeof t_errno_buf);
    char *errinfo = strerror_r(saved_errno, t_errno_buf, sizeof t_errno_buf);
    if (strlen(errinfo) == 0) {
        strcpy(errinfo, "no error");
    }
    // 去掉文件路径slash
    weedout_filname_slash();
    strm << ts.get_format_time(false, false)
         << " tid-" << kish::tid()
         << " " << level_str[lv] << "[" << errinfo << "]"
         << " ";
}

kish::logger::~logger() {
    // 添加文件位置
    append_logpos();
    switch (lv) {
        case LL_TRACE:
            log_stdout(strm.buffer()->data(), strm.buffer()->length());
            log_file(strm.buffer()->data(), strm.buffer()->length());
            break;
        case LL_DEBUG:
        case LL_INFO:
            log_stdout(strm.buffer()->data(), strm.buffer()->length());
            break;
        case LL_WARN:
        case LL_FATAL:
            log_stderr(strm.buffer()->data(), strm.buffer()->length());
            log_file(strm.buffer()->data(), strm.buffer()->length());
            break;
        case LL_RECOR:
            log_file(strm.buffer()->data(), strm.buffer()->length());
            break;
        case LL_LEVEL_NUM:
            break;
    }
}

void kish::logger::weedout_filname_slash() {
    if (filnm) {
        // 用strrchr剔除绝对路径的前缀，只保留文件名
        // eg: /root/desktop/info.txt -> /info.txt
        const char *fn = strrchr(filnm, '/');
        if (fn) {
            fn++;   // 去掉 '/'
            filnm = fn;
        }
    }
}

void kish::logger::append_logpos() {
    strm << " --- " << filnm << ":" << ln << "\n";
}

void log_stdout(const char *data, size_t len) {
    size_t n = fwrite(data, 1, len, stdout);
    // fixme: check n
}

void log_stderr(const char *data, size_t len) {
    size_t n = fwrite(data, 1, len, stderr);
}

void log_file(const char *data, size_t len) {
    FWRITER.append(data, len);
}