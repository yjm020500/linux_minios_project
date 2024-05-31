#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

void delete_object_files(const char *dir_path) {
    struct dirent *entry;
    DIR *dp = opendir(dir_path);

    if (dp == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dp))) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);

        struct stat statbuf;
        if (stat(path, &statbuf) == -1) {
            perror("stat");
            continue;
        }

        if (S_ISDIR(statbuf.st_mode)) {
            // 재귀적으로 하위 디렉토리 탐색
            delete_object_files(path);
        } else if (S_ISREG(statbuf.st_mode)) {
            // 파일 확장자가 .o인지 검사
            const char *ext = strrchr(entry->d_name, '.');
            if (ext && strcmp(ext, ".o") == 0) {
                // 오브젝트 파일이면 삭제
                if (remove(path) == 0) {
                    printf("Deleted: %s\n", path);
                } else {
                    perror("remove");
                }
            }
        }
    }

    closedir(dp);
}

int main() {
    char *dir_path = "."; // 현재 디렉토리
    delete_object_files(dir_path);
    return 0;
}
