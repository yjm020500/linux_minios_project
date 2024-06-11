#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "system.h"

int get_first_available_pane(int * pane_arr) {
    for (int index = 0; index < 4; index++) {
        if (*(pane_arr + index) == 0) {
            *(pane_arr + index) = 1;
            return index + 2; // 인덱스가 0이면 pane 2, 인덱스가 1이면 pane 3, 등등
        }
    }
    return -1; // 모든 pane이 사용 중인 경우
}

void execute(void * virtual_physical_memory, FrameList * fl, FrameManager * fm, ProcessPool * pp) {
    char path[30]; // 프로그램 이름을 저장할 배열
    char full_path[40];

    int total_bytes = 0;
    char select; // 읽어온 값 선택하는 변수 ( y or n )

    int i = 0; // 프로그램 binary 읽어올 때 interate 변수 (for문에서 쓰는)

    int total_pages = 0;
    int pane_num = get_first_available_pane(pane_arr);

    if(pane_num == -1) {
        print_minios("\n 프로그램을 실행할 공간이 없습니다. \n");
        return;
    }

    printf("실행할 프로그램을 입력하세요 : ");
    scanf(" %s", path); // 사용자로부터 파일 이름을 입력 받음

    // 프로그램 중복 실행 검사
    Process * tmp = pp -> head;
    while (tmp != NULL) {
        if (!strcmp(tmp -> process_name, path)) {
            printf("\n \"%s\"는 이미 실행 중인 프로그램입니다. \n\n", path);
            return;
        }
        tmp = tmp -> next;
    }

    strcpy(full_path, "program/");
    strcat(full_path, path);

    FILE * fp = fopen(full_path, "rb");
    if (fp == NULL) {
        printf("\n 디렉토리에 해당 프로그램이 존재하지 않습니다. \n\n");
        perror("fopen");
        return;
    }

    // 동적할당 받은 64KB (최대값으로 설정하였음) 배열에 
    // 읽어온 프로그램의 바이트를 채워 넣을 것임
    // 
    unsigned int * byte = (unsigned int *)malloc(TOTAL_MEMORY_SIZE * sizeof(unsigned int));
    memory_set(byte, 0, 64 * 1024);
    // 이부분 좀 메모리 측면에서 낭비긴한데

    // 읽어오는 바이트의 양이 많으므로 선택적 처리
    while (1) {
        printf("읽어온 바이트 값을 출력하시겠습니까? (y/n) : ");
        scanf(" %c", &select);
        if (select == 'y' || select == 'n') break;
        else printf("잘못된 입력입니다. 다시 입력해주세요. \n\n");
    }
    
    while (((byte[i] = fgetc(fp)) != EOF)) {
        total_bytes++;
        i++;
    }
    
    if (select == 'y') {
        print_minios("\n======================================== \n");
        print_minios("처음 20 Bytes : \n");
        for (int index = 0; index < 20; index++) {
            printf("%02X ", byte[index]); // 예시로 읽어온 바이트 값을 16진수로 출력
        }
        print_minios("\n");
        print_minios("마지막 20 Bytes : \n");
        for (int index = total_bytes - 20; index < total_bytes ; index++) {
            printf("%02X ", byte[index]);
        }
        print_minios("\n");
    }

    fclose(fp);

    if (total_bytes > 65536) {
        printf("Program is too big! \n");
        return;
    }

    // 페이지수 결정
    if (total_bytes % 4096 != 0) {
        total_pages = (total_bytes/4096) + 1;
    } else {
        total_pages = (total_bytes/4096);
    }

    // 입력한 프로그램의 page manager 생성
    PageManager * page_manager = create_page_manager(total_pages);
    // page manageer 세팅 하고 페이지 테이블 생성
    set_page_data(page_manager, total_bytes, byte);
    printf("[ %s ] Program Page manager setting 완료 \n\n", path);

    free(byte);

    // 메모리에 값 적재
    if (count_empty_frames(fl) < page_manager -> allocated_pages) {
        printf("Not enough Frames \n");
        printf("Waiting Queue 미구현 ... \n");
        // 생성하였던 모든 프로그램 관련 동적할당 메모리 반환
        remove_page_manager(page_manager); 
        return;
        // 대기 Queue로 가야함
    }

    print_minios("======================================== \n");
    for (int i = 0; i < page_manager -> allocated_pages; i++) {
        Frame frame_out = get_first_empty_frame(fl, fm);
        page_manager -> pages[i].matched_frame = frame_out.frame_number;
        page_manager -> pages[i].is_matched_frame = 1;

        unsigned char * addr_ptr;
        addr_ptr = (virtual_physical_memory + (int)frame_out.first_address);
    
        for (int j = 0; j < PAGE_SIZE; j++) {
            *(addr_ptr + j) = page_manager -> pages[i].data[j];
        }
        printf(" [ %d번 page => %d번 frame에 매핑 ] \n", page_manager -> pages[i].page_number, page_manager -> pages[i].matched_frame);
    }
    print_minios("");
    print_minios("======================================== \n");

    // kernel에서 보여지는 영역 시작
    page_manager -> is_memory_loaded = 1;
    show_pf_table(page_manager, fm);
    addProcess(pp, page_manager, path, pane_num); // 프로세서 pool에 넣는다.
    print_minios("프로그램 적재(loading) 완료 \n");
    memory_view(virtual_physical_memory, 0, 20);

    char tmux_command[64];
    const char * terminal = "terminal";
    // const char * pane = "2";
    snprintf(tmux_command, sizeof(tmux_command), "tmux send-keys -t %s.%d './%s' C-m", terminal, pane_num, full_path);
    system(tmux_command);

    return;
}
