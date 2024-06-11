#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define N 10 //10 processes
#define TQ 3 //The size of the time quantum

struct Process{
    int id; //process id
    int burst_time; //process burst_time
    int remain_time; //remain_time
    bool is_completed; //completion status
};

void initialize();
void round_robin_scheduling();
bool is_all_completed();

struct Process processes[N];

int main() {  //main
    while(1){
        int num;
        printf("if you want to start, enter 1\n");
        scanf("%d", &num);
        if(num == 1) {break;}
    }
   
    srand(time(NULL));
    initialize();
   
    printf("\n");
   
    round_robin_scheduling();
   
    return 0;
}

void initialize(){ //initialization
    for(int i=0;i<N;i++){
        processes[i].id = i;
        processes[i].burst_time = rand()%10+1; //random burst time
        processes[i].remain_time = processes[i].burst_time;
        processes[i].is_completed = false;
        printf("process %d, burst_time is %d \n", i, processes[i].burst_time);
    }
}

void round_robin_scheduling() { //round_robing_scheduling
    while(true){
        for(int i=0;i<N;i++){
            if(processes[i].remain_time > 0){ //not completed process
                if(processes[i].remain_time > TQ){ //more than TQ
                    processes[i].remain_time = processes[i].remain_time - TQ;
                    printf("process %d processed, remain_time is %d, turnaround_time is %d \n", i, processes[i].remain_time, TQ);
                }
                else{ //equal and less than TQ
                    int remain = processes[i].remain_time;
                    processes[i].remain_time = 0;
                    processes[i].is_completed = true;
                    printf("process %d processed, remain_time is 0 that is, processing completed, turnaround_time is %d \n", i, remain);
                }
            }
        }
       
        if(is_all_completed()){ //test all completed
            printf("all processes completed!!\n");
            break;
        }
    }
}

bool is_all_completed(){  //test all completed
    int count=0;
    for(int i = 0;i<N;i++){
        if(processes[i].is_completed == true){
            count++;
        }
    }
    if(count == 10){
        return 1;
    }
    else {
        return 0;
    }
}
