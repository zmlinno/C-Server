#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <sys/select.h>
#include <vector>

#define MAX_CLIENTS  FD_SETSIZE

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "Socket 创建失败\n";
        return -1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8888);

    if (bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "绑定失败\n";
        return -1;
    }

    listen(server_fd, 5);
    std::cout << "select 服务器启动，监听端口 8888...\n";

    // 客户端集合
    std::vector<int> clients;

    while (true) {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(server_fd, &read_fds);  // 添加服务端 socket

        int max_fd = server_fd;

        // 添加所有客户端 socket 到监听集合
        for (int fd : clients) {
            FD_SET(fd, &read_fds);
            if (fd > max_fd) max_fd = fd;
        }

        int activity = select(max_fd + 1, &read_fds, nullptr, nullptr, nullptr);
        if (activity < 0) {
            std::cerr << "select 出错\n";
            break;
        }

        // 新客户端连接
        if (FD_ISSET(server_fd, &read_fds)) {
            sockaddr_in client_addr{};
            socklen_t client_len = sizeof(client_addr);
            int client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);
            if (client_fd >= 0) {
                clients.push_back(client_fd);
                char ip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof(ip));
                std::cout << "新客户端连接: " << ip << ":" << ntohs(client_addr.sin_port) << "\n";
            }
        }

        // 检查现有客户端是否有数据
        for (auto it = clients.begin(); it != clients.end();) {
            int client_fd = *it;
            if (FD_ISSET(client_fd, &read_fds)) {
                char buffer[1024] = {0};
                int bytes = read(client_fd, buffer, sizeof(buffer));
                if (bytes <= 0) {
                    std::cout << "客户端断开: fd=" << client_fd << "\n";
                    close(client_fd);
                    it = clients.erase(it);  // 从集合中移除
                    continue;
                } else {
                    std::cout << "收到客户端消息: " << buffer << "\n";
                    const char* reply = "你好，我是 select 服务端！";
                    send(client_fd, reply, strlen(reply), 0);
                }
            }
            ++it;
        }
    }

    close(server_fd);
    return 0;
}
