#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define BUFFER_SIZE 100

int main() {
    int counter = 0;
    char buffer[BUFFER_SIZE];

    printf("How are you today? Tell me please. (Good, Fine, Bad)\n");

    while (1) {
        printf("\r\n답변 횟수: %d \n응답: ", counter++);
        fflush(stdout);
        usleep(100000); // 0.1초 대기

        // non-blocking 입력 설정
        if (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
            // 입력된 문자열에서 개행 문자를 제거
            buffer[strcspn(buffer, "\n")] = '\0';

            // 종료 단어와 비교
            if (strcmp(buffer, "Good") == 0 || strcmp(buffer, "Fine") == 0 || strcmp(buffer, "Bad") == 0) {
                break;
            }
        }
    }

    printf("\nThank you for your response!\n");
    return 0;
}
