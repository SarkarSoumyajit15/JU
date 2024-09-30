#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 256


void print_processor_info() {
    FILE *fp = fopen("/proc/cpuinfo", "r");
    if (fp == NULL) {
        perror("Failed to open /proc/cpuinfo");
        return;
    }

    char line[256];
    char vendor_id[256] = {0};
    char model_name[256] = {0};
    char cache_size[256] = {0};

    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "vendor_id") != NULL) {
            sscanf(line, "vendor_id : %[^\n]", vendor_id);
        } else if (strstr(line, "model name") != NULL) {
            sscanf(line, "model name : %[^\n]", model_name);
        } else if (strstr(line, "cache size") != NULL) {
            sscanf(line, "cache size : %[^\n]", cache_size);
        }

         //Stop reading if we've found all the required information
          if (vendor_id[0] && model_name[0] && cache_size[0]) {
             break;
          }
     }

     fclose(fp);

     // Print the collected information
     printf("Processor Information:\n");
     printf("Vendor ID: %s\n", vendor_id);
     printf("Model Name: %s\n", model_name);
     printf("Cache Size: %s\n", cache_size);
 }



void print_kernel_info() {
    char buffer[MAX_LINE_LENGTH];

    // Print Kernel Name and Release
    FILE *file = fopen("/proc/version", "r");
    if (file == NULL) {
        perror("Error opening /proc/version");
        return;
    }
    if (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("Kernel Information: %s", buffer);
    } else {
        perror("Error reading /proc/version");
    }
    fclose(file);
}

void print_os_info() {
    char buffer[MAX_LINE_LENGTH];

    // Print OS Type
    FILE *file = fopen("/proc/sys/kernel/ostype", "r");
    if (file == NULL) {
        perror("Error opening /proc/sys/kernel/ostype");
        return;
    }
    if (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("Operating System Type: %s", buffer);
    } else {
        perror("Error reading /proc/sys/kernel/ostype");
    }
    fclose(file);

    // Print OS Release
    file = fopen("/proc/sys/kernel/osrelease", "r");
    if (file == NULL) {
        perror("Error opening /proc/sys/kernel/osrelease");
        return;
    }
    if (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("Operating System Release: %s", buffer);
    } else {
        perror("Error reading /proc/sys/kernel/osrelease");
    }
    fclose(file);
}

void print_memory_info() {
    FILE *file = fopen("/proc/meminfo", "r");
    if (file == NULL) {
        perror("Error opening /proc/meminfo");
        return;
    }

    char label[MAX_LINE_LENGTH];
    unsigned long value;
    char unit[MAX_LINE_LENGTH];

    while (fscanf(file, "%s %lu %s", label, &value, unit) == 3) {
        if (strcmp(label, "MemTotal:") == 0) {
            printf("Total Memory: %lu %s\n", value, unit);
        } else if (strcmp(label, "MemFree:") == 0) {
            printf("Free Memory: %lu %s\n", value, unit);
        }
    }

    fclose(file);
}

void print_uptime() {
    FILE *file = fopen("/proc/uptime", "r");
    if (file == NULL) {
        perror("Error opening /proc/uptime");
        return;
    }

    double uptime_seconds;
    if (fscanf(file, "%lf", &uptime_seconds) != 1) {
        perror("Error reading /proc/uptime");
        fclose(file);
        return;
    }
    fclose(file);

    int hours = (int)(uptime_seconds / 3600);
    int minutes = (int)((uptime_seconds - hours * 3600) / 60);
    int seconds = (int)(uptime_seconds - hours * 3600 - minutes * 60);

    printf("Uptime: %d hours, %d minutes, %d seconds\n", hours, minutes, seconds);
}

int main() {
    printf("System Information:\n");

    // Print Kernel Information
    print_kernel_info();

    // Print OS Information
    print_os_info();

    // Print Memory Information
    print_memory_info();

    // Print Uptime Information
    print_uptime();

    // Print Processor info
    print_processor_info();

    return EXIT_SUCCESS;
}

