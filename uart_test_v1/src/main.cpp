#include <QCoreApplication>
#include <iostream>
using namespace std;

extern "C"
{
    #include <unistd.h>
    #include <assert.h>
    #include "uart.h"
}

struct Msg
{
    Msg()
    {
        nState = 1;
        targetID = 13;
        lat = 123.15267;
    }

    unsigned char nState; // 1, [0~255]
    int targetID; // 2, -2^15~2^15-1, [-32,768, 32,767]
    float lat; // 4,
};

int openPort();
int msgSend(int fd);
int msgRecv(int fd);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int fd = openPort();
    cout<< " fd = " << fd << endl;
    while(true)
    {// 低位在前，协议头:0XAA0XAA0X29，和校验
//        msgSend(fd);
        int ret = msgRecv(fd);
        cout << " ret = " << ret << endl;
        usleep(50*1000);
    }

    return a.exec();
}

int openPort()
{
    int fd;
    if((fd = Open_Device()) < 0)
    {
        perror("open_port error");
        return -1;
    }
    printf("fd=%d\n", fd);

    if((set_opt(fd, 115200, 8, 'N', 1)) < 0)
    {
        perror("set_opt error");
        return -1;
    }
    return fd;
}

int msgSend(int fd)
{
    // 低位在前，协议头:0XAA0XAA+0X29，和校验
    Msg msgSend;
    const int SEND_BUFF_LENGTH = 3+1 +1+2+4 +1; // 12
    unsigned char buff[SEND_BUFF_LENGTH] = {0X00};
    buff[0] = 0XAA;
    buff[1] = 0XAA;
    buff[2] = 0X29;
    buff[3] = (uint8_t)(SEND_BUFF_LENGTH-5); // 16-(3+1+1)=11

    // shuju
    buff[4] = (uint8_t)msgSend.nState;
    char tid[2] = "\0";
    *(int16_t *)tid = (int16_t)msgSend.targetID;
    buff[5] = tid[0];  buff[6] = tid[1];

    char lat[4] = "\0";
    *(int32_t *)lat = (int32_t)(msgSend.lat * 1e6);
    buff[7] = lat[0];
    buff[8] = lat[1];
    buff[9] = lat[2];
    buff[10] = lat[3];

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
    write_bytes(fd, buff, SEND_BUFF_LENGTH);
}

int msgRecv(int fd)
{
    const int RECV_BUFF_LENGTH = (3+1 +1+2+4 +1)*2; // 12 * 2
    unsigned char buff[RECV_BUFF_LENGTH] = "\0";
    int nread = read_bytes(fd, buff, RECV_BUFF_LENGTH);
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
        return 1;

    if (sIdx > RECV_BUFF_LENGTH-nData-2)
        return 1;

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
