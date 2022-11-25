#include "tcpUtil.hpp"
using namespace tcpUtil;
int main(int argc,char **argv){
    if(argc!=3){
        std::cout<<"usage: ./tcp_client serverip serverport [--optional socketfd]"<<std::endl;
        return -1;
    }
    tcpSocket::Connect(argv[1],argv[2]);
    tcpSocket::Close();
    return 0;
}