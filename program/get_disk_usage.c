#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void get_disk_usage();

int main() {
    printf("디스크 사용량:\n");
    
    while (1) {
        get_disk_usage();
        sleep(3);
    }
    return 0;
}

void get_disk_usage() {
    FILE *fp;
    char buffer[128];

    // "df -h" 명령을 실행하여 디스크 사용량을 가져옴
    fp = popen("df -h", "r");
    if (fp == NULL) {
        perror("popen");
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer);
    }
    
    pclose(fp);
}
