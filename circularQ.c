/* circularQ.c
 *
 *  Data Structures, Homework #5
 *  School of Computer Science at Chungbuk National University
 */
// ���� �ڵ弭 ������ �κ� ���� : �䱸������ ������Ų ��, ����� ���� �����ϴٴ� ������ �� Full ��Ȳ������ ���� ��ɵ� dequeue�� Ȱ���ؼ� �־�ý��ϴ�.
#include <stdio.h>
#include <stdlib.h>

int MAX_QUEUE_SIZE = 4; // ť�� �⺻ ũ��� 4�� ���� 
// (���� ť�̹Ƿ�, Full�� empty ���� ������ ��������Ʈ �ڸ��� ����ּ� ���������� ���� ������ �����ʹ� 3����.)
// �ʿ�� ������ ���� ���� ������ ����

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
		printf("[--------------  [�ֻ�]  	[2022041062]  --------------]\n");

		printf("Command = ");
		scanf(" %c", &command); // command ���� �Է¹ޱ�

		switch(command) { // �Է¹��� command ������ ������ ��� ���� switch
		case 'i': case 'I':
			data = getElement();
			if(isFull(cQ)) // ���� �� á�µ� ������ �Ϸ� �ϸ�, ������ ������
			{
				cQ = queue_double(cQ); 
			}
			enQueue(cQ, data); // ���� enqueue
			break;
		case 'd': case 'D':
			deQueue(cQ, &data); // dequeue
			break;
		case 'p': case 'P':
			printQ(cQ); // ť ����Ʈ
			break;
		case 'b': case 'B':
			debugQ(cQ); // ť �����
			break;
		case 'q': case 'Q':
   	        freeQueue(cQ); // ť ����
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n"); // �̿� ������� ���� ��Ȳ�� Concentration!! ���
			break;
		}

	}while(command != 'q' && command != 'Q'); // q �Է¹��������� ��� �ݺ�


	return 0; // ���� 1�̿��µ�, �������Ḧ �˸��� ���� ���� 0���� ����
}

QueueType *createQueue() // QueueType ����ü�� ���� �����Ҵ��� ��, �׸� ����Ű�� ������ ����
{
	QueueType *cQ;
	int i;
	cQ = (QueueType *)malloc(sizeof(QueueType));
	cQ->front = 0;
	cQ->rear = 0; // ó���� front�� rear�� �Ѵ� 0���� �ʱ�ȭ
	cQ->queue = (element*)malloc(sizeof(element) * MAX_QUEUE_SIZE); // �ʿ�� ������ ����, queue�� MAX_QUEUE_SIZE��ŭ �����Ҵ����� ����

	// ť�� ���� ���� ��, debug�� �ʱ�ȭ�� �� �Ǽ� �̻��� ���� ����ִ� ���� �����ϱ� ���� ���� �߰�
	// �߰����� ������, �ƹ� ���뵵 �������� �ʾҾ / ( ���� �̻��� ���� ��µ���.
	for(i = 0; i < MAX_QUEUE_SIZE; i++)
	{
		cQ->queue[i] = '\0'; // �ʱ�ȭ
	}

	return cQ;
}

QueueType *queue_double(QueueType *cQ)
{
	QueueType *re_cQ; // ���ο� ť�� ����� ���� Queuetype ����ü ������ ����
	int i;
	char item;
	int re_max_queue_size = 2 * MAX_QUEUE_SIZE; // ���Ӱ� ���� re_CQ�� ť ������
	re_cQ = (QueueType *)malloc(sizeof(QueueType));
	re_cQ->front = 0;
	re_cQ->rear = MAX_QUEUE_SIZE-1; // front�� rear�� ��ġ ������
	re_cQ->queue = (element*)malloc(sizeof(element) * re_max_queue_size); // ���ο� ť ���� �����Ҵ�

	// �Ʒ� �ڵ���� ���� ���� �ű��, debug�� �̻��� �� ����ִ� ���� �����ϱ� ���� ���� ����
	re_cQ->queue[0] = '\0'; // front �κ� �ʱ�ȭ

	for(i = 1; i < MAX_QUEUE_SIZE; i++)
	{
		deQueue(cQ, &item); // deQueue�� Ȱ���Ͽ�, ���� ť�� ��� ������ ���ʴ�� �����Ͽ� �̵�
		re_cQ->queue[i] = item;
	}

	for(i = re_cQ->rear+1; i < re_max_queue_size; i++)
	{
		re_cQ->queue[i] = '\0'; // �ʱ�ȭ
	}

	MAX_QUEUE_SIZE = re_max_queue_size; // �ִ� ũ�⸦ ��Ÿ���� ���������� ���ο� ť�� MAX ũ��� ������

	freeQueue(cQ); // ���� cQ�� free
	return re_cQ; // ���ο� re_cQ�� ��ȯ�ϸ�, ������ cQ ������ ������ ���� re_cQ�� ����Ű�� �� ����.
}

int freeQueue(QueueType *cQ) // QueueType�� free�ϴ� �Լ�
{
    if(cQ == NULL) return 1; // �̹� �����Ǿ��ִٸ� ����
	free(cQ->queue);
    free(cQ); // ťŸ�԰�, ťŸ�� �ȿ� ���� ť ��� ���� �����Ҵ� �� ���̹Ƿ�, �Ϻ��� ������ ���ؼ� ���� ��������� ��.
    return 1;
}

element getElement() // ������ �����͸� �Է¹޴� �Լ�
{
	element item;
	printf("Input element = ");
	scanf(" %c", &item);
	return item;
}


/* complete the function */
int isEmpty(QueueType *cQ)
{ // ����ִٸ� 1, �׷��� �ʴٸ� 0�� ����
	if(cQ->front == cQ->rear) // front�� rear�� ��ġ�ϴ� ��Ȳ�� ����ִ� ��Ȳ
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
{ // �� á�ٸ� 1, �׷��� �ʴٸ� 0�� ����
	if(cQ->front == (cQ->rear+1)%MAX_QUEUE_SIZE) // ��� ����Ʈ �ٷ� �տ� �ִ� ��찡 �� �� ��Ȳ
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
{ // rear�� ���� ����ִ� ������ ������(������)�� ����Ű��, front�� �� ���� �� ��ġ�� ����Ŵ. �׸� ����ϸ� 
// ������ ������ ���� �Ʒ��� ���� ����� �ʿ���.
	cQ->rear = (cQ->rear+1)%MAX_QUEUE_SIZE; 
	cQ->queue[cQ->rear] = item;
}

/* complete the function */
void deQueue(QueueType *cQ, element *item)
{
    if(!isEmpty(cQ)) // ������� ���� ��쿡�� dequeue�� ����
	{
		// rear�� ���� ����ִ� ������ ������(������)�� ����Ű��, front�� �� ���� �� ��ġ�� ����Ŵ. �׸� ����ϸ� 
		// ������ ������ ���� �Ʒ��� ���� ����� �ʿ���.
		*item = cQ->queue[(cQ->front+1)%MAX_QUEUE_SIZE];
		// cQ->queue[(cQ->front+1)%MAX_QUEUE_SIZE] = '\0'; 	// debug�� ���� �״�� ���� ���� �����ϱ� ���� ���� �߰� (������ ���󿡼��� �Ϻη� �ν� �� ����, �ּ�ó���߽��ϴ�.)
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


void debugQ(QueueType *cQ) // ���� �������ִ� ������, ������� �����Ͽ� front�� tail�� ��ġ, �׸��� ť ��ü�� ���¸� Ȯ���� �� �ִ� �Լ�
{

	printf("\n---DEBUG\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++) // ť �����ŭ ���� �ص�
	{
		if(i == cQ->front) { // ����Ʈ�� ����Ʈ�� ���
			printf("  [%d] = front\n", i);
			continue;
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]); // �������� �׳� ���

	}
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear); // ����Ʈ�� ���� ��ġ ���
}

