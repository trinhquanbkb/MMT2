#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{

    char *fileName = argv[1];
    FILE *fptr;

    if ((fptr = fopen(fileName, "r")) == NULL)
    {
        printf("File %s doesn't exist!!!", fileName);
        return 0;
    }

    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    char *serverIP = argv[2];
    int portNum = atoi(argv[3]);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(serverIP);
    addr.sin_port = htons(portNum);

    int ret = connect(client, (struct sockaddr *)&addr, sizeof(addr));
    if (ret == -1)
    {
        perror("connect() failed");
        return 1;
    }

    char num[1500];
    fscanf(fptr, "%s", num);
    send(client, num, strlen(num), 0);

    close(client);
    fclose(fptr);
}