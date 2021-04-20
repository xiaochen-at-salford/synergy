#ifndef rs232_INCLUDED
#define rs232_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>



#ifdef __linux__

#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>

#else

#include <windows.h>

#endif

int OpenComport(char *, int);
int PollComport(unsigned char *, int);
int SendByte(unsigned char);
int SendBuf(unsigned char *, int);
void CloseComport();
int IsCTSEnabled();


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif

