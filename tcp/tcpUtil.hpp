#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <thread>
#define BUFSIZE 1024

namespace tcpUtil
{
    class tcpSocket
    {
    public:
        static int _socketfd;

    public:
        tcpSocket() {}
        static void work()
        {
        }
        // server端
        static int Bind(std::string port)
        {
            sockaddr_in addr;
            addr.sin_addr.s_addr = INADDR_ANY; //服务端绑定0.0.0.0，可以接收本机所有IP的指定port
            addr.sin_port = htons(atoi(port.c_str()));
            addr.sin_family = AF_INET;
            if (bind(_socketfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
            {
                std::cerr << "bind error" << std::endl;
                return -1;
            }
            return 0;
        }
        //开启监听状态
        static int Listen(int socketfd = _socketfd)
        {
            if (listen(socketfd, 10) < 0)
            {
                std::cerr << "listen error" << std::endl;
                return -1;
            }
            return 0;
        }
        //监听
        static void Accept(int socketfd = _socketfd)
        {
            int fd;
            sockaddr_in client;
            socklen_t client_len = sizeof(client); //必须初始化，指明client的大小。
            while (1)
            {
                fd = accept(socketfd, (struct sockaddr *)&client, &client_len); //监听，并创建通信socket
                if (fd < 0)
                {
                    return;
                }
                std::thread thrd([=]()
                                 {
                std::string client_ip = inet_ntoa(client.sin_addr);
                uint16_t client_port = ntohs(client.sin_port);
                std::cout <<"ip:"<< client_ip << " port:" << client_port<<std::endl;
                std::cout << "fd:" << fd << std::endl;
                char buffer[BUFSIZE] = {0};
                int length = recv(fd, buffer, sizeof(buffer) - 1, 0); //从通信socket读取数据
                buffer[length] = '\0';
                std::cout << buffer << std::endl;
                std::string str ("srever:data have accepted");
                sendto(fd, str.c_str(), str.size(), 0, (struct sockaddr *)&client, client_len); //向client发送数据
                close(fd); });
                // std::cout << "detach" << std::endl;
                thrd.detach();
            }
        }
        static void Connect(std::string ip, std::string port, int socketfd = _socketfd)
        {
            sockaddr_in addr;
            addr.sin_addr.s_addr = inet_addr(ip.c_str());
            addr.sin_port = htons(atoi(port.c_str()));
            addr.sin_family = AF_INET;
            if (connect(socketfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
            {
                std::cerr << "connect failed" << std::endl;
                return;
            }
            std::cout << "connect sucess" << std::endl;
            std::string str;
            std::cout << "请输入：";
            std::cin >> str;
            sendto(socketfd, str.c_str(), sizeof(str), 0, (struct sockaddr *)&addr, sizeof(addr));

            char buffer[BUFSIZE] = {0};
            recv(socketfd, buffer, sizeof(buffer) - 1, 0);
            std::cout << buffer << std::endl;
        }
        static void Close(int socketfd = _socketfd)
        {
            close(_socketfd);
        }
    };
    int tcpSocket::_socketfd = socket(AF_INET, SOCK_STREAM, 0);
}