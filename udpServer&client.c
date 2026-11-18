//server

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void translate(char *str) {
    const char *abbr[] = {"tbh","idc","ig","tbf","atm","irl","lol","asap","omg","ttyl","idk","nvm"};
    const char *full[] = {"to be honest","I don't care","I guess","to be fair","at the moment",
                          "in real life","laughing out loud","as soon as possible","oh my God",
                          "talk to you later","I don't know","never mind"};
    char res[1024] = "", *tok = strtok(str, " ,.!?\n");

    while (tok) {
        const char *rep = tok;
        for (int i = 0; i < 12; i++)
            if (strcasecmp(tok, abbr[i]) == 0) { rep = full[i]; break; }
        strcat(res, rep);
        strcat(res, " ");
        tok = strtok(NULL, " ,.!?\n");
    }
    strcpy(str, res);
}

int main() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in s_addr = {AF_INET, htons(8080), INADDR_ANY}, c_addr;
    socklen_t len = sizeof(c_addr);
    char buf[1024];

    bind(sock, (struct sockaddr *)&s_addr, sizeof(s_addr));
    printf("UDP Server Running...\n");

    int n = recvfrom(sock, buf, sizeof(buf) - 1, 0, (struct sockaddr *)&c_addr, &len);
    buf[n] = '\0';
    printf("Received: %s\n", buf);

    translate(buf);
    sendto(sock, buf, strlen(buf) + 1, 0, (struct sockaddr *)&c_addr, len);
    printf("Translated message sent.\n");
    close(sock);
    return 0;
}


//client

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in addr = {AF_INET, htons(8080), inet_addr("127.0.0.1")};
    char buf[1024];

    printf("Enter sentence:\n");
    fgets(buf, sizeof(buf), stdin);

    sendto(sock, buf, strlen(buf) + 1, 0, (struct sockaddr *)&addr, sizeof(addr));
    recvfrom(sock, buf, sizeof(buf), 0, NULL, NULL);

    printf("\nTranslated Sentence:\n%s\n", buf);
    close(sock);
    return 0;
}
