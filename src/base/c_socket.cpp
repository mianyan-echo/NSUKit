#include "base/c_socket.h"

using namespace nsukit;



SocketGenerator::SocketGenerator(unsigned short int p, std::string c_ip, bool m) : port(p), ip(std::move(c_ip)),
                                                                                  mode(m) {
    tcpClientConnected = false;
}


SocketGenerator::~SocketGenerator() {
//    std::cout << "free socket server" << std::endl;
    CloseSock(tcpServer);

}


void SocketGenerator::Init(bool update, unsigned short int port, std::string ip) {
    if (update) {
        port = port;
        ip = ip;
    }
#ifdef _WIN32
    // WinSock service initialization
    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA wsaData;
    if (WSAStartup(sockVersion, &wsaData) != 0) {
        WSACleanup();
        throw std::runtime_error("Winsock server Init failed");
    }
    addr_len = sizeof(SOCKADDR);
    if (mode) {
        tcpClient = socket(AF_INET, SOCK_STREAM, 0);
        if (tcpClient == INVALID_SOCKET) {
            throw std::runtime_error("socket client error");
        }
        addrCli.sin_family = AF_INET;
        addrCli.sin_port = htons(port);
        addrCli.sin_addr.S_un.S_addr = inet_addr(ip.c_str());

        // Set 5s timeout
        setsockopt(tcpClient, SOL_SOCKET, SO_RCVTIMEO, (char *)&recvTimeout, sizeof(recvTimeout));
    } else {
        tcpServer = socket(AF_INET, SOCK_STREAM, 0);
        if (tcpServer == INVALID_SOCKET) {
            throw "socket server error";
        }
        // Bind ip and port
        addrSer.sin_family = AF_INET;
        addrSer.sin_port = htons(port);
        addrSer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
        // addrSer.sin_addr.S_un.S_addr = inet_addr(ip);
        if (bind(tcpServer, (SOCKADDR *) &addrSer, addr_len) == SOCKET_ERROR) {
            throw "socket bind error";
        }
        if (listen(tcpServer, LISTEN_COUNT) == SOCKET_ERROR) {
            throw "socket listen error";
        }
    }

#else
    addr_len = sizeof(struct sockaddr_in);
    timeval timeout = {(int)(recvTimeout/1000), 0};
    if (mode) {
        tcpClient = socket(AF_INET, SOCK_STREAM, 0);
        if (tcpClient == -1) {
            throw "socket client error";
        }
        addrCli.sin_family = AF_INET;
        addrCli.sin_port = htons(port);
        addrCli.sin_addr.s_addr = inet_addr(ip.c_str());
        // 设置5s超时
        setsockopt(tcpClient, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    }
    else {
        tcpServer = socket(AF_INET, SOCK_STREAM, 0);
        if (tcpServer == -1) {
            throw "socket server error";
        }
        // 绑定ip和端口
        addrSer.sin_family = AF_INET;
        addrSer.sin_port = htons(port);
        addrSer.sin_addr.s_addr = htonl(INADDR_ANY);
        // addrSer.sin_addr.S_un.S_addr = inet_addr("192.168.1.111");
        if (bind(tcpServer, (struct sockaddr*)&addrSer, addr_len) == -1) {
            throw "socket bind error";
        }
        if (listen(tcpServer, LISTEN_COUNT) == -1) {
            throw "socket listen error";
        }
    }

#endif
}


int SocketGenerator::ConnectServer(float connect_time) {
    // Returns -1 on failure, 0 on success
    int ul, recv;
#ifdef _WIN32
    ul = 1;
    ioctlsocket(tcpClient, FIONBIO, (unsigned long *) &ul);
    recv = connect(tcpClient, (SOCKADDR *) &addrCli, addr_len);  // Setting non-blocking returns -1,
    ul = 0;
    ioctlsocket(tcpClient, FIONBIO, (unsigned long *) &ul);
#else
    ul = fcntl(tcpClient, F_GETFL, 0);
    fcntl(tcpClient, F_SETFL, ul | O_NONBLOCK);
    recv = connect(tcpClient, (struct sockaddr*)&addrCli, addr_len);
    ul = fcntl(tcpClient, F_GETFL, 0);
    fcntl(tcpClient, F_SETFL, ul & ~O_NONBLOCK);
#endif
    tcpClientConnected = false;

    fd_set rfds, wfds;
    timeval tv = {(int)connect_time, 0};
    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    FD_SET(tcpClient, &rfds);
    FD_SET(tcpClient, &wfds);
    /* set select() time out */
//    tv.tv_sec = connect_time;  // s
//    tv.tv_usec = 0;  // us
    int selres = select(tcpClient + 1, &rfds, &wfds, NULL, &tv);
    if (selres == 0) {
        tv = {(int)connect_time, 0};
        selres = select(tcpClient + 1, &rfds, &wfds, NULL, &tv);
    }
    switch (selres) {
        case -1:
            printf("select error\n");
            break;
        case 0:
            printf("select time out\n");
            break;
        case 2:
            printf("select time out\n");
            break;
        default:
            if (FD_ISSET(tcpClient, &rfds) || FD_ISSET(tcpClient, &wfds)) {
                // success
                recv = 0;
                tcpClientConnected = true;
            }
            break;
    }

    return recv;
}


int SocketGenerator::AcceptClient() {
    // Return -1 on failure
#ifdef _WIN32
    tcpClient = accept(tcpServer, (SOCKADDR *) &addrCli, &addr_len);
    tcpClientConnected = true;
    return tcpClient;
#else
    tcpClient = accept(tcpServer, (struct sockaddr*)&addrCli, &addr_len);
    tcpClientConnected = true;
    return tcpClient;
#endif
}

// The sent data length is returned when send/recv is successful, 
// 0 is returned when the connection is completed, 
// and SOCKET_ERROR(-1) is returned when the connection fails.
int SocketGenerator::RecvData(int s, char *buf, int len) {
    int recv_count = recv(s, buf, len, 0);
    if (recv_count > 0) {
        return recv_count;
    } else {
         tcpClientConnected = false;
        return 0;
    }
}


int SocketGenerator::SendData(int s, char *buf, int len) {
    int sent_count = send(s, buf, len, 0);
    WSAGetLastError();
    if (sent_count > 0) {
        return sent_count;
    } else {
         tcpClientConnected = false;
        return 0;
    }
}


void SocketGenerator::CloseSock(int s) {
#ifdef _WIN32
    closesocket(s);
    WSACleanup();
#else
    close(s);
#endif
    tcpClientConnected = false;
}


int nsukit::TcpRecvBytes(SocketGenerator *s, char *buf, size_t len, bool *stopped) {
    int rxLen = 0;
    int totalLen = 0;
    int cur_count;

    while (len) {
        if (stopped && *stopped) {
            return 1;
        }
        cur_count = len - totalLen > BLOCK_SIZE ? BLOCK_SIZE : len - totalLen;
        rxLen = s->RecvData(s->tcpClient, buf + totalLen, cur_count);
        if (rxLen == 0) {
            return 0;
        } else if (rxLen > 0) {
            totalLen += rxLen;
            if (totalLen >= len) {
                break;
            }
        }
    }

    return totalLen;
}


int nsukit::TcpSendBytes(SocketGenerator *s, char *buf, size_t len, bool *stopped) {
    int stLen = 0;
    int totalLen = 0;
    int cur_count;

    while (len) {
        if (stopped && *stopped) {
            return 1;
        }
        cur_count = len - totalLen > BLOCK_SIZE ? BLOCK_SIZE : len - totalLen;
        stLen = s->SendData(s->tcpClient, buf + totalLen, cur_count);
        if (stLen == 0) {
            // server connect interrupt
            return 0;
        } else if (stLen > 0) {
            totalLen += stLen;
            if (totalLen >= len) {
                break;
            }
        }
    }

    return 1;
}
