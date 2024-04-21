#ifndef _C_SOCKET
#define _C_SOCKET

#include <iostream>
#include <cstdio>
#include <string>
#include <utility>

#ifdef linux
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/shm.h>
#include <fcntl.h>
#elif _WIN32

#include <winsock2.h>  //
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")
// C4996 'inet_addr': Use inet_pton() or InetPton() instead or define _WINSOCK_DEPRECATED_NO_WARNINGS to disable deprecated API warnings
#pragma warning(disable:4996)

#endif

#include "type.h"

namespace nsukit {
    #define LISTEN_COUNT             5
    #define SELECT_TIMEOUT           1
    #define RECV_DEFAULT_TIMEOUT     5000
    #define BLOCK_SIZE               1024


    /**
     * @code
     * char *data = (char *)"abcd";
     * unsigned short int port = 5050;
     * string s_ip("192.168.1.65");
     * const char *ip = s_ip.c_str();
     * int mode = 1;
     * SocketGenerator* s = new SocketGenerator(port, ip, mode);
     * if (mode) {
     *     cout << s->ConnectServer() << endl;
     *     cout << s->SendData(s->tcpClient, data, 10) << endl;
     * } else {
     *     while (1) {
     *         int sockConn = s->AcceptClient();
     *         cout << sockConn << endl;
     *         if (sockConn == -1) {
     *             cout << -1 << sockConn << endl;
     *             s->CloseSock(sockConn);
     *             continue;
     *          }
     *          int cmd_id = tcp_recv(*s, sockConn);
     *          if (cmd_id == CMD_ERR) {
     *              break;
     *          } else if (cmd_id == CMD_START) {
     *              // start
     *          } else if (cmd_id == CMD_STOP) {
     *              // stop
     *          }
     *          s->CloseSock(sockConn);
     *     }
     * }
     * @endcode
     */
    class NSU_DLLEXPORT SocketGenerator {
    private:
#ifdef linux
        struct sockaddr_in addrSer;
        struct sockaddr_in addrCli;
        socklen_t addr_len;
#else
        sockaddr_in addrSer{};
        sockaddr_in addrCli{};
        int addr_len{};
#endif
        unsigned short int port;
        std::string ip;
        bool mode;
    public:
        int recvTimeout;

        int tcpServer{};
        int tcpClient{};
        bool tcpClientConnected;

        // SocketGenerator() {};
        explicit SocketGenerator(unsigned short int port = 0, std::string ip = "", bool mode = true);

        ~SocketGenerator();

        void Init(bool update, unsigned short int port = 0, std::string ip = "");

        int AcceptClient();

        int ConnectServer(float connect_time = SELECT_TIMEOUT);

        int RecvData(int s, char *buf, int len);

        int SendData(int s, char *buf, int len);

        void CloseSock(int s);

        unsigned short int GetPort() const {
            return port;
        };

        std::string GetIp() {
            return ip;
        }
    };

    int TcpRecvBytes(SocketGenerator *s, char *buf, size_t len, bool *stopped = nullptr);

    int TcpSendBytes(SocketGenerator *s, char *buf, size_t len, bool *stopped = nullptr);
}
#endif