#include <stdio.h>
#define SIZE 10
int bubble()
{
    //升序冒泡
    int x;
    int arr [SIZE] = {1, 4, 2, 19, 27, 8, 2, 6,12, 7};
    for(int i = 0; i < SIZE; i++)//每个数字都来冒
    {
        int  flag = 0;//当前轮是否有进行过交换

        for (int j = 0;j < SIZE-1-i; j++)//每轮过后最右边一定是最大的
        {
            if (arr[j] > arr[j+1])
            {
                x = arr[j+1];
                arr[j+1] = arr[j];
                arr[j] = x;//左右交换
                flag = 1;//挂旗
            }
        }
        if (flag == 0) break;

    }
    for (int m = 0; m< SIZE; m++) printf("%d\n", arr[m]);
    return 0;
}


int insert()//升序插入排序
{
    int arr[SIZE] =  {1, 4, 2, 19, 27, 8, 2, 6,12, 7};
    int arr2[SIZE];
    arr2[0] = arr[0];
    int flag = 0;
    int bit = 1;//左边已经排好bit个
    for (int i = 1; i < SIZE; i++)//除了第一个，其余都拿出来搓牌
    {
        for (int j = 0; j < bit; j++)//拿出来的数从小往大跟排好的比
        {
            if ( arr[i] < arr2[j])
            {
                for (int k = bit; k > j; k--)//右边全都右移
                {
                    arr2[k] = arr2[k-1];
                }
                bit++;
                flag = 1;//表示右移过了
                arr2[j] = arr[i];
                break;
            }
        }
        if (flag == 0)//比完发现没有移动(说明最大)
        {
            arr2[bit] = arr[i];
            bit++;
        }
        flag = 0;
    }
    for (int i = 0; i < SIZE; i ++) printf("%d  ", arr2[i]);
    return 0;
}


int quick(int arr[], int low, int high)//升序快速排序
{
    if (low >= high)
        return 0;
    int i = low,j = high;//i初始化为数组头，j初始化为数组尾
    
   int key = arr[low];//设   定数组头为key
    while (i < j)
    {
    while (j > i && arr[j] > key) j--;//右哨兵左移找比key更小的
    while (i < j && arr[i] <= key) i++;//左哨兵右移找大数
    if (i < j) 
    {
        int temp = arr[i];//exchange
        arr[i] = arr[j];
        arr[j] = temp;
    }
    }
        arr[low] = arr[i];
        arr[i] = key;
    
    quick(arr, low, i-1);
    quick(arr, i+1, high);
    return 0;
    //static int arr[10] = {1,2,10,5,2,8,19,8,6,9};//for quick
    //for (int l = 0; l< 10; l++) printf("%d  ", arr[l]);//for quick
}

//-----------------下边两个是堆排序-----------
void heap_set(int arr[], int length)//每次构造堆
{
    
    for (int i = length-1; i >0; i--) //i初始化为最后一个节点,直到arr的头（不一定是0
    {
        int parent = (i-1)/2;    //arr[parent]是arr[i]的爹
        if (arr[i] > arr[parent])//如果儿子大就交换
        {
            int temp = arr[i];
            arr[i] = arr[parent];
            arr[parent] = temp;//子父交换
        }
    }//循环过后最大的在arr[0]

    int temp = arr[0];
    arr[0] = arr[length-1];
    arr[length-1] = temp;//首尾交换,把最大的放到数组尾并不再参与排序

}

void heap(int arr[], int length)//升序堆排序
{
    
    for (int n = length; n >0 ; n--)
    {
        heap_set(arr, n);
    }
    for (int i = 0; i < length; i++)
    {
    printf("%d  ", arr[i]);
    }

}
//------------上面是堆排序------------
 //-----------下面是归并排序----------
 
void merge(int in[], int out[],int i,int m, int n)//i是arr1起点，m是arr1起点，n是arr2终点
 {

    int j = m+1;
    int k = i;//这点很重要，在一轮归并中k要可连续，所以要跟着i走
    while ( i <= m && j <=  n)
    {
        if ( in[i] < in[j])//比较两组，小的放进去并且指针右移
        out[k++] = in[i++];
        else
        out[k++] = in[j++];
    }
    while (i <= m) out[k++] = in[i++];
    while (j <= n) out[k++] = in[j++];
 }

int min(int a, int b) {return a<b?a:b;}
void cpy(int in[], int arr2[], int length)              //把arr2写入in里边(保存每次并归的结果)
{for (int i = 0; i< length; i++) in[i] = arr2[i];}


void merge_sort(int in[], int arr2[], int length)//第二个参数是空的
{
    
    int m, n, width = 1;
    for (;width < length; width *= 2)
    {
        for (int i = 0; i < length; i = i + width * 2)
        {
            m = min(i+width-1, length-1);//m和n都不能超过那个数组的范围
            n =  min(m+width, length -1);
            merge(in, arr2, i, m, n);
        }
        cpy(in, arr2, length);
    }
}

//-------------上面是归并------------
//----------计数(小桶)-----------, 
void count_sort(int arr[],int length) //max是arr数组里边的最大值(要求arr是非负整数集)
{
    int max = arr[0],min = arr[0];
    for (int i = 0; i < length; i++)
    {
        max = (arr[i]>max)?arr[i]:max;
        min = (arr[i]<min)?arr[i]:min;
    }//找最大和最小
    max++;

    int bucket[max];
    for (int i = min; i < max; i++) bucket[i] = 0;//把桶里的元素全部清零
    for (int i = 0; i < length; i++)  bucket[arr[i]]++; //桶里加料
    int out[length];
    int k = 0;

    for(int m = 0; m < max; m++)
    {
        for (int i = 0; i < bucket[m]; i++)
            out[k++] = m;
    }
    for (int i = 0; i< length; i++) printf("%d  ", out[i]);
}
//----------计数-----------
//----------大桶-----------
void bucket_sort(int arr[], int length)
{
    int max = arr[0],min = arr[0];
    for (int i = 0; i < length; i++)
    {
        max = (arr[i]>max)?arr[i]:max;
        min = (arr[i]<min)?arr[i]:min;
    }//找最大和最小
    int n = (max-min)/2;//默认length/2个桶
    int *bucket[n];
    for (int i = 0; i < n; i++) bucket[i] = 0;
    for (int i = 0; i < length; i++)
    {
        int k = arr[i]/2;//k是第几个桶
        bucket[k]++; //桶里加料
    }
    int out[length];
    int k = 0;

    for(int m = 0; m < max; m++)
    {
        for (int i = 0; i < bucket[m]; i++)
            out[k++] = m;
    }
    for (int i = 0; i< length; i++) printf("%d  ", out[i]);

}

int main()
{
    int arr [SIZE] = {1, 4, 2, 19, 27, 8, 2, 6,12, 7};
    return 0;
}