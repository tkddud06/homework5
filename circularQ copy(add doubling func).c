/* circularQ.c
 *
 *  Data Structures, Homework #5
 *  School of Computer Science at Chungbuk National University
 */
// �䱸������ ������Ų ��, ����� ���� �����ϴٴ� ������ �� Full ��Ȳ������ ���� ��ɵ� dequeue�� ��Ȱ���ؼ� �����ϰ� �־�ý��ϴ�.
#include <stdio.h>
#include <stdlib.h>

int MAX_QUEUE_SIZE = 4; // ť�� �⺻ ũ��� 4�� ���� (���� ť�̹Ƿ�, ���������� ���� ������ �����ʹ� 3����. �ʿ�� ������ ���� ���� ������ ����)

typedef char element; // char�� element��� �������� �θ�
typedef struct {
	element* queue; // (element)char�� ������ ���� queue(���������� �����͸� �����ϴ� ���� ����Ű���� �����Ҵ��� ����, ������ ���� ������)
	int front, rear; // front�� rear�� ��Ÿ���ִ� int�� ����
}QueueType; // ť ����ü�� QueueType�̶�� ���� ����


QueueType *createQueue(); // QueueType�� �����ؼ� ��ȯ�ϴ� �Լ�
QueueType *queue_double(QueueType *cQ); // ������ �����ϴ� �Լ�
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

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'i': case 'I':
			data = getElement();
			if(isFull(cQ)) // ���� �� á�µ� ������ �Ϸ� �ϸ�, ������ ������
			{
				cQ = queue_double(cQ); 
			}
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

	printf("[-----  [�ֻ�]    [2022041062]  -----]");

	return 1;
}

QueueType *createQueue() // QueueType ����ü�� ���� �����Ҵ��� ��, �׸� ����Ű�� ������ ����
{
	QueueType *cQ;
	int i;
	cQ = (QueueType *)malloc(sizeof(QueueType));
	cQ->front = 0;
	cQ->rear = 0; // ó���� front�� rear�� �Ѵ� 0���� �ʱ�ȭ
	cQ->queue = (element*)malloc(sizeof(element) * MAX_QUEUE_SIZE);
	// ť�� ���� ���� ��, debug�� �ʱ�ȭ�� �� �Ǽ� �̻��� ���� ����ִ� ���� �����ϱ� ���� ���� �߰�
	for(i = 0; i < MAX_QUEUE_SIZE; i++)
	{
		cQ->queue[i] = '\0';
	}
	return cQ;
}

QueueType *queue_double(QueueType *cQ)
{
	QueueType *re_cQ;
	int i;
	char item;
	int re_max_queue_size = 2 * MAX_QUEUE_SIZE; // ���Ӱ� ���� re_CQ�� ������
	re_cQ = (QueueType *)malloc(sizeof(QueueType));
	re_cQ->front = 0;
	re_cQ->rear = MAX_QUEUE_SIZE-1; // front�� rear�� ��ġ ������
	re_cQ->queue = (element*)malloc(sizeof(element) * re_max_queue_size);

	// ���� ���� �ű��, debug�� �̻��� �� ����ִ� ���� �����ϱ� ���� ���� ����
	re_cQ->queue[0] = '\0'; // front �κ�

	for(i = 1; i < MAX_QUEUE_SIZE; i++)
	{
		deQueue(cQ, &item); // deQueue�� Ȱ���Ͽ�, ���� ť�� front���� rear���� ���ʴ�� �����Ͽ� ����
		re_cQ->queue[i] = item;
	}

	for(i = re_cQ->rear+1; i < re_max_queue_size; i++) // debug�� �̻��� �� ����ִ� ���� �����ϱ� ���� ���� ����
	{
		re_cQ->queue[i] = '\0';
	}

	MAX_QUEUE_SIZE = re_max_queue_size; // ���������� ������

	freeQueue(cQ); // ���� cQ�� free
	return re_cQ; // ���ο� re_cQ�� ��ȯ�ϸ�, ������ cQ ������ ������ ���� re_cQ�� ����Ű�� �� ����.
}

int freeQueue(QueueType *cQ)
{
    if(cQ == NULL) return 1;
	free(cQ->queue);
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
	cQ->rear = (cQ->rear+1)%MAX_QUEUE_SIZE; // rear�� ���� ������ �������� ����Ű��, front�� �� ���� �� ��ġ�� ����Ŵ
	cQ->queue[cQ->rear] = item;
}

/* complete the function */
void deQueue(QueueType *cQ, element *item)
{
    if(!isEmpty(cQ))
	{
		*item = cQ->queue[(cQ->front+1)%MAX_QUEUE_SIZE];
		cQ->queue[(cQ->front+1)%MAX_QUEUE_SIZE] = '\0'; 	// debug�� ���� �״�� ���� ���� �����ϱ� ���� ���� �߰� (�׷��� ������ ������ ������ ����ʹ� ������ ���̰� �ֽ��ϴ�.)
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


void debugQ(QueueType *cQ) // ���� �������ִ� ���ҿ� �����ϰ�, ������� �����Ͽ� front�� tail�� ��ġ, �׸��� ť ��ü�� ���¸� Ȯ���� �� �ִ� �Լ�
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

