#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "system.h"

#define PAGE_SIZE 4096 // 4KB 페이지 크기
#define OFFSET_MASK 0xFFF // 12비트 오프셋 마스크

typedef struct{ //page struct
    unsigned char data[PAGE_SIZE];
    int page_number;
} Page;

typedef struct{ //page manager struct
    Page pages[TOTAL_PAGES]; //number of max page allocation // ToTAL_PAgses 프로그램 읽어왔을 때 바이트크기/4096
    int allocated_total_pages; //allocated pages
    int current_total_pages;
    // 전체 페이지 갯수 0 ~ 4 라고 하면
    // 할당 되어 있느 페이지 갯수 -> Queue  기능에서 모니터링 할 때
    int memory_load = 0 ,1 // 메모리 적재가 다 됨 trigger
} PageManager;


pool 에 첫번째로 들어가

fl 에서 가용가능한 프레임 갯수를 확인한다음에


===========
page num     frame
0       |  13   40000 44960

1       |  2

2       |  8

3       |  4

4       |  x
==========
return


추가 함수를 만들어서
fl을 인자로 받고
frame 칸에다 하나씩 채워

virtual_physical_memory로 접근을 해서

void * memory_set(void * ptr, int value, size_t num) {
    unsigned char * p = (unsigned char *)ptr;
    while (num--) {
        *p++ = (unsigned char)value;
    }
    return ptr;
}
40000 44960
for문을 돌려  0 4095
virtual_physical_memory++ = 페이지 한 오프셋 값이 메모리 번지 하나에 계속 채워지게


신호 보내서 그 분할 화면에 프로그램 띄우기
if memory_load == 1{
    "tmux send-keys -t %s.%s 'cat %s' C-m"
}



===========
page num     frame
0       |

1       |

2       |

3       |

4       |
==========


===========
page num     frame
0       |

1       |

2       |

3       |

4       |
==========


===========
page num     frame
0       |

1       |

2       |

3       |

4       |
==========


===================================
===========
page num     frame
0       |  x

1       |  x

2       |  x

3       |  x

4       |  x
==========
===================================