#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <thread>
#include "singleton_thread_pool.hpp"
#include "task_for_engine.hpp"
#define BUFSIZE 1024
using namespace thread_pool_util;

namespace socketUtil
{
    class tcpSocket
    {
    private:
        int _socketfd;

    public:
        tcpSocket()
        {
            _socketfd = socket(AF_INET, SOCK_STREAM, 0);
            if (_socketfd < 0)
            {
                std::cerr << "create socket failed" << std::endl;
            }
        }
        static void work()
        {
        }
        // server端
        void Bind(std::string port)
        {
            sockaddr_in addr;
            addr.sin_addr.s_addr = INADDR_ANY; //服务端绑定0.0.0.0，可以接收本机所有IP的指定port
            addr.sin_port = htons(atoi(port.c_str()));
            addr.sin_family = AF_INET;
            if (bind(_socketfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
            {
                std::cerr << "bind error" << std::endl;
                exit(1);
            }
        }
        //开启监听状态
        int Listen()
        {
            if (listen(_socketfd, 10) < 0)
            {
                // std::cerr<<"listen error"<<std::endl;
                return -1;
            }
            return 0;
        }
        //监听
        void Accept()
        {
            int fd;
            sockaddr_in client;
            socklen_t client_len = sizeof(client); //必须初始化，指明client的大小。
            // thread_pool<task_for_engine>::getInstance();
            thread_pool<task_for_engine>::getInstance();
            while (1)
            {
                printf("11111111111\n");
                fd = accept(_socketfd, (struct sockaddr *)&client, &client_len); //监听，并创建通信socket
                printf("22222222222\n");
                if (fd < 0)
                {
                    return;
                }
                thread_pool<task_for_engine>::getInstance()->push(task_for_engine(fd, client));
            }
        }
        void Connect(std::string ip, std::string port)
        {
            sockaddr_in addr;
            addr.sin_addr.s_addr = inet_addr(ip.c_str());
            addr.sin_port = htons(atoi(port.c_str()));
            addr.sin_family = AF_INET;
            if (connect(_socketfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
            {
                std::cerr << "connect failed" << std::endl;
                return;
            }
            std::cout << "connect sucess" << std::endl;
            std::string str;
            std::cout << "请输入：";
            std::cin >> str;
            sendto(_socketfd, str.c_str(), sizeof(str), 0, (struct sockaddr *)&addr, sizeof(addr));

            char buffer[BUFSIZE] = {0};
            recv(_socketfd, buffer, sizeof(buffer) - 1, 0);
            std::cout << buffer << std::endl;
        }
        void Close()
        {
            close(_socketfd);
        }
    };
}
// thread_pool<task_for_engine>* tcpSocket:: _tp=thread_pool<task_for_engine>::getInstance(10);
// int tcpSocket::_socketfd = socket(AF_INET, SOCK_STREAM, 0);