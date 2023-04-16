/* postfix.c
 *
 * ?Data Structures, Homework #5
 * ?School of Computer Science at Chungbuk National University
 */
// ���� �ڵ弭 ������ �κ� ���� : isp, icp �迭�� �߰��Ͽ�, '('�� ���� �ڵ��� ���⼺�� �ٿ����ϴ�.

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_STACK_SIZE 10
#define MAX_EXPRESSION_SIZE 20

// stack ������ �켱����, lparen = 0 ���� ����. ���� ���� �ڵ��� �켱����
typedef enum{
	lparen,  // ( ���� ��ȣ 
	rparen,  // ) ������ ��ȣ
	plus,    // + ���� 
	minus,   // - ���� 
	divide,  // / ������ 
	times,   // * ���� 
	operand // �ǿ����� 
} precedence;

// 0,10 : isp�������� icp �������� ���� ��ȣ �켱����
// 1 : ������ ��ȣ �켱����
// 2 : +, - �켱����
// 3 : *, / �켱����
// -1 : operand �켱���� 
static int isp[7] = {0,1,2,2,3,3,-1}; // in-stack precedence
static int icp[7] = {10,1,2,2,3,3,-1}; // incoming precedence 

char infixExp[MAX_EXPRESSION_SIZE];   	/* infix expression�� �����ϴ� �迭 */
char postfixExp[MAX_EXPRESSION_SIZE];	/* postfix�� ����� ���ڿ��� �����ϴ� �迭 */
char postfixStack[MAX_STACK_SIZE];	/* postfix�� ��ȯ�� ���� �ʿ��� ���� */
int evalStack[MAX_STACK_SIZE];		/* ����� ���� �ʿ��� ���� */

int postfixStackTop = -1;  /* postfixStack�� top */
int evalStackTop = -1;	   /* evalStack�� top */

int evalResult = 0;	   /* ��� ����� ���� */

void postfixpush(char x);
char postfixPop();
void evalPush(int x);
int evalPop();
void getInfix();
precedence getToken(char symbol);
precedence getPriority(char x);
void charCat(char* c);
void toPostfix();
void debug();
void reset();
void evaluation();

int main()
{
	char command;

	do{
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");
		printf("[--------------  [�ֻ�]  	[2022041062]  --------------]\n");
		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'i': case 'I':
			getInfix();
			break;
		case 'p': case 'P':
			toPostfix();
			break;
		case 'e': case 'E':
			evaluation();
			break;
		case 'd': case 'D':
			debug();
			break;
		case 'r': case 'R':
			reset();
			break;
		case 'q': case 'Q':
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

void postfixPush(char x)
{
    postfixStack[++postfixStackTop] = x;
}

char postfixPop()
{
    char x;
    if(postfixStackTop == -1)
        return '\0';
    else {
    	x = postfixStack[postfixStackTop--];
    }
    return x;
}

void evalPush(int x)
{
    evalStack[++evalStackTop] = x;
}

int evalPop()
{
    if(evalStackTop == -1)
        return -1;
    else
        return evalStack[evalStackTop--];
}

/**
 * infix expression�� �Է¹޴´�.
 * infixExp���� �Էµ� ���� �����Ѵ�.
 */
void getInfix()
{
    printf("Type the expression >>> ");
    scanf("%s",infixExp);
}

precedence getToken(char symbol)
{
	switch(symbol) {
	case '(' : return lparen;
	case ')' : return rparen;
	case '+' : return plus;
	case '-' : return minus;
	case '*' : return times;
	case '/' : return divide;
	default : return operand;
	}
}

precedence getPriority(char x)
{
	return getToken(x);
}

/**
 * �����ϳ��� ���޹޾�, postfixExp�� �߰�
 */
void charCat(char* c)
{
	if (postfixExp == '\0')
		strncpy(postfixExp, c, 1);
	else
		strncat(postfixExp, c, 1);
}

/**
 * infixExp�� ���ڸ� �ϳ��� �о�鼭 stack�� �̿��Ͽ� postfix�� �����Ѵ�.
 * ����� postfix�� postFixExp�� ����ȴ�.
 */
void toPostfix()
{
	/* infixExp�� ���� �ϳ����� �б����� ������ */
	char *exp = infixExp;
	char x; /* �����ϳ��� �ӽ÷� �����ϱ� ���� ���� */
	char temp; // �ӽ� ���� 2
	precedence current_priority;

	/* exp�� �������Ѱ��鼭, ���ڸ� �а� postfix�� ���� */
	while(*exp != '\0')
	{
		x = *exp;
		current_priority = getPriority(x);
		if(current_priority == operand) // operand�� �켱������ �ٷ� postFixExp�� �ֱ�
		{
			charCat(&x);
		}
		else if(current_priority == rparen) // ���� x�� ���� ')'���
		{
			while(getPriority(postfixStack[postfixStackTop]) != isp[lparen]) // '(' ������ ��� ���ÿ� ���� ������ ������
			{
				temp = postfixPop();
				charCat(&temp);
			}
			postfixPop(); // '(' ����
		}
		else
		{
			// ������ ������� ������, ���� x�� �켱������ ���� ����Ʈ�Ƚ� ������ ž ���빰���� ����(����) ������ ��쿡��
			// ����Ʈ�Ƚ������� ž ���빰 �켱������ x�� �켱�������� �۾����ų� �� ���� ������ ��� ������, ���� x�� ����Ʈ�Ƚ� ���ÿ� �ֱ�
			while(postfixStackTop != -1 && (isp[getPriority(postfixStack[postfixStackTop])] >= icp[current_priority]))
			{
				temp = postfixPop();
				charCat(&temp);
			} 
			postfixPush(x);
		} 

		exp += 1;
	}
	
	while(postfixStackTop != -1) // ���ÿ� ���� �����ڵ��� ��� ������
	{
		temp = postfixPop();
		charCat(&temp);
	}


}
void debug()
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp);
	printf("postExp =  %s\n", postfixExp);
	printf("eval result = %d\n", evalResult);

	printf("postfixStack : ");
	for(int i = 0; i < MAX_STACK_SIZE; i++)
		printf("%c  ", postfixStack[i]);

	printf("\n");

}

void reset()
{
	infixExp[0] = '\0';
	postfixExp[0] = '\0';

	for(int i = 0; i < MAX_STACK_SIZE; i++)
		postfixStack[i] = '\0';
         
	postfixStackTop = -1;
	evalStackTop = -1;
	evalResult = 0;
}

void evaluation()
{

	/* postfixExp�� ���� �ϳ����� �б����� ������ */
	char *exp = postfixExp;
	char x; /* �����ϳ��� �ӽ÷� �����ϱ� ���� ���� */
	int num1, num2; // ������ ���� ����

	/* exp�� �������Ѱ��鼭, ���ڸ� �а� postfix�� ���� */
	while(*exp != '\0')
	{
		x = *exp;
		if(getPriority(x) == operand)
		{
			evalPush(x-48); // operand���, ���ڷ� ��ȯ�ؼ� ���ÿ� �߰�
		}
		else // �ƴ� ����, ���� ����
		{
				switch(x) 
				{
					case '+' :
					{
						num2 = evalPop();
						num1 = evalPop();
						evalPush(num1+num2);
						break;
					}
					case '-' : 
					{
						num2 = evalPop();
						num1 = evalPop();
						evalPush(num1-num2);
						break;
					}
					case '/' : 
					{
						num2 = evalPop();
						num1 = evalPop();
						evalPush(num1/num2);
						break;
					}
					case '*' : 
					{
						num2 = evalPop();
						num1 = evalPop();
						evalPush(num1*num2);
						break;
					}
				}
		}

		exp += 1;

	}

	evalResult = evalPop();
	/* postfixExp, evalStack�� �̿��� ��� */
}
