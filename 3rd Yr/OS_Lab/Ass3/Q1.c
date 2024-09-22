 *
 * Team Number:
 * Member:
 *
 * Date:
 *
 * Assignment: 3, IPC using Socket
 *
 *   Objective:
 *       Transfer a file (1GB) between parent and child processes
 *     uisng a socket, and compare them.
 *
 *       This communicaton will be happening between processes within the same host,
 *     so will be using UNIX Domain Sockets.
 *
 *       Steps involved in this program:
 *     Step 1: Create Socket, establish connection and print its info
 *     Step 2: Given File -> (parent) -> |Scoket| -> (child) -> output-2
 *     Step 3: Given File -> (child) -> |Socket| -> (parent) -> output-1
 *     Step 4: Comapre output-1 & output-2
 *     Step 5: Print file transfer time
 *
 *
 * Input Description:
 *
 * Output Description:
 *
 *
 * Compilation Command:
 * Execution Sequence:
 *
 *
 * Sample Input:
 * Sample Output:
 /-----------------------------------

 -----------------------------------/
***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/types.h>

#define SOCKET_PATH "unix_socket"
#define BUFFER_SIZE 1024

long getTimeElapsed()
{
    struct timeval timeElapsed;
    gettimeofday(&timeElapsed, NULL);
    return timeElapsed.tv_sec * 1000 + timeElapsed.tv_usec / 1000;
}

void error_exit(const char*);
void createServer(int *, struct sockaddr_un *);
void createClient(int *, struct sockaddr_un);
void acceptConnection(int, int *, struct sockaddr_un);
void sendFile(int, const char *);
void receiveFile(int, const char *);
int compare_files(const char *, const char *);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>", argv[0]);
        exit(EXIT_FAILURE);
    }

     int startingTime = getTimeElapsed();




    int server_fd, client_fd, connection_fd;
    struct sockaddr_un server_addr, client_addr;
    pid_t child;
    const char *output_file1 = "output_file1";
    const char *output_file2 = "output_file2";
    int choice;
    int status = 0;



    // Step 1
 //   createServer

    if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        error_exit("[-]Socket failed");
    }

    // Unlink path if already exists
    unlink(SOCKET_PATH);

    // Specifing the address
    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    // Bind the socket to the specified path
    if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr_un)) < 0) {
        close(server_fd);
        error_exit("[-]Bind failed");
    }

    // Listen for incoming connections
    if (listen(server_fd, 5) < 0) {
        close(server_fd);
        error_exit("Listen failed");
    }

    printf("[+]Server is listening on %s\n", SOCKET_PATH);

    //  createClient  : --


    if ((client_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        error_exit("[-]Socket failed");
    }

    // Connect to the server
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_un)) < 0) {
        close(client_fd);
        error_exit("[-]Client failed to connect");
    }

    printf("[+]Client is ready to connect\n");




    //  Now client will accept the connection : --

     //acceptConnection : --

     socklen_t client_addr_len = sizeof(client_addr);

    if ((connection_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len)) < 0) {
        error_exit("[-]Failed to accept connection");
    }

    char *client_path = client_addr.sun_path[0] == '\0' ? "<path_not_specified>" : client_addr.sun_path;
    printf("[+]Client Connected on %s\n", client_path);


    // Create child process
    if ((child = fork()) < 0) {
        error_exit("Fork failed");
    }

    // Step 2

    // In parent
    if (child > 0) {
        printf("[+]Step 2\n");
        sendFile(connection_fd, argv[1]);
        // waitpid(child, &status, 0);


        //printf("Enter 1 to send the file back to the parent\n");
        //printf("Enter 0 exit the process");
        //scanf("%d",&choice);
        //printf("\n");
        //waitpid(child, &status, 0);
    }
    // In child
    else {
        receiveFile(client_fd, output_file2);
    }




    // Step 3

    // In parent

   // if(choice == 1){


    if (child > 0) {
        printf("[+]Step 3\n");
        receiveFile(connection_fd, output_file1);
        waitpid(child, &status, 0);
    }
    // In child
    else {
        sendFile(client_fd, argv[1]);
        exit(0);
    }

    // Step 4 - Parent Process
    if (child > 0) {
        printf("[+]Step 4\n");

        if (compare_files(output_file1, output_file2) == 0) {
            printf("[+]Files are identical\n");
        } else {
            printf("[+]Files aren't identical\n");
        }
    }


     int endingTime = getTimeElapsed();
        // Calculating the round-about time for transferring the file between process1 and process2 and back to process1
        int totalTimeTaken = endingTime - startingTime;

         printf(">>> The round-about time for transferring the file between process1 and process2 and back to process1 is %d milliseconds\n", totalTimeTaken);
    //}
    // Clean up
    unlink(SOCKET_PATH);
    close(server_fd);
    close(client_fd);
    close(connection_fd);

    return 0;
}

void error_exit(const char *message) {
    perror(message);
    unlink(SOCKET_PATH);
    exit(EXIT_FAILURE);
}


void sendFile(int fd, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        error_exit("[-]Failed to open file for reading");
    }

    char buffer[BUFFER_SIZE];
    size_t bytes_read;

    printf("[+]Sending file\n");

    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        // printf("%s", buffer);
        if (send(fd, buffer, bytes_read, 0) < 0) {
            error_exit("[-]Failed to send file data");
        }
    }

    printf("[+]File sent\n");

    shutdown(fd, SHUT_WR);

    fclose(file);
}

void receiveFile(int fd, const char *output_filename) {
    FILE *file = fopen(output_filename, "wb");
    if (!file) {
        error_exit("[-]Failed to open file for writing");
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_received;

    printf("[+]Receiving file\n");

    while ((bytes_received = recv(fd, buffer, BUFFER_SIZE, 0)) > 0) {
        // printf("%s\n", buffer);
        fwrite(buffer, 1, bytes_received, file);
    }

    if (bytes_received < 0) {
        error_exit("[-]Failed to receive file data");
    }

    printf("[+]File received\n");

    fclose(file);
}

int compare_files(const char *file1, const char *file2) {
    FILE *f1 = fopen(file1, "r");
    FILE *f2 = fopen(file2, "r");

    if (!f1) {
        error_exit("[-]Compare: Couldn't open file1");
    }
    if (!f2) {
        fclose(f1);
        error_exit("[-]Compare: Couldn't open file2");
    }

    char ch1 = fgetc(f1);
    char ch2 = fgetc(f2);

    // Match one character at a time
    while (ch1 != EOF && ch2 != EOF) {
        if (ch1 != ch2) {
            fclose(f1);
            fclose(f2);
            return 1;
        }
        ch1 = fgetc(f1);
        ch2 = fgetc(f2);
    }

    // One file ended prematurly
    if ((ch1 != EOF) || (ch2 != EOF)) {
        fclose(f1);
        fclose(f2);
        return 1;
    }

    fclose(f1);
    fclose(f2);
    return 0;
}

            
