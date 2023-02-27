#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <string>
#include <pthread.h>

#define PORT 8080
#define NUM_CLIENTS 5

void *client_thread(void *thread_id)
{
    int sock = 0; 
	long valread;
    struct sockaddr_in serv_addr;
    std::string hello = "CLIENT: Hello from client " + std::to_string((unsigned long)thread_id);
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return nullptr;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return nullptr;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nCLIENT: Connection Failed \n");
        return nullptr;
    }
    send(sock , hello.c_str() , hello.length() , 0 );
    printf("CLIENT: Hello message sent from client %lu\n", (unsigned long)thread_id);
    valread = read( sock , buffer, 1024);
    printf("CLIENT: Response from server: %s\n",buffer );
    return nullptr;
}

int main()
{
    pthread_t clients[NUM_CLIENTS];
    for (int i = 0; i < NUM_CLIENTS; i++) {
        pthread_create(&clients[i], NULL, client_thread, (void *)(i+1));
    }
    for (int i = 0; i < NUM_CLIENTS; i++) {
        pthread_join(clients[i], NULL);
    }
    return 0;
}


// #include <stdio.h>
// #include <sys/socket.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <netinet/in.h>
// #include <string.h>
// #include <arpa/inet.h>
// #include <string>
// #include <thread>
// #include <functional>

// #define PORT 8080
// #define NUM_CLIENTS 5

// void client_thread(int thread_id)
// {
//     int sock = 0; long valread;
//     struct sockaddr_in serv_addr;
//     std::string hello = "Hello from client " + std::to_string(thread_id);
//     char buffer[1024] = {0};

//     if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
//     {
//         printf("\n Socket creation error \n");
//         return;
//     }

//     memset(&serv_addr, '0', sizeof(serv_addr));

//     serv_addr.sin_family = AF_INET;
//     serv_addr.sin_port = htons(PORT);

//     if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
//     {
//         printf("\nInvalid address/ Address not supported \n");
//         return;
//     }

//     if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
//     {
//         printf("\nConnection Failed \n");
//         return;
//     }
//     send(sock , hello.c_str() , hello.length() , 0 );
//     printf("Hello message sent from client %d\n", thread_id);
//     valread = read( sock , buffer, 1024);
//     printf("Response from server: %s\n",buffer );
// }

// int main()
// {
//     std::thread clients[NUM_CLIENTS];
//     // for (int i = 0; i < NUM_CLIENTS; i++) {
//     //     clients[i] = std::thread(std::bind(client_thread, i+1));
//     // }
// 	for (int i = 0; i < NUM_CLIENTS; i++) {
//         clients[i] = std::thread([i](){ client_thread(i+1); });
//     }
//     for (int i = 0; i < NUM_CLIENTS; i++) {
//         clients[i].join();
//     }
//     return 0;
// }


// #include <stdio.h>
// #include <sys/socket.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <netinet/in.h>
// #include <string.h>
// #include <arpa/inet.h>
// #include <string>
// #include <thread>

// #define PORT 8080
// #define NUM_CLIENTS 5

// void client_thread(int thread_id)
// {
//     int sock = 0; long valread;
//     struct sockaddr_in serv_addr;
//     std::string hello = "Hello from client " + std::to_string(thread_id);
//     char buffer[1024] = {0};

//     if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
//     {
//         printf("\n Socket creation error \n");
//         return;
//     }

//     memset(&serv_addr, '0', sizeof(serv_addr));

//     serv_addr.sin_family = AF_INET;
//     serv_addr.sin_port = htons(PORT);

//     if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
//     {
//         printf("\nInvalid address/ Address not supported \n");
//         return;
//     }

//     if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
//     {
//         printf("\nConnection Failed \n");
//         return;
//     }
//     send(sock , hello.c_str() , hello.length() , 0 );
//     printf("Hello message sent from client %d\n", thread_id);
//     valread = read( sock , buffer, 1024);
//     printf("Response from server: %s\n",buffer );
// }

// int main()
// {
//     std::thread clients[NUM_CLIENTS];
//     for (int i = 0; i < NUM_CLIENTS; i++) {
//         clients[i] = std::thread(client_thread, i+1);
//     }
//     for (int i = 0; i < NUM_CLIENTS; i++) {
//         clients[i].join();
//     }
//     return 0;
// }
