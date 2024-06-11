#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void get_memory_usage();

int main() {
    printf("메모리 사용량:\n");
    
    while (1) {
        get_memory_usage();
        sleep(3);
    }
    return 0;
}

void get_memory_usage() {
    FILE *fp;
    char buffer[256];
    long total, free, available;

    fp = fopen("/proc/meminfo", "r");
    if (fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    fgets(buffer, sizeof(buffer), fp);
    sscanf(buffer, "MemTotal: %ld kB", &total);

    fgets(buffer, sizeof(buffer), fp);
    sscanf(buffer, "MemFree: %ld kB", &free);

    fgets(buffer, sizeof(buffer), fp);
    sscanf(buffer, "MemAvailable: %ld kB", &available);

    fclose(fp);
    
    printf("Total: %ld kB, Free: %ld kB, Available: %ld kB\n", total, free, available);
}
