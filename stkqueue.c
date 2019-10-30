#include <stdio.h>
#include<malloc.h>

void schange(struct stack* stk1,struct stack* stk2);//倒库操作,把stk1的元素倒到stk2里
void sinit(struct stack * stk);//初始化一个栈
void inque(struct queue* que, int data);
void outque(struct queue* que);
void qinit(struct queue* que);
void push(struct stack *stk, int data);
int pop(struct stack *stk);

struct queue
{
    struct stack* stk1;
    struct stack* stk2;
};
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


void schange(struct stack* stk1, struct stack* stk2)//stk1倒到stk2里
{
    struct node* cur1, *cur2;
    while (stk1->length > 1)
    {
    cur1 = stk1->p;
    //下面是对stk2的push操作
    cur2 = (struct node*)malloc(sizeof(struct node));
    cur2->data = cur1->data;
    cur2->next = stk2->p;
    stk2->p = cur2;
    stk2->length ++;
    //下面是对stk1的pop操作
    stk1->p = cur1->next;
    free(cur1);
    stk1->length--;
    }
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
    printf("%d\n", x);
    return x;
}


void sinit(struct stack * stk)
{
    
    struct node * cur;
    cur = (struct node *)malloc(sizeof(struct node));
    cur->data = 0;
    cur->next = NULL;
    stk->length = 1;
    stk->p = cur;
}

void qinit(struct queue *que)
{
    struct stack *stk1, *stk2;
    stk1 = (struct stack*)malloc(sizeof(struct stack));
    stk2 = (struct stack*)malloc(sizeof(struct stack));
    sinit(stk1);
    sinit(stk2);
    que->stk1 = stk1;
    que->stk2 = stk2;

}


void inque(struct queue* que, int data)
{
    if ((que->stk1->length > 1 && que->stk2->length == 1)|| (que->stk1->length == 1 && que->stk2->length == 1))//说明现在处于在stk1的状态，直接入库
        push(que->stk1, data);
    else//说明现在处于skt2的状态，需要倒到skt1里边再入库
    {
        schange(que->stk2, que->stk1);
        push(que->stk1, data);
    }
}

void outque(struct queue* que)
{
     if (que->stk1->length == 1 && que->stk2->length > 1)//说明现在处于在stk2的状态，直接出库
        pop(que->stk2);
    else//说明现在处于skt1的状态，需要倒到skt2里边再出库
    {
        schange(que->stk1, que->stk2);
        pop(que->stk2);
    }
}


int main()
{
    struct queue *que;
    qinit(que);
    inque(que, 12450);
    inque(que, 114514);
    inque(que, 900);
    outque(que);
    outque(que);
    outque(que);
    return 0;
}