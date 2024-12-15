#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>

#define SERVER_PORT 23456
#define MAX_BACKLOG 5

int main (int argc, char *argv)
{
     /* The struct sock_addr contains sin_family, sin_port, sin_addr */
    struct sockaddr_in Server_Details = { 0 },
                     Client_Details = { 0 };

    int i32ServerSocketFD   = 0,
        i32ClientSocketFD   = 0,
        i32ErrorCode        = 0,
        i32ReuseAddr        = 1,
        i32ClientDetSize    = sizeof (Client_Details),
        i32DataCount        = 0;

    char acBuffer[40]       = {0};
   
    /* Creating a socket for TCP, here IPPROTO_IP is 0, so the protocol will be decided by kernel based on the domain and family */
    i32ServerSocketFD = socket ( AF_INET, SOCK_STREAM, IPPROTO_IP );

    if ( 0 > i32ServerSocketFD )
    {
        perror("SOCKET CREATION FAILED");

        return 1;
    }

    /* Set the created socket to reuse IP and Port using SO_REUSEADDR */
    i32ErrorCode = setsockopt (i32ServerSocketFD, SOL_SOCKET, SO_REUSEADDR, &i32ReuseAddr, sizeof(i32ReuseAddr) );

    if ( 0 > i32ErrorCode )
    {
        perror("REUSEADDR FAILED");
        return 1;
    }

    Server_Details.sin_family = AF_INET;
    Server_Details.sin_port    = htons(23456);
    Server_Details.sin_addr.s_addr = INADDR_ANY; /* Use INADDR_ANY to so socket will listen to all interface, Such as ethernet and wifi */

    /* Bind the created socket to local IP and Port so that socket will listen to the port*/
    i32ErrorCode = bind ( i32ServerSocketFD, (struct sockaddr*)&Server_Details, sizeof(Server_Details) );

    if ( 0 > i32ErrorCode  )
    {
        perror("Binding Failed");

        close (i32ServerSocketFD);
        return 1;
    }

    i32ErrorCode = listen ( i32ServerSocketFD, MAX_BACKLOG );

    if ( 0 > i32ErrorCode  )
    {
        perror("Listen Failed");

        close (i32ServerSocketFD);
        return 1;
    }

    /* Since accept is a blocking call it would wait for any incoming connection request*/
    i32ClientSocketFD = accept (i32ServerSocketFD, (struct sockaddr*)&Client_Details, &i32ClientDetSize );

    if ( 0 > i32ClientSocketFD )
    {
        perror("Accept Failed");
        close (i32ServerSocketFD);
        
        return 1;
    }

    /* After successfully accepting, it would return a new file descriptor and Client_Details will have the details such as IP and port of client */
    i32DataCount = send (i32ClientSocketFD, "Hello Client", 12 , 0 ); /*12 refers to message size and 0 refers to flag */

    if ( 1 > i32DataCount )
    {
        if ( 0 == i32DataCount )
        {
            printf("Connection Closed");

            close (i32ServerSocketFD);
        }
        else
        {
            perror("ERR:");
        }
        return 1;
    }

    /* Receive a Message from client */
    i32DataCount = recv(i32ClientSocketFD, (void *)acBuffer, 12, 0 );

    if ( 1 > i32DataCount )
    {
        if ( 0 == i32DataCount )
        {
            printf("Connection Closed");

            close (i32ServerSocketFD);
        }
        else
        {
            perror("ERR:");
        }
        return 1;
    }
    
    printf( "Received Message : %s",acBuffer );

    close (i32ClientSocketFD);
    close (i32ServerSocketFD);

    return 0;
}