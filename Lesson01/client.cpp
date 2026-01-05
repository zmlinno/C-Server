#include<iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<cstring>
//搞个电子
//电
int main()
{

  //1创建socket
  int client_fd = socket(AF_INET,SOCK_STREAM,0);
  if(client_fd == -1)
  {
    std::cerr<<"Socket 创建失败\n";
    return -1;
  }

  //第二步配置服务端地址结构
  sockaddr_in server_addr{};
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(8888);

  //IP地址字符串 - 网络地址
  if(inet_pton(AF_INET,"127.0.0.1",&server_addr.sin_addr)<=0)
  {
    std::cerr<<"无效的IP地址\n";
    return -1;

  }

  //连接服务器
  if(connect(client_fd,(sockaddr*)&server_addr,sizeof(server_addr)) < 0)
  {
    std::cerr<<"连接服务器失败\n";
    return -1;
  }

  //发送消息  + 接收回应
  const char* msg = "您好，服务器";
  send(client_fd,msg,strlen(msg),0);

  char buffer[1024] = {0};
  read(client_fd,buffer,sizeof(buffer));
  std::cout<<"服务器回复: "<<buffer<<"\n";

  //关闭socket
  close(client_fd);
  return 0;
}
