/* postfix.c
 *
 * ?Data Structures, Homework #5
 * ?School of Computer Science at Chungbuk National University
 */
// 기존 코드서 부족한 부분 수정 : isp, icp 배열을 추가하여, '('로 인한 코드의 복잡성을 줄였습니다.

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_STACK_SIZE 10 // 스택의 MAX 사이즈
#define MAX_EXPRESSION_SIZE 20 // 받을 수 있는 식의 MAX 사이즈 20글자까지.

// stack 내에서 우선순위, lparen = 0 가장 낮음. 기존 원래 코드의 우선순위
typedef enum{
	lparen,  // ( 왼쪽 괄호 
	rparen,  // ) 오른쪽 괄호
	plus,    // + 덧셈 
	minus,   // - 뺄셈 
	divide,  // / 나눗셈 
	times,   // * 곱셈 
	operand // 피연산자 
} precedence;

// 0,10 : isp내에서와 icp 내에서의 왼쪽 괄호 우선순위 
// 왼쪽 괄호의 우선순위를 다르게 하지 않으면, 
// 의도치 않은 연산이 일어나는 것을 막기 위해 postfix 연산에서 '('에 대해 추가적인 조건이 필요해진다.
// 1 : 오른쪽 괄호 우선순위
// 2 : +, - 우선순위
// 3 : *, / 우선순위
// -1 : operand 우선순위 
static int isp[7] = {0,1,2,2,3,3,-1}; // in-stack precedence
static int icp[7] = {10,1,2,2,3,3,-1}; // incoming precedence 

char infixExp[MAX_EXPRESSION_SIZE];   	/* infix expression을 저장하는 배열 */
char postfixExp[MAX_EXPRESSION_SIZE];	/* postfix로 변경된 문자열을 저장하는 배열 */
char postfixStack[MAX_STACK_SIZE];	/* postfix로 변환을 위해 필요한 스택 */
int evalStack[MAX_STACK_SIZE];		/* 계산을 위해 필요한 스택 */

int postfixStackTop = -1;  /* postfixStack용 top */
int evalStackTop = -1;	   /* evalStack용 top */

int evalResult = 0;	   /* 계산 결과를 저장 */

void postfixpush(char x); // postfix로 변환을 할 때 쓰는 스택에 푸시하는 함수
char postfixPop(); // postfix로 변환을 할 때 쓰는 스택에서 팝하는 함수
void evalPush(int x); // 계산을 할 때 쓰는 스택에 푸시하는 함수
int evalPop(); // 계산을 할 때 쓰는 스택에 푸시하는 함수
void getInfix(); // 중위표기식을 입력받는 함수
precedence getToken(char symbol); // 우선순위를 얻기 위한 함수
precedence getPriority(char x); // 우선순위를 얻기 위한 함수
void charCat(char* c); // postfix 변환을 할 때 쓰는 함수로, 문자를 덧붙여준다
void toPostfix(); // postfix 변환 함수
void debug(); // 디버그용 함수
void reset(); // 리셋 함수
void evaluation(); // 계산 함수

int main()
{
	char command;

	do{
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");
		printf("[--------------  [최상영]  	[2022041062]  --------------]\n");
		printf("Command = ");
		scanf(" %c", &command); // command 변수 입력받기

		switch(command) { // 입력받은 command 변수로 실행할 기능 구분 switch
		case 'i': case 'I':
			getInfix(); // 중위표기식 입력받기
			break;
		case 'p': case 'P':
			toPostfix(); // postfix 변환하기
			break;
		case 'e': case 'E':
			evaluation(); // 변환된 후위표기식 계산하기
			break;
		case 'd': case 'D':
			debug(); // 디버그하기
			break;
		case 'r': case 'R':
			reset(); // 리셋하기
			break;
		case 'q': case 'Q': // 종료
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n"); // 이외 고려하지 않은 상황시 Concentration!! 출력
			break;
		}

	}while(command != 'q' && command != 'Q'); // q 받으면 종료

	return 0; // 리턴 1이였는데, 정상종료를 알리기 위해 리턴 0으로 변경
}

void postfixPush(char x) 
{
    postfixStack[++postfixStackTop] = x; // postfix 변환을 위한 스택에 데이터 추가
}

char postfixPop()
{
    char x;
    if(postfixStackTop == -1) // 추출할 데이터가 없다면 '\0' 리턴
        return '\0';
    else {
    	x = postfixStack[postfixStackTop--]; // postfix 변환을 위한 스택에서 데이터 추출
    }
    return x;
}

void evalPush(int x)
{
    evalStack[++evalStackTop] = x; // 계산을 위한 스택에 데이터 추가
}

int evalPop()
{
    if(evalStackTop == -1) // 추출할 데이터가 없다면 '-1' 리턴
        return -1;
    else
        return evalStack[evalStackTop--]; // 계산을 위한 스택에서 데이터 추출
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
	case '*' : return times;
	case '/' : return divide;
	default : return operand;
	} // 문자에 해당하는 우선순위를 리턴함
}

precedence getPriority(char x)
{
	return getToken(x); // gettoken 함수를 호출해서, 우선순위를 받아옴
}

/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char* c) // postfix 변환을 할 때 쓰는 함수로, 문자를 덧붙여준다
{
	if (postfixExp == '\0') // 비어있을땐 strncpy를 통해 복사 연산으로서 내용을 넣음
		strncpy(postfixExp, c, 1);
	else // 일반적인 상황에서는 원래 있는 postfixExp에 매개변수로 받은 문자를 뒤에 연결함.
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
	precedence current_priority; // 현재 문자(x)의 우선순위를 나타내줄 변수

	/* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
	while(*exp != '\0') // 중위표기식을 끝까지 다 읽을 때까지 진행
	{
		x = *exp; // 문자 받아옴
		current_priority = getPriority(x); // 우선순위 구해옴
		if(current_priority == operand) // operand면 우선적으로 바로 postFixExp에 넣기
		{
			charCat(&x);
		}
		else if(current_priority == rparen) // 현재 x가 만약 ')'라면
		{
			while(getPriority(postfixStack[postfixStackTop]) != isp[lparen]) // '(' 이후의 모든 스택에 쌓인 연산자 빼내기
			{
				temp = postfixPop();
				charCat(&temp);
			}
			postfixPop(); // '(' 제거
		}
		else
		{
			// 스택이 비어있지 않으며, 현재 x의 우선순위가 현재 포스트픽스 스택의 탑 내용물보다 작거나 같은(느리거나 같은) 경우에는
			// 포스트픽스스택의 탑 내용물 우선순위가 x의 우선순위보다 작아지거나, 내용물이 없을 때까지 모두 빼낸다
			// 이후 현재 x는 포스트픽스 스택에 삽입하기
			while(postfixStackTop != -1 && (isp[getPriority(postfixStack[postfixStackTop])] >= icp[current_priority]))
			{
				temp = postfixPop();
				charCat(&temp);
			} // 스택이 비어있거나, x의 우선순위가 스택 내에 탑 내용물의 우선순위보다 크다면, 바로 스택에 삽입된다.
			postfixPush(x);
		} 

		exp += 1;
	}
	
	while(postfixStackTop != -1) // 스택에 남은 연산자들을 모두 빼내기
	{
		temp = postfixPop();
		charCat(&temp);
	}


}
void debug()
{ // 각각 중위표기식, 후위표기식, 후위표기식 계산 결과, postfixstack의 상태를 출력해준다
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
{ // 모든 값들을 초기화해준다.
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

	// exp를 증가시켜가면서, 문자를 읽고 x에 저장한다
	while(*exp != '\0') // 후위표기식을 끝까지 다 읽을 때까지 진행
	{
		x = *exp;
		if(getPriority(x) == operand)
		{
			evalPush(x-48); // x가 operand라면, 숫자로 변환해서 스택에 추가
		}
		else // 아닌 경우는, 연산 진행
		{
				switch(x) 
				{ // 숫자를 스택에서 2개 뽑아와서 각각 연산자 케이스에 맞는 연산을 진행하고, 
				// 다시 스택에 연산 결과 삽입
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

	evalResult = evalPop(); // 모든 연산이 끝나고, 마지막에 남은 수가 전체 연산의 결과임.
	/* postfixExp, evalStack을 이용한 계산 */
}
