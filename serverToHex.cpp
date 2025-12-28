//
// Created by Joel Amroodt on 12/22/25.
//
#include <cctype>
#include <cstdio>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/errno.h>
#include <sys/select.h>

int main()
{
    //get local address
    printf("Configuring server address...\n");

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM; //TCP
    hints.ai_flags = AI_PASSIVE;    //server side???

    struct addrinfo *bind_address;
    getaddrinfo(0, "8080", &hints, &bind_address);

    printf("Creating socket...\n");
    int socket_listen;
    socket_listen = socket(bind_address->ai_family, bind_address->ai_socktype,bind_address->ai_protocol);
    if (socket_listen < 0)
    {
        printf("Error creating socket %d.\n", errno);
        return 1;
    }
    //bind

    printf("Binding socket to local address...");
    if (bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen))
    {
        fprintf(stderr, "bind() failed, %d\n", errno);
        return 1;
    }
    freeaddrinfo(bind_address);
    //listen
    printf("Listening...\n");
    if (listen(socket_listen, 5) < 0)
    {
        fprintf(stderr, "listen() failed, %d\n", errno);
        return 1;
    }

    //connect
    fd_set master;
    FD_ZERO(&master);
    FD_SET(socket_listen, &master);
    int socket_max = socket_listen;

    printf("Waiting for connections...\n");
    while (1)
    {
        fd_set reads;
        reads = master;
        // if select() return -1, return error response
        if (select(socket_max + 1, &reads, NULL, NULL, NULL) < 0)
        {
            fprintf(stderr, "select() failed, %d\n", errno);
            return 1;
        }

        // sockets only return positive ints on success
        // traverse through the returned fd_set
        for(int i = 1; i <= socket_max; i++)
        {
            if(FD_ISSET(i, &reads))
            {
                if(i == socket_listen)
                {
                    struct sockaddr_storage client_address;
                    socklen_t client_len = sizeof(client_address);

                    int socket_client = accept(socket_listen, (struct sockaddr *)&client_address, &client_len);
                    if (socket_client < 0)
                    {
                        fprintf(stderr, "accept() failed, %d\n", errno);
                        return 1;
                    }
                    FD_SET(socket_client, &master);
                    if (socket_client > socket_max)
                        socket_max =socket_client;

                    char address_buffer[100];
                    getnameinfo((struct sockaddr*) &client_address, client_len, address_buffer,
                        sizeof(address_buffer), 0, 0,NI_NUMERICHOST);
                    printf("New connection from %s\n", address_buffer);
                }
                else
                {
                    char read[1024];
                    memset(read, 0, sizeof(read));
                    int bytes_received = recv(i, read, sizeof(read), 0);
                    if (bytes_received <1)
                    {
                        FD_CLR(i, &master);
                        close(i);
                        continue;
                    }
                    {
                        int j = bytes_received;
                        if(j <= 4)
                        {
                            uint8_t one, two, three, four;
                            one = read[0];
                            two = read[1];
                            three = read[2];
                            four = read[3];
                            printf("0x%02X 0x%02X 0x%02X 0x%02X", one, two, three, four);
                            uint32_t first;
                            first =
                                ((uint32_t)one << 24) |
                                ((uint32_t)two << 16) |
                                ((uint32_t)three << 8) |
                                ((uint32_t) four);
                            read[0] = (first >> 24) & 0xFF;
                            read[1] = (first >> 16) & 0xFF;
                            read[2] = (first >> 8)  & 0xFF;
                            read[3] = first & 0xFF;
                            send(i, read, bytes_received, 0);

                        }
                        else if( j > 4 && j <= 8)
                        {
                            uint8_t one, two, three, four, five , six, seven, eight;
                            one = read[0];
                            two = read[1];
                            three = read[2];
                            four = read[3];
                            five = read[4];
                            six = read[5];
                            seven = read[6];
                            eight =read[7];
                            printf("0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X", one, two, three, four, five,
                                six, seven, eight);
                            uint32_t first, second;
                            first =
                                ((uint32_t)one << 24) |
                                ((uint32_t)two << 16) |
                                ((uint32_t)three << 8) |
                                ((uint32_t) four);
                            printf("\n0x%08X", first);
                            second =
                                ((uint32_t)five << 24) |
                                ((uint32_t)six << 16) |
                                ((uint32_t)seven << 8) |
                                ((uint32_t)eight);
                            printf("\n0x%08X", second);
                            read[0] = (first >> 24) & 0xFF;
                            read[1] = (first >> 16) & 0xFF;
                            read[2] = (first >> 8)  & 0xFF;
                            read[3] = first & 0xFF;

                            read[4] = (second >> 24) & 0xFF;
                            read[5] = (second >> 16) & 0xFF;
                            read[6] = (second >> 8)  & 0xFF;
                            read[7] = second & 0xFF;

                            send(i, read, bytes_received, 0);
                        }

                    }
                }
            }
        }
    }
    printf("Closing socket...\n");
    close(socket_listen);
}
//
// Created by Joel Amroodt on 12/28/25.
//
