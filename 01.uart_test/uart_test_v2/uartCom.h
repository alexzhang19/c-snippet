#ifndef UART_H
#define UART_H
#include <string>
using namespace std;

class uartCom
{
public:
    uartCom();
    ~uartCom();

private:
    int m_fd;
    int openDevice(const string &devName);
    int setOpt(int fd, int nSpeed, int nBits, char nEvent, int nStop);

public:
    int openPort(string devName, int nSpeed=115200, int nBits=8, char nEvent='N',
                 int nStop=1);
    int writeBytes(unsigned char* buff, int length);
    int readBytes(unsigned char *buff, int length);

    // ceshi
    void msgSend();
    int msgRecv();
};

#endif // UART_H
