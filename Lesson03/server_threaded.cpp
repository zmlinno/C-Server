#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <pthread.h>
//电子
//222
//点
//哦哦哦
//点
//点
//34
//的
//哈哈
void* handle_client(void* arg) {
    int client_fd = *(int*)arg;
    delete (int*)arg;  // 释放内存

    char buffer[1024] = {0};
    int bytes = read(client_fd, buffer, sizeof(buffer));
    if (bytes > 0) {
        std::cout << "客户端消息: " << buffer << "\n";

        const char* reply = "你好，客户端（来自线程）";
        send(client_fd, reply, strlen(reply), 0);
    }

    close(client_fd);
    std::cout << "连接关闭。\n";
    return nullptr;
}

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
    std::cout << "多线程服务端已启动，监听端口 8888...\n";

    while (true) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            std::cerr << "接受连接失败\n";
            continue;
        }

        // 打印客户端 IP 和端口
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof(ip));
        std::cout << "客户端连接：" << ip << ":" << ntohs(client_addr.sin_port) << "\n";

        // 创建线程处理客户端
        pthread_t tid;
        int* pclient = new int(client_fd);  // 为每个线程分配独立的客户端fd副本
        pthread_create(&tid, nullptr, handle_client, pclient);
        pthread_detach(tid);  // 分离线程，自动回收资源
    }

    close(server_fd);
    return 0;
}
