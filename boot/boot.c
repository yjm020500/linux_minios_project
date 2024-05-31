#include <stdio.h>
#include <stdlib.h>

int main() {
    // tmux 세션을 생성 (백그라운드에서) 마지막에 attach
    
    int result = system("tmux new -d -s terminal");
    if (result == -1) {
        perror("system");
        return 1;
    }

    result = system("tmux split-window -h -t terminal"); // 좌 우로 쪼개기
    if (result == -1) {
        perror("system");
        return 1;
    }

    result = system("tmux split-window -h -t terminal");
    if (result == -1) {
        perror("system");
        return 1;
    }

    result = system("tmux split-window -v -t terminal"); // 상 하로 쪼개기
    if (result == -1) {
        perror("system");
        return 1;
    }

    result = system("tmux select-pane -t 2");
    if (result == -1) {
        perror("system");
        return 1;
    }

    result = system("tmux split-window -v -t terminal");
    if (result == -1) {
        perror("system");
        return 1;
    }

    result = system("tmux select-pane -t 4");
    if (result == -1) {
        perror("system");
        return 1;
    }

    result = system("tmux split-window -v -t terminal");
    if (result == -1) {
        perror("system");
        return 1;
    }

    result = system("tmux select-pane -t 0");
    if (result == -1) {
        perror("system");
        return 1;
    }

    result = system("tmux attach -t terminal");
    if (result == -1) {
        perror("system");
        return 1;
    }   

    return 0;
}