#include "group_broadcast.h"


int soketfd;  //sn?
//    int err;

//  int count = 0;
// int temp =0x11;
int ttl =150;
struct sockaddr_in mcast_addr;
struct sockaddr_in local_addr;         // Sender socket address structure

// main(int argc, char **argv)
int init_group_broadcast()
{

    //struct ip_mreq host_madd_ifadd;     //struct init instance
  char arm_addr[20] = "192.168.1.2";
    char* arg_addr = arm_addr;

    soketfd=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    //soketfd=socket(AF_INET,SOCK_RAW,IPPROTO_UDP);
    if(soketfd == -1 )
    {
        perror("socket()");
        return -1;
    }

    bzero(&local_addr, sizeof(struct sockaddr_in) );
    bzero(&mcast_addr, sizeof(struct sockaddr_in) );
    mcast_addr.sin_family =AF_INET;
    mcast_addr.sin_port=htons(MCAST_PORT);
    mcast_addr.sin_addr.s_addr =inet_addr(MCAST_ADDR);
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(LOCAL_PORT);

    if(arg_addr!= NULL )
    {
        if(inet_pton(AF_INET, arg_addr, &local_addr.sin_addr.s_addr) <= 0)
        {
            printf("set ip address error!\n");
            exit(0);
        }
    }
    else
    {
            printf("please give a local address ip !\n");
            exit(0);
    }

 /* if(argv[2]!= NULL )
    {
            local_addr.sin_port = htons(atoi(argv[2]));
            mcast_addr.sin_port=    htons(atoi(argv[2]));
            printf("port:%d\n",atoi(argv[2]));

    }
    else
    {
        local_addr.sin_port = htons(LOCAL_PORT);
        mcast_addr.sin_port=htons(MCAST_PORT);
    }*/

 // local_addr.sin_addr.s_addr =inet_addr(LocalIPAddr);

    if(bind(soketfd,(struct sockaddr *)&local_addr, sizeof(local_addr) ) < 0 )
    {
        printf("bind() error \n");

    }

  setsockopt(soketfd,IPPROTO_IP,IP_MULTICAST_TTL,&ttl,sizeof(ttl));
//  strcpy(buffer,MCAST_DATA);
/*
    while(1)
    {
        int n =0;
        n=sendto(soketfd,buffer,sizeof(buffer),0,(struct sockaddr *)&mcast_addr,sizeof(mcast_addr));
        if(n<0)
        {
            perror("sendto()");
            return -2;
        }
        usleep(2);
    }
    */
}
