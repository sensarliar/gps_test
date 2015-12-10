#ifndef GROUP_BROADCAST_H
#define GROUP_BROADCAST_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <memory.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>


//#define MCAST_PORT 31241
//#define LOCAL_PORT 56171
#define MCAST_PORT 6005
#define LOCAL_PORT 7005
#define MCAST_ADDR "224.1.2.3"
#define LocalIPAddr "10.10.106.108"
#define MCAST_DATA "111111222223333344444"
//unsigned char buffer[]={"111111222223333344444"};     // UDP packet limit size: 15000 bytes

extern int soketfd;  //sn?

extern struct sockaddr_in mcast_addr;


extern int init_group_broadcast();

#endif // GROUP_BROADCAST_H
