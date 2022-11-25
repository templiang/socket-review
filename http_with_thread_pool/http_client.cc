#include "socket_util.hpp"
using namespace socketUtil;

int main(int argc,char **argv){
    if(argc!=3){
        std::cout<<"usage: ./tcp_client serverip serverport"<<std::endl;
        return -1;
    }
    tcpSocket sock;
    sock.Connect(argv[1],argv[2]);
    sock.Close();
    return 0;
}