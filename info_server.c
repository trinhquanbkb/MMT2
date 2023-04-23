#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int findString(char *t, int to)
{
    for (int i = to; i <= strlen(t); i++)
    {
        if ((t[i] >= 65 && t[i] <= 90) || (t[i] >= 97 && t[i] <= 122))
        {
            return i;
        }
    }
}

int findNumber(char *t, int to)
{
    for (int i = to; i <= strlen(t); i++)
    {
        if (t[i] >= 48 && t[i] <= 57)
        {
            return i;
        }
    }
}

int findSpace(char *t)
{
    for (int i = 0; i <= strlen(t); i++)
    {
        if (t[i] == 32)
        {
            return i;
        }
    }
}

int count(int x, int y)
{
    int res = 1;
    for (int i = 0; i < y; i++)
    {
        res *= x;
    }
    return res;
}

int convertStringToInt(char *t)
{
    int number = 0, arrNum[100];
    for (int i = 0; i < strlen(t); i++)
    {
        arrNum[i] = t[i] - '0';
        number += arrNum[i] * count(10, i);
    }
    return number;
}

char *cutText(char *t, int start, int end)
{
    char *res = malloc(255 * sizeof(char));
    for (int i = start; i <= end; i++)
    {
        char x[1];
        x[0] = t[i];
        strcat(res, x);
    }
    return res;
}

int main(int argc, char *argv[])
{
    // create socket
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1)
    {
        perror("socket() failed");
        return 1;
    }

    // server
    int portNum = atoi(argv[1]);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(portNum);

    // bind
    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)))
    {
        perror("bind() failed!\n");
        return 1;
    }

    // listen
    int backlog = 5;
    if (listen(listener, backlog) == -1)
    {
        perror("listen() failed!");
        return 1;
    }
    printf("Waiting for clients...\n");

    // accept
    struct sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    int client = accept(listener, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (client == -1)
    {
        perror("accept() failed!");
        return 1;
    }

    char buf[256];
    int ret = recv(client, buf, sizeof(buf), 0);
    if (ret <= 0)
    {
        printf("recv() failed.\n");
        exit(1);
    }
    char number[10];
    printf("Ten may tinh: %s\n", cutText(buf, 0, findSpace(buf)));
    sprintf(number, "%s", cutText(buf, findSpace(buf) + 1, findString(buf, findSpace(buf)) - 1));
    printf("So o dia: %s\n", number);
    int to = findString(buf, findSpace(buf));
    for (int x = 0; x < convertStringToInt(number); x++)
    {
        printf("Ten o dia %d: %s\n", x + 1, cutText(buf, to, findNumber(buf, to) - 1));
        printf("Kich thuoc o dia %d: %s\n", x + 1, cutText(buf, findNumber(buf, to), findString(buf, findNumber(buf, to)) - 1));
        to = findString(buf, findNumber(buf, to));
    }
    close(listener);
}