#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#define NUM 1024
int main(int argc,char** argv){
    if(argc!=3){
        std::cerr<<"usage: ./client server_addr server_port"<<std::endl;
        return 1;
    }
    //client端必须指明数据发往哪个ip+port
    //指定server端地址
    sockaddr_in server_addr;
    server_addr.sin_addr.s_addr=inet_addr(argv[1]);//注意，ip地址需转为网络字节序（大端，低地址存高位）
    server_addr.sin_port=htons(atoi(argv[2]));
    server_addr.sin_family=AF_INET;//IPV4协议簇

    //创建套接字 
    //AF_INET   IPv4 Internet protocols
    //SOCK_DGRAM  Supports  datagrams --udp
  
    //云服务器端口受限，指定端口做实验
    int sockfd=socket(AF_INET,SOCK_DGRAM,0);
    // struct sockaddr_in client_addr;
    // client_addr.sin_addr.s_addr=inet_addr("0.0.0.0");
    // client_addr.sin_port=htons(8080);
    // client_addr.sin_family=AF_INET;
    // bind(sockfd,(struct sockaddr*)&client_addr,sizeof(client_addr));
    //client端无需绑定到指定ip和port;端口是非常珍贵的资源
    //相比于server,client只是临时与server端建立通信，使用任何一个空闲的端口都可以,并没有必要绑定某个端口。
    //在调用
    //server端作为服务器，需长时间以确定的地址对外提供服务。
    //因此server端必须绑定，且端口号一旦确定，便开始公开并对外提供服务，不能再轻易更改
    
    while(true){
        std::string s;
        std::cout<<"请输入：";
        std::cin>>s;
        //套接字未绑定端口时，发送的时候会随机分配一个空闲端口
        sendto(sockfd,s.c_str(),s.size(),0,(struct sockaddr*)&server_addr,sizeof(server_addr));

        char buffer[NUM]={0};
        size_t len=sizeof(buffer)-1;
        struct sockaddr_in tmp;
        socklen_t tmp_len=sizeof(tmp);
        std::cout<<"准备接收数据"<<std::endl;
        recvfrom(sockfd,buffer,len,0,(struct sockaddr*)&tmp,&tmp_len);
        std::cout<<"server say:"<<buffer<<std::endl;
    }
    return 0;
}