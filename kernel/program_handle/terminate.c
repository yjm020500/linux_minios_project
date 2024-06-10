#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "system.h"

void execute(void * virtual_physical_memory, FrameList * fl, FrameManager * fm) {
    char process_name[20]; // 프로그램 이름을 저장할 배열

    printf("종료할 프로그램을 입력하세요 : ");
    scanf("%s", process_name); // 사용자로부터 파일 이름을 입력 받음

    // 입력한 프로그램의 page manager 생성
    PageManager * page_manager = create_page_manager(total_pages);
    // page manageer 세팅 하고 페이지 테이블 생성
    set_page_data(page_manager, total_bytes, byte);
    printf("[ %s ] Program Page manager setting 완료 \n\n", path);

    free(byte);

    // fill_frames(virtual_physical_memory, page_manager, fl, fm, byte);

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

    page_manager -> is_memory_loaded = 1;
    show_pf_table(page_manager, fm);
    // 프로세서 pool에 넣는다.

    print_minios("프로그램 적재(loading) 완료 \n");
    
    memory_view(virtual_physical_memory, 0, 20);

//  kill로 바꿔야함
    char tmux_command[64];
    const char * terminal = "terminal";
    // const char * pane = "2";
    snprintf(tmux_command, sizeof(tmux_command), "tmux send-keys -t %s.%d './%s' C-m", terminal, pane, full_path);
    system(tmux_command);
    pane--;

    return;
}