#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
电子
int main() {
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        std::cerr << "Socket 创建失败\n";
        return -1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8888);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    if (connect(client_fd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "连接服务器失败\n";
        return -1;
    }

    const char* msg = "你好，服务器！";
    send(client_fd, msg, strlen(msg), 0);

    char buffer[1024] = {0};
    read(client_fd, buffer, sizeof(buffer));
    std::cout << "服务器回复: " << buffer << "\n";

    close(client_fd);
    return 0;
}
