#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void minisystem()
{
    printf("minisystem\n");
}

void up_down_game() {
  srand(time(NULL));

  int randomNumber = (rand() % 20) + 1;
  int num;
  
  while(1) {
    printf("숫자를 입력하세요(1 ~ 20) : ");
    scanf("%d", &num);

    if (num < 1 || num > 20) {
      printf("1 ~ 20 사이의 숫자를 입력하세요! \n");
      continue;
    }
    else if (randomNumber == num) {
      printf("정답입니다! \n");
      break;
    }
    else if (randomNumber < num) {
      printf("Down! \n");
    }
    else {
      printf("Up! \n");
    }
  }
}
