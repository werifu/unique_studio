#include<stdio.h>
#include<string.h>
#define MAX 100
int fundfind(int x);
int *fundmerge(int x, int y);//把x和y所在集合合并
int ysfind(int x);//路径压缩版本查询
int *mkset(int x);//初始化集合
void rankmerge(int x, int y);//按秩合并，秩小的往秩大的根上贴，只要二者秩不同就不会增加秩大小
int getrank(int x);


int dad[MAX];//储存dad[x]储存x的父亲
int rank[MAX];

int* mkset(int x)//x不能超过MAX
{
    for (int i = 0; i < x; i++)
        dad[i] = i;
    for (int i = 0; i< x; i++)
        rank[i] = 1;
    return dad;
}

int fundfind(int x)//最基本的祖先查询，沿着父子链一直往上走
{
    if (x == dad[x])//父亲是自己的就是祖父了
    return x;
    else
    return fundfind(dad[x]);
}

int ysfind(int x)
{
    if (dad[x] != x)
    dad[x] = ysfind(dad[x]);//第一次查询就把树给扁平化，之后的查询就变成1次完成了
    return dad[x];
}

int* fundmerge(int x, int y)
{
    int xroot = fundfind(x);
    int yroot = fundfind(y);
    dad[xroot] = yroot;
    return dad;
}
int rankfind(int x)
{
    if (dad[x] != x)
    dad[x] = ysfind(dad[x]);//第一次查询就把树给扁平化，之后的查询就变成1次完成了
    return dad[x];
}



void rankmerge(int x, int y)
{
    int xroot = fundfind(x);
    int yroot = fundfind(y);
    if (xroot == yroot) 
    {
        printf("同一家人");
        return ;
    }   
    else//x和y不同家，可合并
    {
        if (rank[xroot] > rank[yroot])
                dad[yroot] = xroot;
        else if (rank[x] == rank[y])
        {
            dad[xroot] = yroot;
            rank[yroot] += 1;
        }
        else
            dad[xroot] = yroot;
        
    }
    
}

int main()
{
    mkset(6);
    rankmerge(1,3);
    rankmerge(2,3);
    rankmerge(1, 5);
    printf("%d", fundfind(5));
    return 0;
}
