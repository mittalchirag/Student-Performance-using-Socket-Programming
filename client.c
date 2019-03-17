#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define MAX 12
#define PORT 8080
#define SA struct sockaddr
char* subjects[4]={"NPS","Compilers","DBMS","CG"};
int numberOfSubjects = 4;

int computeEligibility(int tt1, int tt2, int tt3, int attendance, int a1, int a2)
{
    int b1 = 0, b2 = 0;
    if (tt1 >= tt2)
    {
        b1 = tt1;
        b2 = (tt2 >= tt3) ? tt2 : tt3;
    }
    else if (tt1 < tt2)
    {
        b1 = tt2;
        b2 = (tt1 >= tt3) ? tt1 : tt3;
    }

    int total = (a1 + a2 + b1 + b2);
    total = ceil(total /= 2);

    return total;
}

void cpy(char *dest, char *src, int n)
{
    for (int i = 0; i < n; i++)
    {
        dest[i] = src[i];
    }
    dest[n] = '\0';
}
void func(int sockfd)
{
    char registration_number[MAX];
    int n;
    bzero(registration_number, sizeof(registration_number));
    printf("\nEnter the registration number : ");
    n = 0;
    //take the registration_numberfrom user
    while ((registration_number[n++] = toupper(getchar())) != '\n')
        ;
    cpy(registration_number, registration_number, 12);
    //exit the communicaation if user types EXIT
    if ((strncmp(registration_number, "EXIT", 4)) == 0)
    {
        printf("Client Exit...\n");
        close(sockfd);
        exit(0);
    }
    //send the registration number to server
    write(sockfd, registration_number, sizeof(registration_number));

    char studentCriteria[7][2];
    int studentCriteriaInt[7];
    memset(studentCriteria, '\0', sizeof(studentCriteria));
    printf("\n================Overall Performance of Student================\n");

    for (int i = 0; i < numberOfSubjects; i++)
    {
        printf("Performance of %s in %s: \n",registration_number, subjects[i]);

        printf("Component1\tAttendance\tComponent2\tResult\n");
        for (int i = 0; i < 7; i++)
        {
            char buffer[2];
            //receive data from server and store it in buffer
            recv(sockfd, buffer, 2, 0);
            //copy the data to the studentCriteria array
            cpy(studentCriteria[i], buffer, 2);
            //convert the string stored to integer
            studentCriteriaInt[i] = atoi(studentCriteria[i]);
           
        }
        //call the compute eligibility function for computing the result
        int res = computeEligibility(studentCriteriaInt[0], studentCriteriaInt[1], studentCriteriaInt[2], studentCriteriaInt[3], studentCriteriaInt[4], studentCriteriaInt[5]);
        int see = ceil(studentCriteriaInt[6] / 2);
        if (res >= 20 && studentCriteriaInt[3] >= 80)
        {
            if (see >= 20)
            {
                printf("%d\t\t%d\t\t%d\t\t%s\n", res, studentCriteriaInt[3],see, "Pass");
            }
            else if (see < 20)
            {
                printf("%d\t\t%d\t\t%d\t\t%s\n", res, studentCriteriaInt[3],see, "Fail");
            }
        }
        else
        {
            printf("%d\t\t%d\t\t%s\t\t%s\n", res, studentCriteriaInt[3],"NE", "Fail");
        }
        printf("\n");
    }
}

int main()
{
    // connect the client socket to server socket

    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    // socket create and varification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");

    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
    {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

    // function for sneding and recieving data
    func(sockfd);

    // close the socket
    close(sockfd);
}