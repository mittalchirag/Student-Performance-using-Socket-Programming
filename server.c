#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <my_global.h>
#include <mysql.h>
#include <pthread.h>
#define MAX 12
#define PORT 8080
#define BACKLOG 10
#define SA struct sockaddr

int subjectPorts[] = {8081, 8082, 8083, 8084}; //NPS, Compilers, DBMS, CG
int numberOfSubjects = 4;

typedef struct student
{
    char value[2];
} student;

void cpy(char *dest, char *src, int n)
{
    for (int i = 0; i < n; i++)
    {
        dest[i] = src[i];
    }
    dest[n] = '\0';
}

int connectToSubjectServer(int sockfd, char *registrationNumber, int i)
{
    int subjectSockFD, subjectConnFD;
    struct sockaddr_in servaddr, cli;

    // socket create and varification
    subjectSockFD = socket(AF_INET, SOCK_STREAM, 0);
    if (subjectSockFD == -1)
    {
        printf("socket creation failed...\n");
        exit(0);
    }
    

    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(subjectPorts[i]);

    // connect the client socket to server socket
    if (connect(subjectSockFD, (SA *)&servaddr, sizeof(servaddr)) != 0)
    {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the %d server..\n", subjectPorts[i]);

    // function for sending and recieving data
    printf("Registration Number Sent:%s\n", registrationNumber);
    write(subjectSockFD, registrationNumber, 12);
    char studentCriteria[7][2];
    int studentCriteriaInt[7];
    memset(studentCriteria, '\0', sizeof(studentCriteria));
    // close the socket
    for (int i = 0; i < 7; i++)
    {
        //send the data back to client
        char buffer[2];
        //receive data from server and store it in buffer
        recv(subjectSockFD, buffer, 2, 0);
        //copy the data to the studentCriteria array
        cpy(studentCriteria[i], buffer, 2);
        //convert the string stored to integer
        studentCriteriaInt[i] = atoi(studentCriteria[i]);
        printf("%d\t", studentCriteriaInt[i]);
        send(sockfd, studentCriteria[i], 2, 0);
    }
    close(subjectSockFD);
}

// Function designed for chat between client and server.
void func(int sockfd)
{
    //buffer to store registration_number
    char registration_number[MAX];
    //infinitely loop

    bzero(registration_number, MAX);
    //read the registration_number sent from the client side
    read(sockfd, registration_number, sizeof(registration_number));
    printf("Registration Number queried: %s\n ", registration_number);
    //exit if user sends EXIT
    if ((strncmp(registration_number, "EXIT", 4)) == 0)
    {
        printf("Server Exit...\n");
        close(sockfd);
        exit(0);
    }
    for (int i = 0; i < numberOfSubjects; i++)
    {
        //fetch_data from SQL db and store it in student Criteria buffer
        connectToSubjectServer(sockfd, registration_number, i);
    }
}

// Driver function
int main()
{
    int sockfd, connfd, len; //socket file
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("socket creation failed...\n");
        close(sockfd);
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0)
    {
        printf("socket bind failed...\n");
        close(sockfd);
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd, BACKLOG)) != 0)
    {
        printf("Listen failed...\n");
        close(sockfd);
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);

    while (1)
    {
        // Accept the data packet from client and verification
        connfd = accept(sockfd, (SA *)&cli, &len);
        if (connfd < 0)
        {
            printf("server acccept failed...\n");
            close(sockfd);
            exit(0);
        }
        else
            printf("server acccepted the client...\n");
        pid_t pid;
        if ((pid = fork()) == 0)
        {
            printf("Child process of pid: %d created\n", getpid());
            close(sockfd);
            func(connfd);
        }
        //function to receive, send and fetch_data
        close(connfd);
    }


    //close socket after working
    close(sockfd);
}