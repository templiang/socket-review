#include "socket_util.hpp"
using namespace socketUtil;
int main(int argc,char **argv){

    tcpSocket sock;//创建sock
    sock.Bind(argv[1]);//绑定端口
    sock.Listen();
    sock.Accept();
    sock.Close();
    return 0;
}