#pragma once
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <cstdlib>
#include <string>
#define BUFSIZE 1024
#define WWWROOT "./"
#define PATH "index.html"

class task_for_engine
{
private:
    int fd;
    sockaddr_in client;

public:
    // typename run handlerHttpRequest;
    task_for_engine()
    {
    }
    task_for_engine(int sockfd, sockaddr_in addr) : fd(sockfd), client(addr)
    {
    }
    void run()
    {
        int client_len = sizeof(client);
        std::string client_ip = inet_ntoa(client.sin_addr);
        uint16_t client_port = ntohs(client.sin_port);
        std::cout << client_ip << ":" << client_port;
        std::cout << "fd:" << fd << std::endl;

        char buffer[BUFSIZE] = {0};
        int length = recv(fd, buffer, sizeof(buffer) - 1, 0); //从通信socket读取数据
        buffer[length] = '\0';
        std::cout << buffer << std::endl;

        std::string respone;
        // //重定向
        // respone+="HTTP/1.1 301 Permanently moved\n";
        // respone+="Location: https://www.qq.com/\n";
        // respone+="\n";
        // send(fd, respone.c_str(), respone.size(), 0);
        respone += "HTTP/1.1 200 OK\n";
        respone += "Content-Type:text/html\n";
        respone += "Set-Cookie:id=liang\n";
        respone += "Set-Cookie:passwd=Liang\n";
        struct stat fileStat;
        std::string html_path = WWWROOT;
        html_path += PATH;
        stat(html_path.c_str(), &fileStat);
        respone += "Content-Length:";
        respone += std::to_string(fileStat.st_size);
        respone += "\n";
        respone += "\n";

        std::ifstream html(html_path);
        if (!html.is_open())
        {
            std::cerr << "failed to open file" << std::endl;
        }
        std::string line;
        while (std::getline(html, line))
        {
            respone += line;
        }
        send(fd, respone.c_str(), respone.size(), 0); //向client发送数据
        close(fd);
    }
};