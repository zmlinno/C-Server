#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
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
    std::cout << "服务器启动，监听端口 8888...\n";

    while (true) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);

        int client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            std::cerr << "接受连接失败\n";
            continue;
        }

        // 输出客户端 IP 和端口
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        std::cout << "客户端连接成功：" << client_ip << ":" << ntohs(client_addr.sin_port) << "\n";

        char buffer[1024] = {0};
        read(client_fd, buffer, sizeof(buffer));
        std::cout << "客户端消息: " << buffer << "\n";

        const char* reply = "你好，客户端！我是服务器";
        send(client_fd, reply, strlen(reply), 0);

        close(client_fd);  // 通信完关闭连接，等待下一个
    }

    close(server_fd);
    return 0;
}
