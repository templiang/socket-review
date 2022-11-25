#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include <arpa/inet.h>
#include<string>

int main(){
    int sockfd;
    //创建套接字
    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd<0){
        std::cerr<<"创建套接字失败\n"<<std::endl;
    }

    //sockaddr_in是internet环境下套接字的地址形式
    sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(8888);//ip port都要转成网络字节序
    addr.sin_addr.s_addr=INADDR_ANY;//INADDR_ANY-->0.0.0.0。bind 0.0.0.0，当本主机拥有多个IP时，可以接受发送到本机所有IP的指定port的数据
    
    //绑定套接字到指定的IP+port
    //server端必须指明IP和port并把接口暴露给外界
    if(bind(sockfd,(struct sockaddr*)&addr,sizeof(addr))<0){
        std::cerr<<"bind failed"<<std::endl;
    };
    //开始
    while(true){
        char buffer[1024]={0};//负责接收client数据的缓冲区
        size_t length=sizeof(buffer);//缓冲区可接受的最大数据长度
        struct sockaddr_in client_addr;//client的地址数据
        socklen_t client_addr_len=sizeof(client_addr);//接收数据前需初始化，以指明(struct sockaddr*)空间的大小
        //阻塞等待接收数据
        recvfrom(sockfd,buffer,length,0,(struct sockaddr*)&client_addr,&client_addr_len);
        std::cout<<buffer<<std::endl;
        std::cout<<inet_ntoa(client_addr.sin_addr)<<" "<<ntohs(client_addr.sin_port)<<std::endl;

        //发送数据
        std::string s="我已收到数据";
        size_t send_len=s.size();
        sendto(sockfd,s.c_str(),send_len,0,(struct sockaddr*)&client_addr,client_addr_len);
    }

}