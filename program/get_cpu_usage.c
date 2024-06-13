#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void get_cpu_usage();

int main() {
    printf("CPU 사용량:\n");
    
    while (1) {
        get_cpu_usage();
        sleep(3);
    }
    return 0;
}

void get_cpu_usage() {
    FILE *fp;
    char buffer[128];
    char cpu[5];
    long user, nice, system, idle;

    fp = fopen("/proc/stat", "r");
    if (fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    fgets(buffer, sizeof(buffer), fp);
    sscanf(buffer, "%s %ld %ld %ld %ld", cpu, &user, &nice, &system, &idle);
    fclose(fp);

    printf("User: %ld, Nice: %ld, System: %ld, Idle: %ld\n", user, nice, system, idle);
}
