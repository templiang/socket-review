#include "tcpUtil.hpp"
using namespace tcpUtil;

int main(int argc,char **argv){
    if(argc!=2){
        std::cout<<"usage: ./tcp_server port"<<std::endl;
        return -1;
    }

    tcpSocket::Bind(argv[1]);
    tcpSocket::Listen();
    tcpSocket::Accept();
    tcpSocket::Close();
    return 0;
}