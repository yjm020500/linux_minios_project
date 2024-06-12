# Team 1 OS Project

2024년도 1학기 운영체제 강의의 프로젝트입니다. 운영체제 강의를 수강하며, 강의 초반부에 프로세스와 스케줄링 관련한 강의와 학습은 오랜 시간 진행하였지만 메모리 관리 내용에 관해서는 과제 수행 및 추가 학습을 깊게 하지 못하여 본 프로젝트를 진행하였습니다.

<br/>

> 메모리 [ Paging ] 개념을 구현하고자 하였으며 paging 구현을 위해 필요한 자료구조와 이를 제어하는 함수, 알고리즘을 제작하였습니다.

<br/>

> 본 프로젝트는 메모리 관리 기능을 GUI(Graphic User Interface)를 통해 시각적으로 제공함으로써, 사용자들이 보다 쉽게 시스템의 메모리 상태를 이해하고 관리할 수 있도록 하는 것을 목표로 합니다.

# Simulator Flow

minios 프로그램 상에서 64KB 메모리 공간을 할당 받아 이를 Paging 기법 구현을 위한 (Dummy) Physical Memory로 사용을 하도록 구현하였습니다. Page와 Frame의 크기는 4KB 단위 입니다.

minios 실행시 Frame Manager, (Available) Frame List, Process Pool, Waiting Queue를 생성하여 minios 종료시까지 본 프로그램에서 다루어지는 모든 자료구조, 구조체, 함수, 데이터를 유지 · 관리 · 보수 합니다.

execute 커맨드를 통해 실행하고자 하는 프로그램을 읽어 Page 단위로 나누고 Page Manager를 세팅 (Page table을 만드는 과정이라고 보아도 됩니다.) 하여 메모리에 적재하면 Program Monitor에서 이 프로그램이 실행되는 모습을 확인할 수 있습니다.

<br/>

**프로그램이 실행 되지 않는 경우 (Program Monitor에 올라가지 않을 때)**

- Program Monitor에 공간(pane)이 부족한 경우

- 할당 가능한 frame의 수가 부족할 때 → 대기큐로 들어감
  <br/>

프로세스 풀을 통해 현재 메모리에 적재된 프로세스들을 확인할 수 있습니다.

terminate 커맨드를 통해 현재 실행중인 프로그램을 종료시킬 수 있습니다. 프로세스 풀에서 종료하고자 하는 프로그램을 제거 하고 할당 되었던 메모리 영역을 반환한 뒤 메모리 공간의 값을 0x00 으로 초기화 합니다. Program Monitor에서도 제거됩니다. (pane 공간을 반환) 모든 작업 수행 후 waiting Queue를 한바퀴 순회하며 할당 가능한 프로세스들 (Page manager가 모두 세팅된 상태로 Queue에 전달되기 때문에 바로 할당 하는 로직만 수행해 주면 됩니다.) 을 모두 메모리에 적재 합니다. 이 때도 적재 불가능한 프로그램은 다시 waiting Queue에 enqueue 합니다.

### Simulator Command |

- **help** - 사용 가능항 명령어 리스트를 볼 수 있습니다.

- **show_m** - 메모리 VIEW에서 볼 수 있는 매모리 그림을 업데이트 합니다.

  → 첫번째 인자로 몇번지 부터 볼 것인지 입력 받고, 두번째 인자로 몇번지 까지 볼 것인지 입력 받음

- **show_f** - Frame Manager를 참조하여, 모든 frame의 번호, 상태(할당), 프레임의 첫주소를 볼 수 있습니다.

- **show-efl** - 가용 가능 프레임 리스트를 볼 수 있습니다.

- **show_pp** - Process Pool을 참조하여, 현재 메모리에 적재된 프로그램을 볼 수 있습니다.

- **execute** - 실행하고자 하는 프로그램을 입력받아 메모리에 적재합니다.

  → 인자로 프로그램 이름을 입력 받음

- **terminate** - 실행 중인 프로그램을 종료 시킵니다.

  → 인자로 프로그램 이름을 입력 받음

### Program Bundle |

시뮬레이터에서 기본적으로 제공하는 프로그램 번들입니다!

execute와 terminate 명령어 입력 후 입력받는 인자에서 다음과 같은 프로그램명을 이용할 수 있습니다.

- add
- calculator
- estimate_pi
- get_cpu_usage
- get_disk_usage
- get_mem_usage
- helloworld
- howru
- RR_scheduling
- up_down_game

## Memory Management Simulator 실행 방법

0. tmux가 설치되어 있어야 합니다.

   → Linux : sudo apt install tmux

   → MacOS : brew install tmux (MacOS 용 패키지 관리자 homebrew가 필요합니다.)

1. Terminal을 실행합니다.

2. `https://github.com/K-Hwoo/os_project.git` 를 입력하여 git을 클론 합니다.

3. `cd os_project` 를 입력하여 폴더를 이동 합니다.

4. `make`를 입력하여 프로그램(코드)를 빌드합니다.

5. `./powerOn` 을 입력하여 부팅 프로그램을 실행합니다.

   → 맨 왼쪽의 Terminal 영역에서 `./minios`의 입력을 대기합니다.

<br/>

<img src="https://github.com/K-Hwoo/os_project/assets/91537166/1659d564-a605-4b5d-ba05-9fc7c0577e37" width="700"  height="450">

<br/>
<br/>
6.  ./minios 를 입력하여 minios 프로그램을 실행 합니다.

<br/>
<br/>
<img src="https://github.com/K-Hwoo/os_project/assets/91537166/b5f0b0f4-7eab-4297-b669-71a64af37451" width="700"  height="450">
