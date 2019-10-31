#include<stdio.h>
#include<malloc.h>

/*
void insert(struct pq*, struct node*);//插入node,并把最大的node放到堆顶
void pop(struct pq*);
void init(struct pq*, int data);
void change(struct node* child, struct node* parent);
struct node
{
    int data;
    struct node *parent;
    struct node *lchild;
    struct node *rchild;
};

struct pq
{
    struct node* root;
    int length;
    struct node* last;
};

void insert(struct pq* pq, struct node* node)
{

    pq->last = node;
    struct node* cur = pq->last;
    for (int i = 0; i < pq->length -1; i++)
    {
        if (cur->data > cur->parent)
        {
            change(cur, cur->parent);
        }
    }
}
*/
//上面是想要用结构指针实现的思路，但是未完成
struct pq * pqinit(struct pq* pq, int maxlen);
void pqdelete(struct pq*);
void insert(struct pq* pq, int data);
int out(struct pq* pq);


struct pq
{
    int maxlen;
    int length;
    int *arr;
};

struct pq* pqinit(struct pq* pq,int maxlen)
{
    pq = (struct pq*)malloc(sizeof(struct pq));
    pq->arr = (int*)malloc((maxlen+1)* sizeof(int));
    pq->length = 0;
    pq->maxlen = maxlen;
    return pq;
}

void insert(struct pq* pq, int data)
{
    if (pq->length >= (pq->maxlen))
    {
        printf("PQ is full");
    }
    else
    {
    int i;
        /*不断和父节点探测比较，小的放上，直到找到属于它的位置*/
    for(i = pq->length+1;pq->arr[i/2] > data && i > 1;i/=2)
    {
        pq->arr[i] = pq->arr[i/2];
    }
    pq->arr[i] = data;
    pq->length++;

    }
}

int out(struct pq* pq)
{
    int i = 1;
    int mindata = pq->arr[1];
    while (2*i +1<= pq->length)
    {
        if ( pq->arr[2* i] < pq->arr[2*i+1])//父节点跟小的子节点交换
        {
            int temp = pq->arr[i];
            pq->arr[i] = pq->arr[2*i];
            pq->arr[2*i] = temp;
            i *= 2;
        }
        else
        {
            int temp = pq->arr[i];
            pq->arr[i] = pq->arr[2*i+1];
            pq->arr[2*i+1] = temp;
            i = 2*i + 1;
        }
        int temp = pq->arr[i];
        pq->arr[i] = pq->arr[pq->length];
        pq->arr[pq->length] = temp;//把原来的根节点放到最后
        pq->length--;
    }
    printf("%d\n", mindata);
    return mindata;
}

void pqdelete(struct pq* pq)
{
    free(pq->arr);
    free(pq);
}


int main()
{
    struct pq* pq;
    pq = pqinit(pq, 20);
    insert(pq, 15);
    insert(pq, 7);
    insert(pq, 50);
    insert(pq,12);
    out(pq);
    out(pq);
    pqdelete(pq);
    return 0;
}