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
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    char *serverIP = argv[1];
    int portNum = atoi(argv[2]);

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

    char pc[250], oDia[255], packet[1024] = "", size[20];
    int number;
    printf("Ten may tinh: ");
    scanf("%s", pc);
    printf("So o dia: ");
    scanf("%d", &number);
    strcat(packet, pc);
    strcat(packet, " ");
    char s[1];
    sprintf(s, "%d", number);
    strcat(packet, s);
    for (int i = 0; i < number; i++)
    {
        printf("Ten o dia %d: ", i + 1);
        scanf("%s", oDia);
        printf("Kich thuoc o dia %d: ", i + 1);
        scanf("%s", size);
        strcat(packet, oDia);
        strcat(packet, size);
    }
    send(client, packet, strlen(packet), 0);

    close(client);
}