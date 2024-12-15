#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>

#define SERVER_PORT 23456
#define SERVER_IP "192.168.12.13" /* This is sample IP of server*/

int main ( int argc, char *argv )
{
    struct sockaddr_in ServerDetails = {0};

    int i32ClientSocket = 0,
        i32Errorcode    = 0,
        i32DataReadCnt  = 0;

    char acBuffer[40]   = {0};

    i32ClientSocket = socket (AF_INET, SOCK_STREAM, IPPROTO_IP);

    if ( 0 > i32ClientSocket )
    {
        perror("Socket Creation Failed");
        return 1;
    }

    ServerDetails.sin_family        = AF_INET;
    ServerDetails.sin_port          = htons (SERVER_PORT);
    ServerDetails.sin_addr.s_addr   = inet_addr (SERVER_IP);

    /* Establish a connection to Server */
    i32Errorcode = connect (i32ClientSocket, (struct sockaddr*)&ServerDetails, sizeof(ServerDetails));

    if ( 0 > i32Errorcode )
    {
        perror("CONNECT ERROR");

        close (i32ClientSocket);
        return 1;
    }

    i32DataReadCnt = recv (i32ClientSocket, acBuffer, 12, 0); 
    
    if ( 1 > i32DataReadCnt )
    {
        if ( 0 == i32DataReadCnt )
        {
            printf("Connection Closed");

            close (i32ClientSocket);
        }
        else
        {
            perror("ERR:");
        }
        return 1;
    }

    print ("Received Message %s \n",acBuffer);

    i32DataReadCnt = send ( i32ClientSocket, "GoodBye Server", 15, 0 );

    if ( 1 > i32DataReadCnt )
    {
        if ( 0 == i32DataReadCnt )
        {
            printf("Connection Closed");

            close (i32ClientSocket);
        }
        else
        {
            perror("ERR:");
        }
        return 1;
    }

    close (i32ClientSocket);
    
    return 0;
}