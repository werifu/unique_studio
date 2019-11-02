#include<stdio.h>
#include<malloc.h>
#include<queue>
#include<stack>
using namespace std;
struct node* newnode2(struct node* root, int data);
void traverse(struct node*);//dfs递归遍历
void dfsiter(struct node*);//dfs迭代遍历(stack实现))
void deletetree(struct node*);
void bfs(struct node* root);//bfs遍历(queue实现)
void insert(struct node* root, struct node* node);//把node插到以root的树上
struct node* search(struct node* root,int data);//二叉搜索树的查询
void denode1(struct node* parent, struct node* cur, bool isleft);
void denode2(struct node* root, int data);


struct node 
{
    int data;
    struct node* lchild;
    struct node* rchild;
};



struct node*  newnode(struct node*node, int data)
{
    node = (struct node*)malloc(sizeof(struct node));
    node->data = data;
    node->lchild = NULL;
    node->rchild = NULL;
    return node;
}

void deletetree(struct node* root)
{
    if (root != NULL) 
    {
        traverse(root->lchild);
        traverse(root->rchild);
        free(root);
    }
    return ;
}

//下边是DFS（深度优先搜索的遍历）
void traverse (struct node* root) 
{
    if (root != NULL) 
    {
        printf("%d\n", root->data);//先序
        traverse(root->lchild);
        //printf("%d\n", root->data);//中序
        traverse(root->rchild);
        //printf("%d\n", root->data);//后序
    }
}

void dfsiter(struct node* root)//dfs的迭代遍历，用stack实现
{
    if (root == NULL) return ;

    stack<node*> S;
    S.push(root);

    while(!S.empty())//当栈不空时
    {
        struct node* node = S.top();
        printf("%d\n", node->data);
        S.pop();
        
        if ( node->rchild != NULL)
        S.push(node->rchild);
        if ( node->lchild != NULL)
        S.push(node->lchild);

    }
}

//下边是BFS(广度优先的遍历,从上往下走)
void bfs(struct node* root)
{
    if (root == NULL) return ;
    queue<node*> Q;
    
    Q.push(root);

    while ( !Q.empty())//队列不为空的话
    {
        struct node* node = Q.front();
        printf("%d \n", node->data);
        if (node->lchild != NULL)//左子树入队后再让右子树入队
        Q.push(node->lchild);
        if (node->rchild != NULL)
        Q.push(node->rchild);

        Q.pop();//左树会被pop出去
    }
    printf("\n");
}

void insert(struct node* root, struct node* node)//二叉搜索树的插入
{
    if (root->data > node->data)
    {
        if (root->lchild == NULL)
        {
            root->lchild = node;
            return ;
        }
        else
            insert(root->lchild, node);
    }
    else
    {
        if (root->rchild == NULL)
        {
            root->rchild = node;
            return ;
        }
        else
            insert(root->rchild, node);
    }
    
}
void denode1(struct node* parent, struct node* cur, bool isleft)//进行删除操作的函数
{
    if (cur->lchild == NULL && cur->rchild == NULL)//如果是叶子节点
    {
        if (cur == parent) free(cur);
        else if (isleft)
        {
            parent->lchild = NULL;
            free(cur);
        }
        else
        {
            parent->rchild = NULL;
            free(cur);
        }
    }
    else if (cur->rchild == NULL)//如果没有右子
    {
        if (cur == parent)  free(cur);
        else if(isleft)
        {
            parent->lchild = cur->lchild;
            free(cur);
        }
        else
        {
            parent->rchild = cur->lchild;
            free(cur);
        }
    }
    else if (cur->lchild == NULL)//如果没有左子
    {
        if (cur == parent)  free(cur);
        else if(isleft)
        {
            parent->lchild = cur->rchild;
            free(cur);
        }
        else
        {
            parent->rchild = cur->rchild;
            free(cur);
        }
    }
    else//有两个孩子
    {
        struct node* minnode;
        struct node* minpa;
        minnode = cur->rchild;//要从右子中找到最小
        if (minnode->lchild == NULL)
        {
            cur->rchild = minnode->rchild;
        }
        else
        {
        while(minnode->lchild != NULL)
        {
            minpa = minnode;
            minnode = minnode->lchild;
        }
        if (isleft)
        {
            parent->lchild = minnode;
            minpa->lchild = minnode->rchild;
            minnode->lchild = cur->lchild;
            minnode->rchild = cur->rchild;
        }
        else
        {
            parent->rchild = minnode;
            minpa->lchild = minnode->rchild;
            minnode->lchild = cur->lchild;
            minnode->rchild = cur->rchild;
        }
        
        }
    }
}

void denode2(struct node* root, int data)
{
    struct node* parent = root;//先保存父节点
    struct node* cur = parent;//保存当前节点
    bool isleft = true;
    while (data != cur->data)
    {
        if (data < cur->data)
        {
            isleft = true;
            if (cur->lchild != NULL)
            {
                parent = cur;
                cur = cur->lchild;
            }
        }
        else//经过右节点
        {
            isleft = false;
            if (cur->rchild != NULL)
            {
                parent = cur;
                cur = cur->rchild;
            }
        }
        if (cur == NULL) return ;
    }
    denode1(parent, cur, isleft);

}


struct node* search(struct node* root, int data)
{
    if (root->data == data)
        return root;
    else if (data < root->data)
        {
            if (root->lchild == NULL)
                {
                    printf("not found");
                    return NULL;
                }
            return search(root->lchild, data);
        }
    else
        {
            if (root->rchild == NULL)
                {
                    printf("not found");
                    return NULL;
                }
            return search(root->rchild, data);
        }
}


int main()
{
    struct node* root = newnode(root, 20);
    struct node* node2 = newnode(node2, 10);
    //root->lchild = node2;
    struct node* node3 = newnode(node3, 19);
    //root->rchild = node3;
    struct node* node4 = newnode(node4, 25);
    //node2->lchild = node4;
    struct node* node5 = newnode(node5, 17);
    //node2->rchild = node5;
    struct node* node6 = newnode(node6, 7);
    insert(root, node2);
    insert(root, node3);
    insert(root, node4);
    insert(root, node5);
    insert(root, node6);
    denode2(root, 10);
    traverse(root);
    //dfsiter(root);
    //search(root, 25);
    return 0;
}

