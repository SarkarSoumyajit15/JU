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
