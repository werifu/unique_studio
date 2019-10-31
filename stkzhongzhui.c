#include<stdio.h>
#include<malloc.h>

struct stack * init(struct stack*);
void push(struct stack*, char ch);
char pop(struct stack*);

void pour(struct stack* stk1, struct stack * stk2);
void analy(struct stack* stk1, struct stack* stk2, char ch);//对读到的字符进行分析并归位
int ishigh(char ch1, char ch2);//比较运算符高低

struct node 
{
    char ch;
    struct node * next;
};

struct stack 
{
    int len;
    struct node * p;
};

struct stack* init(struct stack*stk)
{
    stk = (struct stack*)malloc(sizeof(struct stack));
    stk->p = NULL;
    stk->len = 0;
    return stk;
};

void push(struct stack *stk, char ch)
{
    struct node *cur;
    cur = (struct node *)malloc(sizeof(struct node));
    cur->ch = ch;
    cur->next = stk->p;
    stk->p = cur;
    stk->len++;
}
char pop(struct stack *stk)
{
    struct node * cur;
    char x;
    cur = stk->p;
    x = cur->ch;
    stk->p = cur->next;
    free(cur);
    stk->len--;
    return x;
}

void pour(struct stack* stk1, struct stack * stk2)//把stk1的东西倒到stk2里边
{
    while (stk1->p != NULL)
    {
        push(stk2, stk1->p->ch);
        pop(stk1);
    }
}
int ishigh(char ch1, char ch2)
{
    if ((ch1 == '*' || ch1 == '/')&&(ch2 == '+' || ch2 == '-')) return 1;
    else return 0;
}


void analy(struct stack *stk1, struct stack* stk2, char ch)
{
    if((ch >= 'a' && ch <= 'z') || (ch >='A' && ch <= 'Z'))//说明是操作数abcd,入stk2
    {
        push(stk2, ch);
    }
    else if (ch == '+' || ch == '-' || ch =='*' || ch == '/')//说明是运算符
    {
        while (1)
        {
            if( (stk1->p == NULL)||(stk1->p->ch == '('))
            {
                push(stk1, ch);
                break;
            }
            else if (ishigh(ch, stk1->p->ch))
            {
                push(stk1, ch);
                break;
            }
            else
            {
                push(stk2, stk1->p->ch);
                pop(stk1);
            }
        }
    }

    else if (ch == '(' )//左右括号
    {
        push(stk1, ch);
    }
    else if (ch ==')')
    {
        while (stk1->p->ch != '(')
        {
            push(stk2, stk1->p->ch);
            pop(stk1);
        }
        pop(stk1);//舍弃左括号(右括号并没有打入)
    
    }
}


int main()
{
    struct stack *stk1, *stk2;
    stk1 = init(stk1);//初始化运算符stack（+-*/）
    stk2 = init(stk2);//初始化操作符号(abcd等)
    char ch;
    while ((ch = getchar()) != '\n')
    {
        analy(stk1, stk2, ch);
    }
    pour(stk2, stk1);//把stk2里剩下的都倒到stk1里
    
    while (stk1->p != NULL)
    {
        putchar(stk1->p->ch);
        pop(stk1);
    }
    
    return 0;
}

