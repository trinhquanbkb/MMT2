#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int countString(char *str)
{
    int i, ans = 0;
    int len = strlen(str);
    char tmp[10];
    if (len > 256)
        len = 256;
    for (i = 0; i + 10 < len; i++)
    {
        strncpy(tmp, str + i, 10);
        if (strcmp(tmp, "0123456789") == 0)
            ans++;
    }
    return ans;
}

int main(int argc, char *argv[])
{
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1)
    {
        perror("socket() failed");
        return 1;
    }

    int portNum = atoi(argv[1]);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(portNum);

    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)))
    {
        perror("bind() failed");
        return 1;
    }

    if (listen(listener, 5))
    {
        perror("listen() failed");
        return 1;
    }
    printf("Waiting for clients...\n");

    struct sockaddr_in clientAddr;
    int clientAddrLen = sizeof(addr);

    int client = accept(listener, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (client == -1)
    {
        perror("accept() failed!");
        return 1;
    }

    char buf[1500];
    char tmpStr[16];
    strcpy(tmpStr, "");
    while (1)
    {
        int ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
        {
            printf("Ket noi bi dong.\n");
            break;
        }
        if (ret < sizeof(buf))
            buf[ret] = 0;

        printf("So chuoi ky tu tim duoc la: %d\n", countString(buf));
    }

    close(client);
    close(listener);
}