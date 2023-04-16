/* circularQ.c
 *
 *  Data Structures, Homework #5
 *  School of Computer Science at Chungbuk National University
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4 // 큐의 최대 크기는 4로 고정 (원형 큐이므로, 실질적으로 저장 가능한 데이터는 3개임)

typedef char element; // char를 element라는 별명으로 부름
typedef struct {
	element queue[MAX_QUEUE_SIZE]; // 4 크기의 (element)char형 배열 변수 queue(실질적으로 데이터를 저장하는 곳)
	int front, rear; // front와 rear을 나타내주는 int형 변수
}QueueType; // 큐 구조체를 QueueType이라는 별명 붙임


QueueType *createQueue(); // QueueType을 구성해서 반환하는 함수
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
		scanf(" %c", &command);

		switch(command) {
		case 'i': case 'I':
			data = getElement();
			enQueue(cQ, data);
			break;
		case 'd': case 'D':
			deQueue(cQ, &data);
			break;
		case 'p': case 'P':
			printQ(cQ);
			break;
		case 'b': case 'B':
			debugQ(cQ);
			break;
		case 'q': case 'Q':
   	        freeQueue(cQ);
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');


	return 1;
}

QueueType *createQueue() // QueueType 구조체를 힙에 동적할당한 후, 그를 가리키는 포인터 리턴
{
	QueueType *cQ;
	int i;
	cQ = (QueueType *)malloc(sizeof(QueueType));
	cQ->front = 0;
	cQ->rear = 0; // 처음의 front와 rear은 둘다 0으로 초기화
	
	// 큐에 내용 삽입 전, debug시 초기화가 안 되서 이상한 값이 들어있는 것을 제거하기 위한 절차 추가
	for(i = 0; i < MAX_QUEUE_SIZE; i++)
	{
		cQ->queue[i] = '\0';
	}
	return cQ;
}

int freeQueue(QueueType *cQ)
{
    if(cQ == NULL) return 1;
    free(cQ);
    return 1;
}

element getElement()
{
	element item;
	printf("Input element = ");
	scanf(" %c", &item);
	return item;
}


/* complete the function */
int isEmpty(QueueType *cQ)
{
	if(cQ->front == cQ->rear)
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
{
	if(cQ->front == (cQ->rear+1)%MAX_QUEUE_SIZE)
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
{
	if(!isFull(cQ))
	{
		cQ->rear = (cQ->rear+1)%MAX_QUEUE_SIZE; // rear는 직접 마지막 아이템을 가르키고, front는 그 전의 빈 위치를 가르킴
		cQ->queue[cQ->rear] = item;
	}
	else
	{
		return 0;
	}
}

/* complete the function */
void deQueue(QueueType *cQ, element *item)
{
    if(!isEmpty(cQ))
	{
		*item = cQ->queue[(cQ->front+1)%MAX_QUEUE_SIZE];
		// cQ->queue[(cQ->front+1)%MAX_QUEUE_SIZE] = '\0'; 	// debug시 값이 그대로 남는 것을 제거하기 위한 절차 추가
		cQ->front = (cQ->front+1)%MAX_QUEUE_SIZE;
	}
	else
	{
		return 0;
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


void debugQ(QueueType *cQ) // 실제 가지고있는 원소를 포함하여, 빈공간을 포함하여 front와 tail의 위치, 그리고 큐 전체의 상태를 확인할 수 있는 함수
{

	printf("\n---DEBUG\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++)
	{
		if(i == cQ->front) {
			printf("  [%d] = front\n", i);
			continue;
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]);

	}
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear);
}

