#include <stdio.h>
#include <stdlib.h>

int prepare_dummy_physical_memory_reorder(void * start_adr, void * end_adr) {
    // 파일을 쓰기 모드로 열기
    FILE * file = fopen("/home/hyunwoo-kim/os_project/kernel/mem_s/dummy_memory_address.txt", "w");
    if (file == NULL) {
        fprintf(stderr, "failed finding file \n");
        return 1;
    }

    // 값을 파일에 쓰기
    fprintf(file, "%x\n", start_adr);
    fprintf(file, "%x\n", end_adr);

    // 파일 닫기
    fclose(file);

    return 0;
}
