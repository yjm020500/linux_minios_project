#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_SIZE 4096 // 4KB 페이지 크기

#define TOTAL_MEMORY_SIZE 65536 // 64KB 전체 메모리 크기
#define TOTAL_FRAMES (TOTAL_MEMORY_SIZE / PAGE_SIZE) // 총 프레임 수
// #define OFFSET_MASK 0xFFF // 12비트 오프셋 마스크

// ================ Frame 관리 구조체 ================

typedef struct { 
    int frame_number;
    int is_allocated;
    size_t first_address;
} Frame; // 각 frame을 관리

typedef struct {
    Frame frames[TOTAL_FRAMES];
    unsigned char * mem;
} FrameManager; // 전체 frame을 관리

// status가 0인 frame들을 관리하는 양방향 연결 리스트
typedef struct FrameNode {
    Frame frame;
    struct FrameNode * prev;
    struct FrameNode * next;
} FrameNode;

typedef struct {
    FrameNode * head;
    FrameNode * tail;
} FrameList;

// ================ ================ ================

// ============ Process Pool 관리 구조체 ============

// 프로세스 정보를 담는 구조체
typedef struct Process {
    PageManager * page_manager;
    struct Process * next;
    char process_name[20];
} Process;

// 링크드 리스트 관리를 위한 구조체
typedef struct ProcessPool {
    Process* head;
} ProcessPool;

// ================ ================ ================

// ============ Process Page 관리 구조체 ============

typedef struct { // page struct
    unsigned int data[PAGE_SIZE];
    int page_number;
    int matched_frame; // 각 page와 frame matching된 값
    int is_matched_frame; // 각 page와 frame matching 여부 판단
} Page;

typedef struct { // page manager struct
    Page * pages; // dynamic allocation
    int allocated_pages; // 총 몇개의 페이지인지
    int is_memory_loaded; // is making page table finished?
} PageManager;

// ================ ================ ================

// ================ Function of Frame ================

FrameList * create_empty_frames_list() { // 비어 있는 frame들을 관리하는 양방향 연결 리스트 생성
    FrameList * empty_frames_list = (FrameList *)malloc(sizeof(FrameList));
    empty_frames_list->head = NULL;
    empty_frames_list->tail = NULL;

    return empty_frames_list;
}

// 프레임을 연결리스트에 다시 순서 맞춰서 넣기
void add_empty_frame_sorted(FrameList * empty_frames_list, Frame frame, FrameManager * fm) {
    FrameNode * new_node = (FrameNode *)malloc(sizeof(FrameNode));
    new_node -> frame = frame;
    new_node -> prev = NULL;
    new_node -> next = NULL;

    if (empty_frames_list -> head == NULL) {
        // List is empty
        empty_frames_list -> head = new_node;
        empty_frames_list -> tail = new_node;
    } else if (empty_frames_list -> head -> frame.frame_number >= frame.frame_number) {
        // Insert at the head
        new_node -> next = empty_frames_list -> head;
        empty_frames_list -> head -> prev = new_node;
        empty_frames_list -> head = new_node;
    } else {
        // Insert at the correct position
        FrameNode * current = empty_frames_list -> head;
        while (current -> next != NULL && current -> next -> frame.frame_number < frame.frame_number) {
            current = current->next;
        }
        new_node -> next = current -> next;
        if (current -> next != NULL) {
            new_node -> next -> prev = new_node;
        } else {
            empty_frames_list->tail = new_node;
        }
        current -> next = new_node;
        new_node -> prev = current;
    }

    fm -> frames[frame.frame_number].is_allocated = 0;
}

void print_empty_frames_list(FrameList * empty_frames_list) {
    FrameNode * current = empty_frames_list -> head;
    printf("\n   [ HEAD ] \n\n");
    while (current != NULL) {
        printf("   Frame Number: %3d    First address : %6zu \n", current -> frame.frame_number, current -> frame.first_address);
        current = current->next;
    }
    printf("\n   [ TAIL ] \n\n");
}

// 빈 프레임 연결 리스트 내의 노드 개수 세기
int count_empty_frames(FrameList * empty_frames_list) {
    int count = 0;
    FrameNode * current = empty_frames_list -> head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

// 빈 프레임 연결 리스트에서 맨 앞의 프레임 가져오기, 페이지 테이블 할당 될 때 사용
// 항상 먼저 연결 리스트 개수 확인한 후에 실행 (개수가 0 이면 에러 - 0일 때 그냥 함수 실행 안되게)
Frame get_first_empty_frame(FrameList * empty_frames_list, FrameManager * fm) {
    if (empty_frames_list -> head == NULL) {
        Frame error;
        error.frame_number = -1;
        error.is_allocated = -1;
        error.first_address = -1;
        printf("No empty frame ! \n");
        return error;
    }

    FrameNode * first_node = empty_frames_list -> head;
    Frame first_frame = first_node -> frame;

    fm -> frames[first_frame.frame_number].is_allocated = 1;

    if (empty_frames_list -> head == empty_frames_list -> tail) {
        // Only one node in the list
        empty_frames_list -> head = NULL;
        empty_frames_list -> tail = NULL;
    } else {
        empty_frames_list -> head = first_node -> next;
        empty_frames_list -> head -> prev = NULL;
    }

    free(first_node);

    return first_frame;
}

// 처음 Frame Mangager 생성 될 때 리스트 세팅
void first_frame_list_set(FrameList * empty_frames_list, Frame frame) {
    FrameNode * new_node = (FrameNode *)malloc(sizeof(FrameNode));
    new_node -> frame = frame;
    new_node -> prev = NULL;
    new_node -> next = NULL;

    if (empty_frames_list -> head == NULL) {
        // List is empty
        empty_frames_list -> head = new_node;
        empty_frames_list -> tail = new_node;
    } else {
        empty_frames_list -> tail -> next = new_node;
        new_node -> prev = empty_frames_list -> tail;
        empty_frames_list -> tail = new_node;
    }
}

// empty frames list 생성 후에 포인터 전달
FrameManager * create_frame_manager(unsigned char * dummy_physical_mem, FrameList * empty_frames_list) {
    // 전체 frame을 묶어서 관리하는 자료구조
    FrameManager * frame_manager = (FrameManager *)malloc(sizeof(FrameManager)); 
    frame_manager -> mem = dummy_physical_mem; // 할당받은 dummy 물리 메모리 공간을 연결

    size_t addr = 0;
    
    for (int i = 0; i < TOTAL_FRAMES; i++) {
        frame_manager -> frames[i].frame_number = i; // 정해지고 변하지 않음
        frame_manager -> frames[i].is_allocated = 0; // 계속 변하는 값
        frame_manager -> frames[i].first_address = addr; // 정해지고 변하지 않음
        addr += 4096;

        // 처음 frame manager가 setting 될 때, 모든 frame을 양방향 연결 리스트에 연결
        first_frame_list_set(empty_frames_list, frame_manager->frames[i]);
    }

    printf("Frame 관리 자료구조 세팅 완료 ...");

    return frame_manager;
}

void show_frame_status(FrameManager * frame_manager) {
    printf("%15s  %9s  %15s \n", "Frame Number", "Status", "First Address");
    printf("   ======================================== \n");
    for (int i = 0; i < TOTAL_FRAMES; i++) {
        printf("%13d  %9d  %17zu \n", 
        frame_manager->frames[i].frame_number, frame_manager->frames[i].is_allocated, frame_manager->frames[i].first_address);
    }
}

// 동적 할당 메모리 free 함수 모음

void free_frame_manager(FrameManager * frame_manager) {
    free(frame_manager);
}

void free_empty_frames_list(FrameList * empty_frames_list) {
    FrameNode * current = empty_frames_list -> head;
    while (current != NULL) {
        FrameNode * temp = current;
        current = current -> next;
        free(temp);
    }
    free(empty_frames_list);
}

// ================ ================ ================

// ============ Function of Process Pool ============

ProcessPool * CreateProcessPool(){// 프로세스 풀 생성
    ProcessPool * pool = (ProcessPool *)malloc(sizeof(ProcessPool));
    pool->head = NULL;

    return pool;
}

// 링크드 리스트에 프로세스 추가
void addProcess(ProcessPool* pool, PageManager* page_manager, char* name) {
    // 새로운 프로세스를 생성 (리스트의 Node 임)
    Process * newProcess = (Process*)malloc(sizeof(Process));
    newProcess->page_manager = page_manager;
    newProcess->next = NULL;
    strcpy(newProcess->process_name, name);

    // 리스트가 비어있다면 새로운 프로세스를 헤드로 지정
    if (pool->head == NULL) {
        pool->head = newProcess;
        return;
    }

    // 리스트의 끝을 찾아서 새로운 프로세스를 연결
    Process* current = pool->head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = newProcess;
}

// 링크드 리스트에서 특정 프로세스를 제거
void removeProcess(ProcessPool* pool, char* name) {
    Process* current = pool->head;
    Process* prev = NULL;

    // 헤드에서 시작하여 해당 Process name을 가진 프로세스 찾기
    while (current != NULL && current->process_name != name) {
        prev = current;
        current = current->next;
    }

    // 특정 Process name을 가진 프로세스를 찾지 못한 경우
    if (current == NULL) {
        printf("Process not found.\n");
        return;
    }

    // 특정 Process name을 가진 프로세스를 찾은 경우
    // 이전 노드와 다음 노드를 연결하고 해당 노드 메모리 해제
    if (prev == NULL) {
        // 삭제할 노드가 헤드인 경우
        pool->head = current->next;
    } else {
        // 삭제할 노드가 헤드가 아닌 경우
        prev->next = current->next;
    }
    free(current);
    printf("Process %s is removed from pool.\n", name);
}

// 링크드 리스트에 있는 모든 프로세스 정보 출력
void printProcesses(ProcessPool* pool) {
    Process* current = pool->head;
    while (current != NULL) {
        printf("process name: %s \n", current->process_name);
        current = current->next;
    }
}

// 메모리 해제를 위해 링크드 리스트의 모든 노드 삭제
void freeProcessPool(ProcessPool* pool) {
    Process* current = pool->head;
    while (current != NULL) {
        Process* temp = current;
        current = current->next;
        free(temp);
    }
    pool->head = NULL;
}

// ================ ================ ================

// ============ Function of Process Page ============

// 프로그램별 Page Manager 생성
PageManager * create_page_manager(int total_page_size) { // create page manager, page data는 비어있는 것으로 생성
    PageManager * page_manager = (PageManager *)malloc(sizeof(PageManager));
    if (page_manager == NULL) { // error
        fprintf(stderr, "Page Manager 할당 실패\n");
        return NULL;
    }
    // 프로그램이 필요한 페이지 갯수만큼 페이지 동적 할당
    page_manager -> pages = (Page *)malloc(total_page_size * sizeof(Page)); 

    if (page_manager -> pages == NULL) { // error
        fprintf(stderr, "Page Manager에 Page 할당 실패\n");
        free(page_manager); // 생성된 Page Manager도 없애버리고 return
        return NULL;
    }
    
    // 프로그램 Page Manager와 프로그램 Page Manager가 관리하는 page들 초기 세팅
    for (int i = 0; i < total_page_size; i++) {
        page_manager -> pages[i].page_number = i;
		page_manager -> pages[i].is_matched_frame = 0;
    }

    page_manager -> allocated_pages = total_page_size;
    page_manager -> is_memory_loaded = 0; // not loaded

    return page_manager;
}

// execute에서 프로그램 읽어온 것과 연결
void set_page_data(PageManager * page_manager, int total_bytes, unsigned int * byte) {
    int k = 0;

    for (int i = 0; i < page_manager -> allocated_pages; i++) {
        for (int j = 0; j < PAGE_SIZE; j++) {
            if(byte[k])
            page_manager -> pages[i].data[j] = byte[k];
            k++;
        }
    }
    printf("[ 총 %d byte, %d page로 분할 ] All pages setting 완료  \n", total_bytes, page_manager -> allocated_pages);
}

void remove_page_manager(PageManager * page_manager) { // remove page manager
    free(page_manager -> pages); // free pages
    free(page_manager); // free page manager
}

// print 함수 모음

void show_pf_table(PageManager *page_manager, FrameManager *frame_manager) {
    if (page_manager == NULL) {
        fprintf(stderr, "PageManager is NULL\n");
        return;
    }

    // matching 여부 파악해서 matching 됐으면 페이지 테이블 출력
    if (page_manager -> is_memory_loaded == 1) {
        
        printf("┌────────────┬────────────┐\n");
        printf("│   Page #   │   Frame #  │\n");
        printf("├────────────┼────────────┤\n");

        for (int i = 0; i < page_manager->allocated_pages; i++) {
            printf("│ %-10d │ %-10d │\n", 
                    page_manager->pages[i].page_number,
                    page_manager->pages[i].matched_frame
                    );
        }
        printf("└────────────┴────────────┘\n");
    }

    else {
        printf("페이지가 프레임과 매칭이 되지 않았습니다.\n");
    }
}

// ================ ================ ================
