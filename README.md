# WebKish

-------

WebKish是一个运行在Linux系统上基于现代C++11标准的一个natvie高性能多线程并发服务器框架，代码的编写采用面向对象设计方式，每一个连接对应一个handler，并且handler自己管理着文件描述符的生命周期，断开连接会自动关闭该文件描述符，该框架既可以用作tcp服务器也可以用作http服务器。

其核心来自于Linux2.6之后新增的系统调用epoll，当然也可以实现select和poll版本的poller，继承即可。

WebKish拥有完善的异步日志系统，在Apple mini M1版的parallels Desktop虚拟机安装的Fedora操作系统中（2个CPU核心），并发写入500W条数据用了不到2s的时间。

整个项目的核心框架借鉴了开源的web服务框架：[muduo](https://github.com/chenshuo/muduo), 所以该框架是响应式的，并且CPU占用率很小，但不影响它的高并发和高性能，编写更加简洁更加完善，自己也封装了Linux的pthread库，以及实现了一个简单版本的线程池。

http服务器有一个同步的loop timer，每隔一定的时间会检查连接队列的最新心跳时间，如果设置了timeout，将会根据timeout来进行过期淘汰，keep-alive将保持永久连接，除非对端主动断开，而一次性的请求将会被直接移除，间隔检查时间暂时设置为10s，可以在服务器启动前自己更改。

http报文的解析采用了开源库[llhttp](https://github.com/nodejs/llhttp), 并对该库做了一个较为完善的封装，该部分花了我不少的精力。

WebKish的http接入很简单，只需要包含 "http_server.h"用于创建http服务器，然后包含"http_interface.h"这个头文件，用于实现请求接口：

```c++
class index_resolver : public http_interface {
  public:
  	// 在构造方法中
  	index_resolver() {
  	  resolver_list.emplace("/");		// 表示可以接受 "/" 路径的请求
  	  resolver_list.emplace("/index.html");		// 表示可以接受 "/index.html" 路径的请求
  	  resolver_list.emplace("/favicon.icon");	// 表示可以接受 "/favicon.icon" 路径的请求
  	}
  	
  	void on_request(
      const string &uri,	// 请求的具体路径
      const param_container &params,	// GET/POST请求都有可能携带参数
      http_response &response	// http上层要返回的响应
    ) override {
      	// 判断请求的路径，选择合适的方法，当然，你也可以选择更快速的方法定位，比如hash，让uri指定到一个具体的方法上面
        if (uri == "/" || uri == "/index.html") {
            /* 更新response */
        } else if (uri == "/favicon.icon") {
            /* 更新response */
        }
    }
  
}
```

然后，在执行前通过reg_http_interfc方法注册该类（传入一个shared指针）：

```c++
int main() {
  KISH_CONFIG.setKishServerName("kishkish");	// example：更改服务器名称
  
  reg_http_interfc(http_infc_ptr(new index_resolver));	// 注册resolver
  
  // 开启8个looper（即8个epoll fd）
  kish::http_server hs(5555, 8);	// 创建服务器，工作在端口5555，默认地址0.0.0.0

  hs.startup();		// 运行服务器
}
```

TODO：后续更新接入方法。。。。
