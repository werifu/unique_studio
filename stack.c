//先入先出的栈
#include <stdio.h>
#include<malloc.h>

void init(struct stack *stk, int data);
int pop(struct stack * stk);
void push(struct stack *stk, int data);
void d(struct stack * stk);

struct node
{
    int data;
    struct node *next;
};

struct stack
{
    int length;
    struct node *p;
};

void init(struct stack * stk, int data)
{
    struct node * cur;
    cur = (struct node *)malloc(sizeof(struct node));
    cur->data = data;
    cur->next = NULL;
    stk->length = 1;
    stk->p = cur;
}

void push(struct stack *stk, int data)
{
    struct node *cur;
    cur = (struct node *)malloc(sizeof(struct node));
    cur->data = data;
    cur->next = stk->p;
    stk->p = cur;
    stk->length ++;
}

int pop(struct stack *stk)
{
    struct node * cur;
    int x;
    cur = stk->p;
    x = cur->data;
    stk->p = cur->next;
    free(cur);
    stk->length--;
    return x;
}

void d(struct stack * stk)
{
    while (stk->length!=1)
    pop(stk);
    
}


int main()
{
    struct stack * stk;
    int data = 1;
    init(stk, data);
    push(stk, 114514);
    push(stk, 12450);
    printf("%d\n", pop(stk));
    d(stk);
    return 0;
}


