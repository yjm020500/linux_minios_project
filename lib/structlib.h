#define PAGE_SIZE 4096 // 4KB 페이지 크기
#define OFFSET_MASK 0xFFF // 12비트 오프셋

#define TOTAL_MEMORY_SIZE 65536 // 64KB 전체 메모리 크기
#define TOTAL_FRAMES (TOTAL_MEMORY_SIZE / PAGE_SIZE) // 총 프레임 수

// program_struct.c 에 있는 구조체들 및 함수들




// frame_struct.c 에 있는 구조체 및 함수들

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
//

FrameList * create_empty_frames_list();
void add_empty_frame_sorted(FrameList * empty_frames_list, Frame frame, FrameManager * fm);
void print_empty_frames_list(FrameList * empty_frames_list); 
int count_empty_frames(FrameList * empty_frames_list);
Frame get_first_empty_frame(FrameList * empty_frames_list, FrameManager * fm);
Frame remove_node_at_position(FrameList * empty_frames_list, int position);


FrameManager * create_frame_manager(unsigned char * dummy_physical_mem, FrameList * empty_frames_list);
void free_frame_manager(FrameManager * frame_manager);
void show_frame_status(FrameManager * frame_manager);
void first_frame_list_set(FrameList * empty_frames_list, Frame frame);


void free_frame_manager(FrameManager * frame_manager);
void free_empty_frames_list(FrameList * empty_frames_list);


