#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include "uart.h"

int set_opt(int fd, int nSpeed, int nBits, char nEvent, int nStop)  
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
  
  
int Open_Device(void)  
{  
    int fd; 
     
    fd = open("/dev/ttyTHS0",O_RDWR|O_NOCTTY|O_NDELAY);
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

#if 0
int read_num_bytes(int fd,unsigned char* buff,int length)
{
	int nread = 0;
	int num   = 0;
	unsigned char*pbuff = buff;
	
	while(num < length)
	{
		if((nread = read(fd,pbuff,length)) > 0)
		{
		    num += nread;
		    pbuff += nread;
		    nread = 0;
	    }
	    usleep(1);
	}
    return length;
}
#endif

int write_bytes(int fd, unsigned char* buff, int length)
{
    int nwrite = 0;
    nwrite = write(fd,buff,length);
    return nwrite;
}

int read_bytes(int fd, unsigned char *buff, int length)
{
    int nread = 0;
    nread = read(fd, buff, length);
    return nread;
}
