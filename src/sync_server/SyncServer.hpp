#ifndef SYNC_SERVER_HPP
#define SYNC_SERVER_HPP

#include <netinet/in.h>
#include <functional>
#include <string>

class SyncServer {
private:
    int server_fd;
    struct sockaddr_in address;
    bool running;

    using Handler = std::function<std::string(const std::string&)>;
    Handler request_handler;

public:
    SyncServer(int port = 8080);
    ~SyncServer();

    void set_handler(Handler handler);
    void start();
    void stop();

private:
    void handle_client(int client_socket);
    std::string process_request(const std::string& request);
};

#endif
