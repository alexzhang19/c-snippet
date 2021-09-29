#include <string>
#include <iostream>
#include "uartCom.h"
using namespace std;

extern "C"
{
    #include <unistd.h>
    #include <assert.h>
    #include <stdio.h>
    #include <string.h>
    #include <sys/types.h>
    #include <errno.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    #include <unistd.h>
    #include <termios.h>
    #include <stdlib.h>
}


uartCom::uartCom():m_fd(-1)
{

}


uartCom::~uartCom()
{

}


int uartCom::openDevice(const string &devName)
{
    int fd;
    fd = open(devName.c_str(), O_RDWR|O_NOCTTY|O_NDELAY);
    if(fd == -1)
    {
        perror("Can't Open Serial Port");
        return -1;
    }

    if(fcntl(fd,F_SETFL,0) < 0)
    {
        printf("fcntl failed\n");
    }else
    {
        printf("fcntl=%d\n",fcntl(fd,F_SETFL,0));
    }

    if(isatty(STDIN_FILENO) == 0)
    {
        printf("standard input is not a terminal device\n");
    }else
    {
        printf("isatty sucess!\n");
    }

    printf("fd-open=%d\n",fd);
    return fd;
}


int uartCom::setOpt(int fd, int nSpeed, int nBits, char nEvent, int nStop)
{
    struct termios newtio;
    struct termios oldtio;

    if(tcgetattr(fd, &oldtio) != 0)
    {
        perror("SetupSerial");
        return -1;
    }

    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag |= CLOCAL |CREAD;
    newtio.c_cflag &= ~CSIZE;

    switch(nBits)
    {
        case 7:
            newtio.c_cflag |= CS7;
        break;
        case 8:
        newtio.c_cflag |= CS8;
        break;
    }

    switch(nEvent)
    {
    case 'O':
        newtio.c_cflag |= PARENB;
        newtio.c_cflag |= PARODD;
        newtio.c_iflag |= (INPCK | ISTRIP);
        break;
    case 'E':
        newtio.c_iflag |= (INPCK |ISTRIP);
        newtio.c_cflag |= PARENB;
        newtio.c_cflag &= ~PARODD;
        break;
    case 'N':
        newtio.c_cflag &= ~PARENB;
        break;
    }

    switch(nSpeed)
    {
    case 2400:
        cfsetispeed(&newtio,B2400);
        cfsetospeed(&newtio,B2400);
        break;
    case 4800:
        cfsetispeed(&newtio,B4800);
        cfsetospeed(&newtio,B4800);
        break;
    case 9600:
        cfsetispeed(&newtio,B9600);
        cfsetospeed(&newtio,B9600);
        break;
    case 115200:
        cfsetispeed(&newtio,B115200);
        cfsetospeed(&newtio,B115200);
        break;
    case 460800:
        cfsetispeed(&newtio,B460800);
        cfsetospeed(&newtio,B460800);
        break;
    default:
        cfsetispeed(&newtio,B9600);
        cfsetospeed(&newtio,B9600);
        break;
    }

    if(nStop == 1)
    {
        newtio.c_cflag &= ~CSTOPB;
    }
    else if(nStop ==2)
    {
        newtio.c_cflag |= CSTOPB;
    }
    newtio.c_cc[VTIME] = 0;
    newtio.c_cc[VMIN] = 0;

    tcflush(fd,TCIFLUSH);
    if((tcsetattr(fd,TCSANOW,&newtio)) != 0)
    {
        perror("com set error");
        return -1;
    }
    printf("set done!\n");
    return 0;
}


int uartCom::openPort(string devName, int nSpeed, int nBits,
                      char nEvent, int nStop)
{
    if ("" == devName)
        devName = "/dev/ttyTHS0";

    if((m_fd = openDevice(devName)) < 0)
    {
        perror("open_port error");
        return -1;
    }
    printf("fd=%d\n", m_fd);
    if (m_fd<1 || m_fd>65535)
    {
        perror("fd error");
        return -1;
    }

    if((setOpt(m_fd, nSpeed, nBits, nEvent, nStop)) < 0)
    {
        perror("set_opt error");
        return -1;
    }

    return 0;
}


int uartCom::writeBytes(unsigned char* buff, int length)
{
    int nwrite = 0;
    nwrite = write(m_fd, buff, length);
    return nwrite;
}

int uartCom::readBytes(unsigned char *buff, int length)
{
    int nread = 0;
    nread = read(m_fd, buff, length);
    return nread;
}

void uartCom::msgSend()
{
    unsigned char nState = 1; // 1, [0~255]
    int targetID = 13; // 2, -2^15~2^15-1, [-32,768, 32,767]
    float lat = 123.15267; // 4

    // 低位在前，协议头:0XAA0XAA+0X29，和校验
    const int SEND_BUFF_LENGTH = 3+1 +1+2+4 +1; // 12
    unsigned char buff[SEND_BUFF_LENGTH] = {0X00};
    buff[0] = 0XAA;
    buff[1] = 0XAA;
    buff[2] = 0X29;
    buff[3] = (uint8_t)(SEND_BUFF_LENGTH-5); // 16-(3+1+1)=11

    // shuju
    buff[4] = (uint8_t)nState;
    char tid[2] = "\0";
    *(int16_t *)tid = (int16_t)targetID;
    buff[5] = tid[0];  buff[6] = tid[1];

    char clat[4] = "\0";
    *(int32_t *)clat = (int32_t)(lat * 1e6);
    buff[7] = clat[0];
    buff[8] = clat[1];
    buff[9] = clat[2];
    buff[10] = clat[3];

    assert(10 == SEND_BUFF_LENGTH-2);

    unsigned char sum = 0;
    for (int i=0; i<SEND_BUFF_LENGTH-1; ++i)
    {
        sum += buff[i];
    }
    buff[11] = sum;

    for (int i=0; i<SEND_BUFF_LENGTH; ++i)
    {
        cout << std::hex << int(buff[i]) << " ";
        if (i%20 == 19)
            cout << endl;
    }
    cout << endl;
    writeBytes(buff, SEND_BUFF_LENGTH);
}

int uartCom::msgRecv()
{
    const int RECV_BUFF_LENGTH = (3+1 +1+2+4 +1)*2; // 12 * 2
    unsigned char buff[RECV_BUFF_LENGTH] = "\0";
    int nread = readBytes(buff, RECV_BUFF_LENGTH);
    cout << "nread = " << nread << endl;
    for (int i=0; i<RECV_BUFF_LENGTH; ++i)
    {
        cout << std::hex << int(buff[i]) << " ";
        if (i%20 == 19)
            cout << endl;
    }

    const int nDataIdx = 3;
    int sIdx = nDataIdx;
    int nData = -1;
    while(sIdx < RECV_BUFF_LENGTH)
    {
        if (int(buff[sIdx-nDataIdx]) == 0xAA && int(buff[sIdx-nDataIdx+1]) == 0XAA
                && int(buff[sIdx-nDataIdx+2]) == 0X29)
        {
            nData = int(buff[sIdx]);
            break;
        }
        sIdx++;
    }

    if (nData < 1)
        return -1;

    if (sIdx > RECV_BUFF_LENGTH-nData-2)
        return -1;

    unsigned char sum = 0;
    for (int i=sIdx-nDataIdx, j=0; i<sIdx+nData+1; ++i, ++j)
    {
//        cout<< dec << " i=" << i << ", j=" << j << endl;
        sum += buff[i];
    }
//    cout<< "recv len: " << RECV_BUFF_LENGTH << endl;
    cout<< hex << "sum1 = " << int(sum) << endl;
    cout<< hex << "sum2 = " << int(buff[sIdx + nData +1]) << endl;

    int nState = (uint8_t)buff[sIdx+1];

    char tid[2] = {buff[sIdx+2], buff[sIdx+3]};
    int targetID = *(int16_t *)tid;

    char clat[4] = {buff[sIdx+4], buff[sIdx+5], buff[sIdx+6], buff[sIdx+7]};
    float lat = float(*((int32_t *)clat) / 1e6);

    cout << dec << " nData = " << nData << "\n"
         << dec << " nState = " << nState << "\n"
         << dec << " targetID = " << targetID << "\n"
         << dec << " lat = " << lat << "\n"
         << endl;

    return 0;
}
