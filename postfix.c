/* postfix.c
 *
 * ?Data Structures, Homework #5
 * ?School of Computer Science at Chungbuk National University
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_STACK_SIZE 10
#define MAX_EXPRESSION_SIZE 20

/* stack 내에서 우선순위, lparen = 0 가장 낮음 */
typedef enum{
	lparen = 0,  /* ( 왼쪽 괄호 */
	rparen = 9,  /* ) 오른쪽 괄호*/
	times = 7,   /* * 곱셈 */
	divide = 6,  /* / 나눗셈 */
	plus = 5,    /* + 덧셈 */
	minus = 4,   /* - 뺄셈 */
	operand = 1 /* 피연산자 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];   	/* infix expression을 저장하는 배열 */
char postfixExp[MAX_EXPRESSION_SIZE];	/* postfix로 변경된 문자열을 저장하는 배열 */
char postfixStack[MAX_STACK_SIZE];	/* postfix로 변환을 위해 필요한 스택 */
int evalStack[MAX_STACK_SIZE];		/* 계산을 위해 필요한 스택 */

int postfixStackTop = -1;  /* postfixStack용 top */
int evalStackTop = -1;	   /* evalStack용 top */

int evalResult = 0;	   /* 계산 결과를 저장 */

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
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
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
	case '/' : return divide;
	case '*' : return times;
	default : return operand;
	}
}

precedence getPriority(char x)
{
	return getToken(x);
}

/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char* c)
{
	if (postfixExp == '\0')
		strncpy(postfixExp, c, 1);
	else
		strncat(postfixExp, c, 1);
}

/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix()
{
	/* infixExp의 문자 하나씩을 읽기위한 포인터 */
	char *exp = infixExp;
	char x; /* 문자하나를 임시로 저장하기 위한 변수 */
	char temp; // 임시 변수 2
	precedence current_priority;

	/* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
	while(*exp != '\0')
	{
		x = *exp;
		current_priority = getPriority(x);
		if(current_priority == operand) // operand면 우선적으로 바로 postFixExp에 넣기
		{
			charCat(&x);
		}
		else if (current_priority == lparen) // 현재 x가 '('라면
		{ // 스택에 추가하기
			postfixPush(x);
		}
		else if(current_priority == rparen) // 현재 x가 만약 ')'라면
		{
			while(getPriority(postfixStack[postfixStackTop]) != lparen) // '(' 이후의 모든 스택에 쌓인 연산자 빼내기
			{
				temp = postfixPop();
				charCat(&temp);
			}
			postfixPop(); // '(' 제거
		}
		else if (postfixStackTop == -1 || getPriority(postfixStack[postfixStackTop]) <= current_priority) // 현재 x의 우선순위가 현재 포스트픽스스택의 탑 내용물 우선순위보다 크거나 같다면(빠르거나 같다면)
		{ // 스택에 추가하기
			postfixPush(x);
		}
		else // 현재 x의 우선순위가 현재 포스트픽스스택의 탑 내용물보다 작은(느린) 나머지 경우
		{
			// 포스트픽스스택의 탑 내용물 우선순위가 x의 우선순위보다 작을 때까지 모두 빼내고, 현재 x는 포스트픽스 스택에 넣기
			while(getPriority(postfixStack[postfixStackTop]) >= current_priority)
			{
				temp = postfixPop();
				charCat(&temp);
			} //현재 x의 우선순위
			postfixPush(x);
		} 

		exp += 1;
	}
	
	while(postfixStackTop != -1) // 스택에 남은 연산자들을 모두 빼내기
	{
		temp = postfixPop();
		charCat(&temp);
	}

	/* 필요한 로직 완성 */

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

	/* postfixExp의 문자 하나씩을 읽기위한 포인터 */
	char *exp = postfixExp;
	char x; /* 문자하나를 임시로 저장하기 위한 변수 */
	int num1, num2; // 연산을 위한 변수

	/* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
	while(*exp != '\0')
	{
		x = *exp;
		if(getPriority(x) == operand)
		{
			evalPush(x-48); // operand라면, 숫자로 변환해서 스택에 추가
		}
		else
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
	/* postfixExp, evalStack을 이용한 계산 */
}
