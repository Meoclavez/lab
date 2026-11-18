//server

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>

int main() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in s_addr = {AF_INET, htons(8080), INADDR_ANY}, c_addr;
    socklen_t len = sizeof(c_addr);
    char buf[64];

    bind(sock, (struct sockaddr *)&s_addr, sizeof(s_addr));
    printf("UDP Time Server Running...\n");

    while (1) {
        recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&c_addr, &len);
        time_t t = time(NULL);
        char *str = ctime(&t);
        sendto(sock, str, strlen(str), 0, (struct sockaddr *)&c_addr, len);
    }
}

//client

#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in addr = {AF_INET, htons(8080), inet_addr("127.0.0.1")};
    char buf[64];

    sendto(sock, "TIME", 4, 0, (struct sockaddr *)&addr, sizeof(addr));
    int n = recvfrom(sock, buf, sizeof(buf) - 1, 0, NULL, NULL);
    buf[n] = '\0';

    printf("Server Time: %s", buf);
    close(sock);
    return 0;
}
