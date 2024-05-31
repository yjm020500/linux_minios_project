#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "print_util.h"

int prepare_dummy_physical_memory_reorder(void * start_adr, void * end_adr) {
    // 파일을 쓰기 모드로 열기
    FILE * file = fopen("kernel/mem_s/dummy_memory_address.txt", "w");
    if (file == NULL) {
        fprintf(stderr, "failed finding file \n");
        return 1;
    }

    // 값을 파일에 쓰기
    fprintf(file, "%p\n", start_adr);
    fprintf(file, "%p\n", end_adr);

    // 파일 닫기
    fclose(file);

    return 0;
}

// 가운데 화면에서 메모리 그림 보는 함수
void memory_view(unsigned char *memory, size_t from, size_t to, const char *tmux_session_name, const char *tmux_pane) {
    if((from < 0) || (to > 64 * 1024 - 1)) {
        fprintf(stderr, "Invalid address range.\n");
        return;
    }

    system("tmux send-keys -t 1 'clear' C-m"); 

    const char * output_file_path = "/tmp/memory_update"; // tmp/memory_update 파일을 메모리 스케치용으로 사용할거임

    int tmux_fd = open(output_file_path, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    FILE *tmux_fp = fdopen(tmux_fd, "w");

    for (size_t i = to; i >= from ; i--) { // from ~ to 로 메모리 어디부터 어디 까지 스케치 할건지, 낮은 번지가 아래 오도록 스케치
        fprintf(tmux_fp, "       ------------------------------------------------\n");
        fprintf(tmux_fp, "       | ADDRESS |   0x%04zX   | DATA |   0b", i);
        print_binary_to_file(memory[i], tmux_fp);
        fprintf(tmux_fp, "   |   ");
        if (i % (4 * 1024) == 0) {
            fprintf(tmux_fp, "%zu\n", i / (4 * 1024));
        }
        else fprintf(tmux_fp, "\n");

        if (i == 0) { // i가 0이면 size_t가 unsigned여서 underflow를 피하기 위해 루프 종료
            break;
        }
    }
    fprintf(tmux_fp, "       ------------------------------------------------\n");

    fclose(tmux_fp);

    // tmp/memory_update 파일에 스케치된거 출력 지정한 pane에 드로잉
    char tmux_command[256];
    snprintf(tmux_command, sizeof(tmux_command), "tmux send-keys -t %s.%s 'cat %s' C-m", tmux_session_name, tmux_pane, output_file_path);
    system(tmux_command);
}