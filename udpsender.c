#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>



 void Usage(char *program_name) {
        printf("Usage: %s <port> <ip_address> <message to send>\n", program_name);
        exit(0);
    }


int main(int argc, char *argv[]) {

    int sockfd;
    struct sockaddr_in target_address;
    char message[1024];
    strcpy(message, argv[3]);

   
    if (argc != 4) {
        Usage(argv[0]);
    }
    

    if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Unable to create udp socket\n");
        exit(1);
    }


    int port = atoi(argv[1]);
    target_address.sin_family = AF_INET;
    target_address.sin_port = htons(port);
    memset(&(target_address.sin_zero), '\0', 8);
    inet_pton(AF_INET6, argv[2], &(target_address.sin_addr));



    if (sendto(sockfd, message, strlen(message), 0, (struct sockaddr *) &target_address, sizeof(struct sockaddr)) == -1) {
        perror("Unable to send message\n");
        exit(1);
    }
    printf("Sent %ld bytes message to %s: %d\n", strlen(message), inet_ntoa(target_address.sin_addr), ntohs(target_address.sin_port));
    close(sockfd);

}
