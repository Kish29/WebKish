#include <iostream>
#include "memory"

class keeper {
public:

    virtual void show() const = 0;
};

class time_keeper : public keeper {
public:
    time_keeper() {
        printf("time_keeper constructor\n");
    }

    explicit time_keeper(int f) : flag(f) {}

    virtual ~time_keeper() {
        printf("time_keeper destructor\n");
    }

    void show() const override {
        printf("in time_keeper!\n");
    }

private:
    int flag{};
};

class atomic_clock : public time_keeper {
public:

    atomic_clock() {
        printf("atomic_clock constructor\n");
    }

    /*atomic_clock() : time_keeper() {
        printf("atomic_clock constructor\n");
    }*/
    explicit atomic_clock(int f) : time_keeper(f) {}

    ~atomic_clock() override {
        printf("atomic_clock destructor\n");
    }

    void show() const override {
        printf("in atomic_clock!\n");
    }
};

void ss() {
    std::shared_ptr<atomic_clock> t1(new atomic_clock(1));
//    std::shared_ptr<time_keeper> t2(dynamic_cast<time_keeper *>(t1.get()));
    std::shared_ptr<time_keeper> t2(t1);
    printf("count %ld\n", t1.use_count());
}

#include "cstring"

void other_test() {
    char buf[3]{};
    memcpy(buf, "1233412341", 3);
    printf("%s\n", buf);

    const char *filename = "/boot/dev/ipv4/sock.conf";
    const char *name = strrchr(filename, '/');
    if (name) name++;

    printf("%s\n", name);
    printf("%s\n", filename);
}


#include "map"

class ss1;

typedef std::shared_ptr<ss1> s_p;

std::map<std::string, s_p> global_map;

class ss1 : std::enable_shared_from_this<ss1> {
public:
    virtual void a(std::string tag) = 0;

    virtual std::string b() const = 0;

};

void reg(const std::shared_ptr<ss1> &target);

void reg(const std::shared_ptr<ss1> &target) {
    global_map.insert(std::make_pair(target->b(), target));
}

class ss2 : public ss1 {
public:
    std::string b() const override {
        return "ss2";
    }

    void a(std::string tag) override {
        printf("ss2 wocao %s\n", tag.c_str());
    }
};

#include "http_interface.h"
#include "http_handler.h"

using namespace kish;

#include "unordered_set"

class test_mapper : public kish::http_interface {
public:

    test_mapper() {
        resolver_list.insert("index.html");
    }

    simp_resp on_request(const string &uri, const parameters &params) override {
        printf("test_mapper received response!\n");
        for (auto &param: params) {
            printf("{%s :%s}\n", param.first.c_str(), param.second.c_str());
        }
        return simp_resp{200, "success"};
    }

};

void test() {
    if (!reg_http_interfc(std::shared_ptr<http_interface>(new test_mapper))) {
        printf("reg false");
        exit(EXIT_FAILURE);
    }
    http_handler::test_mappers();
}

class holer {
public:

    void add_tk(const shared_ptr<time_keeper> &tk1) {
        tk = tk1;
    }

    void show() {
        tk->show();
    }

private:
    shared_ptr<time_keeper> tk;
};

void arg_test(holer &h) {
    shared_ptr <time_keeper> tk(new atomic_clock);
    h.add_tk(tk);
}

void test2() {
    holer h;
    arg_test(h);
    h.show();
}

void test3() {
    shared_ptr <time_keeper> tk(new atomic_clock);
    sleep(2);
    tk = nullptr;
    tk = shared_ptr<time_keeper>(new atomic_clock);
}

volatile bool quit = false;

void *test4(void *) {
    while (!quit) {
        printf("hhh\n");
        sleep(1);
    }
    printf("loop quit\n");
    pthread_exit(nullptr);
}

#include "thread_pool.h"

std::atomic_int started_num{0};

void *tf(void *) {
    for (int i = 0; i < 1000; ++i) {
        started_num++;
    }
    pthread_exit(nullptr);
}

#include "thread_pool.h"

int main() {
//    EXECUTOR_POOL.init(4, 1024);
    /*thread_pool::instance()->init(8);
    thread_pool::instance()->submit(nullptr, [](const std::shared_ptr<void> &) -> void {
        printf("hello");
        fflush(stdout);
    });*/
/*
    pthread_t pt{};
    pthread_create(&pt, nullptr, test4, nullptr);
    sleep(5);
    quit = true;
*/
    /* pthread_t tid[100];
     for (int i = 0; i < 100; ++i) {
         pthread_create(&tid[i], nullptr, tf, nullptr);
     }
     sleep(1);
     printf("started_num %d", (int) started_num);*/
    /*std::vector<callable> vc;
    vc.emplace_back([]() -> void {
        printf("????\n");
        fflush(stdout);
    });
    vc.at(0)();

    callable task = std::move(vc.at(0));
    task();

    printf("vs size is %ld\n", vc.size());*/
    reg_http_interfc(std::make_shared<test_mapper>());
    http_handler::test_mappers();

    exit(EXIT_SUCCESS);
}
