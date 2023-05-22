#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <winsock.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080

typedef struct {
    int id;
    char name[50];
    int specialist;
} Patient;

int main() {
    int client_socket;
    struct sockaddr_in server_address;
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }



    while (1) {
        Patient patient;
        printf("Enter patient ID (or enter -1 to exit): ");
        scanf("%d", &patient.id);
        if (patient.id == -1) {
            break;
        }
        printf("Enter patient name: ");
        scanf("%s", patient.name);
        printf("Enter specialist (1 - Dentist, 2 - Surgeon, 3 - Therapist): ");
        scanf("%d", &patient.specialist);
        send(client_socket, &patient, sizeof(Patient), 0);
        char buffer[1024] = {0};
        read(client_socket, buffer, 1024);
        printf("Server response: %s\n", buffer);
    }
    close(client_socket);
    return 0;
}
