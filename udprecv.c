#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

/*
 * Function: Usage
 * ----------------------------
 *   Prints the usage message for the program and exits.
 *
 *   program_name: A pointer to the program name string.
 */
void Usage(char *program_name) {
    if (program_name == NULL) {
        program_name = "program";
    }
    printf("Usage: %s <port>\n", program_name);
    exit(EXIT_FAILURE);
}


int main(int argc, char **argv) {
    if (argc != 2) {
        Usage(argv[0]);
    }

    int port = atoi(argv[1]);

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr);
    
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Error binding socket");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    
    printf("Listening on %s:%d for incoming UDP messages...\n", inet_ntoa(addr.sin_addr) , port);


    char buffer[1024];
    struct sockaddr_in senderAddr;
    socklen_t senderAddrLen = sizeof(struct sockaddr);
    if (recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&senderAddr, &senderAddrLen) == -1) {

            perror("Error receiving data");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
    buffer[sizeof(buffer)-1] = '\0';
    printf("Received message from %s:%d\n", inet_ntoa(senderAddr.sin_addr), ntohs(senderAddr.sin_port));
    printf("Data: %s\n", buffer);
    
    close(sockfd);

}
