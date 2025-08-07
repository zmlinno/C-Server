#include<iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<cstring>

int main()
{
  //第一步创建 socket
  int server_fd = socket(AF_INET,SOCK_STREAM,0);
  if(server_fd == -1)
  {
    std::cerr<<"Socket 创建失败\n";
    return -1;
  }
  std::cout<<"Socket 创建成功,fd="<<server_fd<<"\n";


  //第二步 绑定IP和端口
  sockaddr_in server_addr{};
  server_addr.sin_family = AF_INET;  //IPv4
  server_addr.sin_addr.s_addr = INADDR_ANY; //监听所有本地IP
  server_addr.sin_port = htons(8888); //设置端口
  
  //绑定bind()
  if(bind(server_fd,(sockaddr*)&server_addr,sizeof(server_addr))<0)
  {
    std::cerr<<"绑定失败\n";
    return -1;
  }
  std::cout<<"绑定成功，监听端口8888\n";


  //第三步 监听端口
  if(listen(server_fd,5)<0)
  {
    std::cerr<<"监听失败\n";
    return -1;
  }
  std::cout<<"监听中...\n";


  //第四步 接受客户端连接
  sockaddr_in client_addr{};
  socklen_t client_len = sizeof(client_addr);
  int client_fd = accept(server_fd,(sockaddr*)&client_addr,&client_len);
  if(client_fd < 0)
  {
    std::cerr<<"接受客户端连接失败\n";
    return -1;
  }
  std::cout<<"客户端连接成功\n";


  //第五步 收发数据+关闭连接
  char buffer[1024] = {0};
  read(client_fd,buffer,sizeof(buffer));
  std::cout<<"客户端说: "<<buffer<<"\n";

  const char* reply = "你好，客户端!";
  send(client_fd,reply,strlen(reply),0);

  //这是关闭的意思
  close(client_fd);
  close(server_fd);
  return 0;

}
