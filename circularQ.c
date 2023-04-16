/* circularQ.c
 *
 *  Data Structures, Homework #5
 *  School of Computer Science at Chungbuk National University
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4 // ť�� �ִ� ũ��� 4�� ���� (���� ť�̹Ƿ�, ���������� ���� ������ �����ʹ� 3����)

typedef char element; // char�� element��� �������� �θ�
typedef struct {
	element queue[MAX_QUEUE_SIZE]; // 4 ũ���� (element)char�� �迭 ���� queue(���������� �����͸� �����ϴ� ��)
	int front, rear; // front�� rear�� ��Ÿ���ִ� int�� ����
}QueueType; // ť ����ü�� QueueType�̶�� ���� ����


QueueType *createQueue(); // QueueType�� �����ؼ� ��ȯ�ϴ� �Լ�
int freeQueue(QueueType *cQ); // QueueType�� free�ϴ� �Լ�
int isEmpty(QueueType *cQ); // QueueType�� ������� üũ�ϴ� �Լ�
int isFull(QueueType *cQ); // QueueType�� �� á���� üũ�ϴ� �Լ�
void enQueue(QueueType *cQ, element item); // ���� �ִ� �Լ�
void deQueue(QueueType *cQ, element* item); // ���� ���� �Լ�
void printQ(QueueType *cQ); // ť ���� ��� �Լ�
void debugQ(QueueType *cQ); // ť�� ���Ͽ� ��� �۵��ϴ��� Ȯ���ϱ� ���� �Լ�
element getElement(); // ������ ���Ҹ� �Է¹޴� �Լ�

int main(void)
{
	QueueType *cQ = createQueue(); // QueueType�� ����Ű�� QueueType�� ������ ���� cQ�� �����ϰ�, createQueue�� ���� ���� �����Ҵ��� �ּҸ� ��������
	element data;
	char command;

	do{
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");
		printf("[--------------  [�ֻ�]  	[2022041062]  --------------]\n");

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

QueueType *createQueue() // QueueType ����ü�� ���� �����Ҵ��� ��, �׸� ����Ű�� ������ ����
{
	QueueType *cQ;
	int i;
	cQ = (QueueType *)malloc(sizeof(QueueType));
	cQ->front = 0;
	cQ->rear = 0; // ó���� front�� rear�� �Ѵ� 0���� �ʱ�ȭ
	
	// ť�� ���� ���� ��, debug�� �ʱ�ȭ�� �� �Ǽ� �̻��� ���� ����ִ� ���� �����ϱ� ���� ���� �߰�
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
		cQ->rear = (cQ->rear+1)%MAX_QUEUE_SIZE; // rear�� ���� ������ �������� ����Ű��, front�� �� ���� �� ��ġ�� ����Ŵ
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
		// cQ->queue[(cQ->front+1)%MAX_QUEUE_SIZE] = '\0'; 	// debug�� ���� �״�� ���� ���� �����ϱ� ���� ���� �߰�
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

	first = (cQ->front + 1)%MAX_QUEUE_SIZE; // �������� �����Ͱ� ����ִ� �ε��� first
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE; // �������� ������ �����Ͱ� ����ִ� �ε����� ���� �ε����� ��ĭ�� ����Ű�� last

	printf("Circular Queue : [");

	i = first;
	while(i != last){ // ���� ť�� ���� ��� �����͸� ����ϴ� while��
		printf("%3c", cQ->queue[i]);
		i = (i+1)%MAX_QUEUE_SIZE;

	}
	printf(" ]\n");
}


void debugQ(QueueType *cQ) // ���� �������ִ� ���Ҹ� �����Ͽ�, ������� �����Ͽ� front�� tail�� ��ġ, �׸��� ť ��ü�� ���¸� Ȯ���� �� �ִ� �Լ�
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

