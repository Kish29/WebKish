//
// Created by 蒋澳然 on 2021/7/30.
// email: 875691208@qq.com
// $desc
//

#include "http_server.h"

#include "logger.h"

void http_server::on_acceptnew(int fd, const inet_address &peer_addr) {
    LOG_TRACE << "new connection from " << peer_addr.ip_port() << " for new connector[" << fd << "]";
    if (connectors.size() < KISH_CONFIG.MAX_SERVER_CNN()) {
        looper_ptr looper = loopers.next_loop();
        auto http_obs = std::make_shared<http_handler>(fd);
        looper->add_observe(http_obs);

        // 加入到连接池中
        {
            mutex_lockguard lck(locker);
            connectors.emplace_back(http_obs);
        }

    } else {
        LOG_WARN << "http server reached max connections!";
    }
}

void http_server::check_for_alive() {
    if (connectors.empty()) return;
    // 注意，整个web服务器框架，仅仅有两处持续持有着fdholder对象，一个是poller的save_map
    // 另一个就是http_server的connectors，所以，在更新移除时，确保没有其他地方持有fdholder对象
    {
        mutex_lockguard lck(locker);
        auto it = connectors.begin();
        while (it != connectors.end()) {
            if (it->get()->dead()) {    // 如果已经死亡，直接移除
                it = connectors.erase(it);
                continue;
            }
            if (it->get()->keep_alive()) {
                it++;
                continue;
            }
            if (it->get()->has_set_timeout()) {
                time_stamp t;
                if (t.get_time_sec() - it->get()->get_last_heart_rev_time().get_time_sec() > it->get()->get_timeout()) {
#ifdef __DEBUG__
                    LOG_INFO << "server set [" << it->get()->fd() << "] dead";
#endif
                    it->get()->set_dead();
                    it->reset();
                    it = connectors.erase(it);
                } else {
                    it++;
                }
            } else {
#ifdef __DEBUG__
                LOG_INFO << "server set [" << it->get()->fd() << "] dead";
#endif
                it->get()->set_dead();
                it->reset();
                it = connectors.erase(it);
            }
        }
    }
}

void http_server::startup() {
    base::startup();
    serv_timer.sync_loop(std::bind(&http_server::check_for_alive, this), KISH_CONFIG.HEART_CHECK_INTERVAL());
}
