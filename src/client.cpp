//
// Created by Joel Amroodt on 12/22/25.
//
#include <cstdio>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    if (argc< 3)
    {
        fprintf(stderr, "Usage: %s hostname port\n", argv[0]);
        return 1;
    }

    printf("Configuring remote address...");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    struct addrinfo *peer_address;

    //take in the ip address from terminal
    if (getaddrinfo(argv[1], argv[2], &hints, &peer_address))
    {
        fprintf(stderr, "getaddrinfo() failed. %d\n", errno);
        return 1;
    };

    printf("Remote address is: ");
    char address_buffer[100];
    char service_buffer[100];
    getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen,
        address_buffer, sizeof(address_buffer),service_buffer, sizeof(service_buffer), NI_NUMERICHOST);
    printf("%s %s\n", address_buffer, service_buffer);

    printf("Creating socket...");
    int socket_peer = socket(peer_address->ai_family,peer_address->ai_socktype, peer_address->ai_protocol);
    if (socket_peer == -1)
    {
        fprintf(stderr, "socket() failed. %d\n", errno);
        return 1;
    }

    printf("Connecting...\n");
    if(connect(socket_peer, peer_address->ai_addr, peer_address->ai_addrlen))
    {
        fprintf(stderr, "connect() failed. %d\n", errno);
        return 1;
    }
    freeaddrinfo(peer_address);
    printf("Connected to remote server.\n");
    printf("To send data, enter text followed by enter.\n");

    // select multiplexer
    while (1)
    {
        fd_set reads;
        FD_ZERO(&reads);
        FD_SET(socket_peer, &reads);
        FD_SET(0, &reads);

        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 10000;
        if (select(socket_peer + 1, &reads, 0, 0, &timeout) < 0)
        {
            fprintf(stderr, "select failed. %d\n", errno);
        }
        if(FD_ISSET(socket_peer, &reads))
        {
            char read [4096];
            int bytes_received = recv(socket_peer, read, sizeof(read), 0);
            if (bytes_received < 1)
            {
                printf("Connection closed by peer.\n");
                break;
            }
            printf("Received (%d bytes): %.*s", bytes_received, bytes_received, read);
            for (int i = 0; i < bytes_received; i++)
            {
                printf("%02X ", (unsigned char)read[i]);
            }
            printf("\n");
        }
        if(FD_ISSET(0, &reads))
        {
            char read[4096];
            if(!fgets(read, 4096, stdin)) break;
            printf("Sending: %s\n", read);
            int bytes_sent = send(socket_peer, read, strlen(read), 0);
            printf("Sent %d bytes\n", bytes_sent);
        }

    }





}
