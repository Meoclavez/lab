#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <linux/if_ether.h>

int main() {
    int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL)), count = 0, len;
    if (sock < 0) return perror("Socket Error"), 1;

    unsigned char buf[65536];
    printf("Packet Capture Started...\n");

    while ((len = recvfrom(sock, buf, sizeof(buf), 0, NULL, NULL)) > 0) {
        struct iphdr *ip = (struct iphdr *)(buf + 14); // Skip 14-byte Ethernet header

        printf("\nPacket No: %d\nPacket Size: %d bytes\nSource IP: %s\n",
               ++count, len, inet_ntoa(*(struct in_addr *)&ip->saddr));
        printf("Destination IP: %s\n-------------------------\n",
               inet_ntoa(*(struct in_addr *)&ip->daddr));
    }

    close(sock);
    return 0;
}
