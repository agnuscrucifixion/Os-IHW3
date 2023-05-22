#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <winsock.h>

#define PORT 8080
#define MAX_QUEUE_SIZE 10

typedef struct {
    int id;
    char name[50];
    int specialist;
} Patient;

int main() {
    int server_socket, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    Patient patients[MAX_QUEUE_SIZE];
    int patient_count = 0;
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_socket, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }



    while (1) {
        if ((new_socket = accept(server_socket, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }
        char buffer[1024] = {0};
        int valread = read(new_socket, buffer, 1024);
        if (patient_count < MAX_QUEUE_SIZE) {
            Patient patient;
            memcpy(&patient, buffer, sizeof(Patient));
            patients[patient_count++] = patient;
            printf("Patient %s added to the queue\n", patient.name);
            char *response = "Patient added to the queue";
            send(new_socket, response, strlen(response), 0);
        } else {
            char *response = "Queue is full. Patient cannot be added";
            send(new_socket, response, strlen(response), 0);
        }
    }

    return 0;
}