#ifndef _UART_H
#define _UART_H

int set_opt(int fd, int nSpeed, int nBits, char nEvent, int nStop);
int Open_Device(void);
int write_bytes(int fd,unsigned char *buff,int length);
int read_bytes(int fd, unsigned char *buff, int length);

#endif
