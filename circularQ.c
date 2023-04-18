/* circularQ.c
 *
 *  Data Structures, Homework #5
 *  School of Computer Science at Chungbuk National University
 */
// 기존 코드서 부족한 부분 수정 : 요구사항을 만족시킨 후, 기능이 조금 부족하다는 생각이 들어서 Full 상황에서의 더블링 기능도 dequeue를 활용해서 넣어봤습니다.
#include <stdio.h>
#include <stdlib.h>

int MAX_QUEUE_SIZE = 4; // 큐의 기본 크기는 4로 설정 
// (원형 큐이므로, Full과 empty 상태 구분을 위해프론트 자리를 비워둬서 실질적으로 저장 가능한 데이터는 3개임.)
// 필요시 더블링을 위해 전역 변수로 선언

typedef char element; // char를 element라는 별명으로 부름
typedef struct {
	element* queue; // (element)char형 포인터 변수 queue(실질적으로 데이터를 저장하는 곳을 가리키도록 동적할당할 예정, 더블링을 위해 변경함)
	int front, rear; // front와 rear을 나타내주는 int형 변수
}QueueType; // 큐 구조체를 QueueType이라는 별명 붙임


QueueType *createQueue(); // QueueType을 구성해서 반환하는 함수
QueueType *queue_double(QueueType *cQ); // 더블링을 진행하는 함수
int freeQueue(QueueType *cQ); // QueueType을 free하는 함수
int isEmpty(QueueType *cQ); // QueueType이 비었는지 체크하는 함수
int isFull(QueueType *cQ); // QueueType이 다 찼는지 체크하는 함수
void enQueue(QueueType *cQ, element item); // 원소 넣는 함수
void deQueue(QueueType *cQ, element* item); // 원소 빼는 함수
void printQ(QueueType *cQ); // 큐 내용 출력 함수
void debugQ(QueueType *cQ); // 큐에 대하여 어떻게 작동하는지 확인하기 위한 함수
element getElement(); // 삽입할 원소를 입력받는 함수

int main(void)
{
	QueueType *cQ = createQueue(); // QueueType을 가리키는 QueueType형 포인터 변수 cQ를 선언하고, createQueue를 통해 힙에 동적할당한 주소를 대입해줌
	element data;
	char command;

	do{
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");
		printf("[--------------  [최상영]  	[2022041062]  --------------]\n");

		printf("Command = ");
		scanf(" %c", &command); // command 변수 입력받기

		switch(command) { // 입력받은 command 변수로 실행할 기능 구분 switch
		case 'i': case 'I':
			data = getElement();
			if(isFull(cQ)) // 만약 꽉 찼는데 삽입을 하려 하면, 더블링을 진행함
			{
				cQ = queue_double(cQ); 
			}
			enQueue(cQ, data); // 이후 enqueue
			break;
		case 'd': case 'D':
			deQueue(cQ, &data); // dequeue
			break;
		case 'p': case 'P':
			printQ(cQ); // 큐 프린트
			break;
		case 'b': case 'B':
			debugQ(cQ); // 큐 디버그
			break;
		case 'q': case 'Q':
   	        freeQueue(cQ); // 큐 프리
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n"); // 이외 고려하지 않은 상황시 Concentration!! 출력
			break;
		}

	}while(command != 'q' && command != 'Q'); // q 입력받을때까지 계속 반복


	return 0; // 리턴 1이였는데, 정상종료를 알리기 위해 리턴 0으로 변경
}

QueueType *createQueue() // QueueType 구조체를 힙에 동적할당한 후, 그를 가리키는 포인터 리턴
{
	QueueType *cQ;
	int i;
	cQ = (QueueType *)malloc(sizeof(QueueType));
	cQ->front = 0;
	cQ->rear = 0; // 처음의 front와 rear은 둘다 0으로 초기화
	cQ->queue = (element*)malloc(sizeof(element) * MAX_QUEUE_SIZE); // 필요시 더블링을 위해, queue는 MAX_QUEUE_SIZE만큼 동적할당으로 구현

	// 큐에 내용 삽입 전, debug시 초기화가 안 되서 이상한 값이 들어있는 것을 제거하기 위한 절차 추가
	// 추가하지 않으면, 아무 내용도 삽입하지 않았어도 / ( 등의 이상한 값이 출력됐음.
	for(i = 0; i < MAX_QUEUE_SIZE; i++)
	{
		cQ->queue[i] = '\0'; // 초기화
	}

	return cQ;
}

QueueType *queue_double(QueueType *cQ)
{
	QueueType *re_cQ; // 새로운 큐를 만들기 위한 Queuetype 구조체 포인터 선언
	int i;
	char item;
	int re_max_queue_size = 2 * MAX_QUEUE_SIZE; // 새롭게 만들 re_CQ의 큐 사이즈
	re_cQ = (QueueType *)malloc(sizeof(QueueType));
	re_cQ->front = 0;
	re_cQ->rear = MAX_QUEUE_SIZE-1; // front와 rear의 위치 재조정
	re_cQ->queue = (element*)malloc(sizeof(element) * re_max_queue_size); // 새로운 큐 공간 동적할당

	// 아래 코드들은 기존 내용 옮기고, debug시 이상한 값 들어있는 것을 제거하기 위한 절차 수행
	re_cQ->queue[0] = '\0'; // front 부분 초기화

	for(i = 1; i < MAX_QUEUE_SIZE; i++)
	{
		deQueue(cQ, &item); // deQueue를 활용하여, 이전 큐의 모든 데이터 차례대로 추출하여 이동
		re_cQ->queue[i] = item;
	}

	for(i = re_cQ->rear+1; i < re_max_queue_size; i++)
	{
		re_cQ->queue[i] = '\0'; // 초기화
	}

	MAX_QUEUE_SIZE = re_max_queue_size; // 최대 크기를 나타내는 전역변수를 새로운 큐의 MAX 크기로 변경함

	freeQueue(cQ); // 기존 cQ는 free
	return re_cQ; // 새로운 re_cQ를 반환하면, 메인의 cQ 포인터 변수는 이제 re_cQ를 가리키게 될 것임.
}

int freeQueue(QueueType *cQ) // QueueType을 free하는 함수
{
    if(cQ == NULL) return 1; // 이미 프리되어있다면 종료
	free(cQ->queue);
    free(cQ); // 큐타입과, 큐타입 안에 속한 큐 모두 따로 동적할당 된 것이므로, 완벽한 프리를 위해선 각각 프리해줘야 함.
    return 1;
}

element getElement() // 삽입할 데이터를 입력받는 함수
{
	element item;
	printf("Input element = ");
	scanf(" %c", &item);
	return item;
}


/* complete the function */
int isEmpty(QueueType *cQ)
{ // 비어있다면 1, 그렇지 않다면 0을 리턴
	if(cQ->front == cQ->rear) // front와 rear가 일치하는 상황이 비어있는 상황
	{
		return 1;
	}
	else
	{
    	return 0;
	}
}

/* complete the function */
int isFull(QueueType *cQ)
{ // 꽉 찼다면 1, 그렇지 않다면 0을 리턴
	if(cQ->front == (cQ->rear+1)%MAX_QUEUE_SIZE) // 리어가 프론트 바로 앞에 있는 경우가 꽉 찬 상황
	{
		return 1;
	}
	else
	{
    	return 0;
	}
}


/* complete the function */
void enQueue(QueueType *cQ, element item)
{ // rear는 직접 들어있는 마지막 아이템(데이터)를 가르키고, front는 그 전의 빈 위치를 가르킴. 그를 고려하면 
// 데이터 삽입을 위해 아래와 같은 명령줄 필요함.
	cQ->rear = (cQ->rear+1)%MAX_QUEUE_SIZE; 
	cQ->queue[cQ->rear] = item;
}

/* complete the function */
void deQueue(QueueType *cQ, element *item)
{
    if(!isEmpty(cQ)) // 비어있지 않은 경우에만 dequeue가 가능
	{
		// rear는 직접 들어있는 마지막 아이템(데이터)를 가르키고, front는 그 전의 빈 위치를 가르킴. 그를 고려하면 
		// 데이터 삽입을 위해 아래와 같은 명령줄 필요함.
		*item = cQ->queue[(cQ->front+1)%MAX_QUEUE_SIZE];
		// cQ->queue[(cQ->front+1)%MAX_QUEUE_SIZE] = '\0'; 	// debug시 값이 그대로 남는 것을 제거하기 위한 절차 추가 (교수님 영상에서는 일부러 두신 것 같아, 주석처리했습니다.)
		cQ->front = (cQ->front+1)%MAX_QUEUE_SIZE;
	}
	else
	{
		return;
	}
}


void printQ(QueueType *cQ)
{
	int i, first, last;

	first = (cQ->front + 1)%MAX_QUEUE_SIZE; // 실질적인 데이터가 들어있는 인덱스 first
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE; // 실질적인 마지막 데이터가 들어있는 인덱스의 다음 인덱스인 빈칸을 가르키는 last

	printf("Circular Queue : [");

	i = first;
	while(i != last){ // 현재 큐가 가진 모든 데이터를 출력하는 while문 
		printf("%3c", cQ->queue[i]);
		i = (i+1)%MAX_QUEUE_SIZE;

	}
	printf(" ]\n");
}


void debugQ(QueueType *cQ) // 실제 가지고있는 데이터, 빈공간을 포함하여 front와 tail의 위치, 그리고 큐 전체의 상태를 확인할 수 있는 함수
{

	printf("\n---DEBUG\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++) // 큐 사이즈만큼 돌게 해둠
	{
		if(i == cQ->front) { // 프론트면 프론트라 출력
			printf("  [%d] = front\n", i);
			continue;
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]); // 나머지는 그냥 출력

	}
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear); // 프론트와 리어 위치 출력
}

