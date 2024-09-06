


/*

Name  : Soumyajit Sarkar
Roll No. : 002211001106
Section A2

Assignment Details :- Objective of this programming assignment is to use mmap() call and observe page-fault
                      using the ‘sar’ command. A big file (about 8GB) should be created using the ‘fallocate’ command.
                      This big file should be written with a single integer value (say X) at a specific offset (say F).
                      Both the integer value and the offset should be generated using a random function.
                      Please do remember this random function should generate a quantity anywhere between 0 and 8G.
                      The above big file should also be mapped in the virtual address space using mmap() call.
                      Once it is mapped, the data should be read from the same specific offset (F).
                      Now, if the data read is X`; then verify that X and X` are the same.
                      In case of verification failure, an error message is to be printed.
                      Note that, the offset value F can be anywhere between 0 and 8G.
                      This sequence of writing and reading data to/from a specific offset and also verification should
                      be put in a while loop to go forever.
                      In another terminal execute the command ‘sar –B 1 1000’ to observe for the page fault.
                      This command should be started before the above program is put under execution.
                      So, one can observe that the page faults are increasing, once the above program starts executing.
                      The output of the program and the ‘sar’ command should be pasted as a comment at
                      the beginning of the program file.

Input Description  :- No input is needed. The file is by default created in the program

Output Description :-

    1)  The program prints the value written in the offset of the buffer and also the value read from the same offset
    2)  The program compares and prints the message for successful verification of both these values.


Compilation command : gcc 106_Ass4.c
Execution command  : ./a.out

Sample Output of the program : 

Verification succeeded at offset 0x22E8ECF8: X = 115, X_read = 115
Verification succeeded at offset 0x2D07D36D: X = 134, X_read = 134
Verification succeeded at offset 0x328FD805: X = 179, X_read = 179
Verification succeeded at offset 0x56FCBC5: X = 181, X_read = 181
Verification succeeded at offset 0x3A47A326: X = 227, X_read = 227
Verification succeeded at offset 0x2CA78928: X = 68, X_read = 68
Verification succeeded at offset 0x1B433EC2: X = 146, X_read = 146
Verification succeeded at offset 0x3604B39A: X = 165, X_read = 165
Verification succeeded at offset 0xA81114: X = 242, X_read = 242
Verification succeeded at offset 0x3588B7ED: X = 162, X_read = 162
Verification succeeded at offset 0xC9E7138: X = 254, X_read = 254
Verification succeeded at offset 0x2AE532CE: X = 66, X_read = 66
Verification succeeded at offset 0x299EEC64: X = 198, X_read = 198
Verification succeeded at offset 0x20719817: X = 91, X_read = 91
Verification succeeded at offset 0x30DC2783: X = 182, X_read = 182
Verification succeeded at offset 0x34B42529: X = 141, X_read = 141
Verification succeeded at offset 0x318D4713: X = 123, X_read = 123
Verification succeeded at offset 0x29FF712E: X = 151, X_read = 151
Verification succeeded at offset 0x319ED74C: X = 24, X_read = 24
Verification succeeded at offset 0x3DF17FB: X = 244, X_read = 244
Verification succeeded at offset 0x3975DA38: X = 114, X_read = 114
Verification succeeded at offset 0x28A4C504: X = 35, X_read = 35
Verification succeeded at offset 0x1286ADC9: X = 250, X_read = 250
Verification succeeded at offset 0x257757EC: X = 158, X_read = 158
Verification succeeded at offset 0x9918E40: X = 221, X_read = 221


Sample output of sar command : 

*/




#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <time.h>
#include <stdint.h>
#include <string.h>

#define FILE_SIZE (1L * 1024L * 1024L * 1024L)  // 1GB file size
#define FILE_NAME "largefile"

int main() {
    int fd;
    int X, X_read;
    off_t F;
    void *mapped;

    // Seed the random number generator
    srand(time(NULL));

    // Create or open the file with read and write permissions
    fd = open(FILE_NAME, O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    // Allocate space for the file using fallocate
    if (fallocate(fd, 0, 0, FILE_SIZE) == -1) {
        perror("Error allocating file space");
        close(fd);
        return 1;
    }


    //  Memory mapping the file
    mapped = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapped == MAP_FAILED) {
        perror("Error mapping file");
        close(fd);
        return 1;
    }




    // Infinite loop
    while (1) {
        // Step 1: Generate random value X (0-255)
        X = rand() % 256;

        // Step 2: Generate random offset F (0 - FILE_SIZE)
        F = (off_t) (rand() % FILE_SIZE);

        // Step 3: Write the byte X at offset F
        sprintf((uint8_t *)mapped + F, "%d", X);

        // Step 5: Read the byte from the same offset
        sscanf((uint8_t *)mapped + F, "%d", &X_read);

        // Step 6: Verify that the written byte (X) and read byte (X_read) are the same
        if (X != X_read) {
            printf("Verification failed at offset 0x%lX: X = %d, X_read = %d\n", (unsigned long)F, X, X_read);
            munmap(mapped, FILE_SIZE);
            close(fd);
            return 1;
        } else {
            printf("Verification succeeded at offset 0x%lX: X = %d, X_read = %d\n", (unsigned long)F, X, X_read);
        }


    }

     if (munmap(mapped, FILE_SIZE) == -1) {
            perror("Error unmapping file");
            close(fd);
            return 1;
        }

    // Close the file
    close(fd);
    return 0;
}
