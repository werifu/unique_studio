#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
 #include <arpa/inet.h>
#include<netinet/in.h>
 #include<string.h>
 #include<malloc.h>
 #include<sys/socket.h>
//#include"../dns/dns.h"
//存放dns查询相关结构与函数的头文件
void requestDNS(unsigned char* hostname, unsigned char* servername);

struct header {
    unsigned short id; //会话标识
    unsigned char rd :1; // 表示期望递归
    unsigned char tc :1; // 表示可截断的 
    unsigned char aa :1; //  表示授权回答
    unsigned char opcode :4; 
    unsigned char qr :1; //  查询/响应标志，0为查询，1为响应
    unsigned char rcode :4; //应答码
    unsigned char cd :1; 
    unsigned char ad :1; 
    unsigned char z :1; //保留值
    unsigned char ra :1; // 表示可用递归
    unsigned short q_count; // 表示查询问题区域节的数量 
    unsigned short ans_count; // 表示回答区域的数量
    unsigned short auth_count; // 表示授权区域的数量
    unsigned short add_count; // 表示附加区域的数量
};


struct question
{
    unsigned short qtype;
    unsigned short qclass;
};
typedef struct 
{
    unsigned char* qname;
    struct question* question;
}query;
/*---------------上边是对报头和询问的定义------------------*/

//编译制导命令
//#pragma pack(push, 1)//保存对齐状态，设定为1字节对齐
struct r_data {
    unsigned short type; //表示资源记录的类型
    unsigned short _class; //类
    unsigned int ttl; //表示资源记录可以缓存的时间
    unsigned short data_len; //数据长度
};
//#pragma pack(pop) //恢复对齐状态

//
struct res_record{
    unsigned char* name;
    struct r_data* r_data;
    unsigned char* record_data;
};
/*------------上边是对回答的定义-----------*/


//buff的内容是[header,qname,question]
unsigned char buff[65535];

struct header*  set_header(struct header *dns)//初始化dns查询的头部
 {
    dns = (struct header*)malloc(sizeof(struct header)); 
    dns->qr = 0; //query or responce(0是前者)
    dns->opcode = 0; //标准询问
    dns->aa = 0; //无授权
    dns->tc = 0; //不截断
    dns->rd = 1; 
    dns->ra = 0; 
    dns->z = 0;
    dns->ad = 0;
    dns->cd = 0;
    dns->rcode = 0;
    dns->q_count = htons(1); //只有一个问题
    dns->ans_count = 0;
    dns->auth_count = 0;
    dns->add_count = 0;
    return dns;
 }




/*域名需要经过简单编码，在每一串字母（.为分界线)前需要有数字来表示后边这一串字母有多少字节
比如baidu.com 应该被改编成
5baidu3com
最后以0结尾*/
void change_format(unsigned char* dns, unsigned char* host)
{
    int lock = 0, i;
    strcat((char*) host, ".");

    for (i = 0; i < strlen((char*) host); i++) {
        if (host[i] == '.') {
            *dns++ = i - lock;
            for (; lock < i; lock++) {
                *dns++ = host[lock];
            }
            lock++; 
        }
    }
    *dns++ = '\0';
}

int main(int argc, char* argv[])
 {
     unsigned char* hostname, *servername;
    if (argc == 2)
    {
        hostname = argv[1];
        requestDNS(hostname, (unsigned char*)"8.8.8.8");
    }
    else if (argv[2][0] == '@')
    {
        hostname = argv[1];
        unsigned char* de_at;
        int m = 0;
        do
        {
            de_at[m] = argv[2][m+1];
            m++;
        } while (argv[2][m+1] != '\0');
        
        servername = de_at;//把'@'删掉
        requestDNS(hostname, servername);
    }

    return 0;
 }

void requestDNS(unsigned char* hostname, unsigned char* servername)
{
    
    int  dnsSock = socket(AF_INET,SOCK_DGRAM, IPPROTO_UDP);//sock_dgram基于UDP传输
    //储存目标IP/端口等信息
    struct sockaddr_in dnsaddr;
    int i = sizeof(dnsaddr);
    dnsaddr.sin_family = AF_INET;
    dnsaddr.sin_addr.s_addr = inet_addr((const char*)servername);
    dnsaddr.sin_port = htons(53);

    struct header *dns = NULL;
    dns = (struct header*) buff;
    dns->qr = 0; //query or responce(0是前者)
    dns->opcode = 0; //标准询问
    dns->aa = 0; //无授权
    dns->tc = 0; //不截断
    dns->rd = 1; 
    dns->ra = 0; 
    dns->z = 0;
    dns->ad = 0;
    dns->cd = 0;
    dns->rcode = 0;

    dns->q_count = htons(1); //只有一个问题
    dns->ans_count = 0;
    dns->auth_count = 0;
    dns->add_count = 0;


    struct question *qinfo = NULL;
    unsigned char* qname;
    qname = (unsigned char*) &buff[sizeof(struct header)];
    change_format(qname, hostname);
    qinfo = (struct question*) &buff[sizeof(struct header) 
    + (strlen((const char*) qname) + 1)]; //qinfo指向问题查询区域的查询类型字段


    struct question* dnsquery = (struct question*) &buff[sizeof(struct header) + (strlen((const char*) qname) + 1)];
    dnsquery->qtype = htons(1);//要查的是A记录
    dnsquery->qclass = htons(1);//上边三行初始化quetion部分

    //缓冲区长度
    int buff_length = (sizeof(struct header)+strlen((const char*)qname)+sizeof(struct question)+1);
    //发送udp包
    if (sendto(dnsSock,buff, buff_length, 0, (struct sockaddr*) &dnsaddr, sizeof(dnsaddr)) < 0) {
       printf("sendto failed\n");
    }
    printf("sendto successfully\n");

    if (recvfrom(dnsSock, (char*)buff, 65535, 0, (struct sockaddr*)&dnsaddr, (socklen_t*)&i +1 )<0){
        perror("recvfrom failed\n");
    }
    printf("receive successfully\n");
    //reader指向回答部分

    dns = (struct header*) buff;

    //将reader指向接收报文的回答区域 
    unsigned char *reader = &buff[buff_length];
    /*
    printf("\n\n响应报文包含: ");
    printf("\n %d个问题", ntohs(dns->q_count));
    printf("\n %d个回答", ntohs(dns->ans_count));
    printf("\n %d个授权服务", ntohs(dns->auth_count));
    printf("\n %d个附加记录\n\n", ntohs(dns->add_count));
*/


    /*下面开始解析受到的回答*///下边算是抄的。（理解不能//对怎么解析真的没有思路

    struct res_record answers[20], auth[20], addit[20];//回答区域、授权区域、附加区域中的资源数据字段

    reader = reader + sizeof(short);//short类型长度为32位，相当于域名字段长度，这时reader指向回答区域的查询类型字段
    answers[i].r_data = (struct r_data*) (reader);
    reader = reader + sizeof(struct r_data);//指向回答问题区域的资源数据字段
    if (ntohs(answers[i].r_data->type) == 1) //判断资源类型是否为IPv4地址
    {
        answers[i].record_data = (unsigned char*) malloc(ntohs(answers[i].r_data->data_len));//资源数据
    for (int j = 0; j < ntohs(answers[i].r_data->data_len); j++) 
    {
        answers[i].record_data[j] = reader[j];
    }
    answers[i].record_data[ntohs(answers[i].r_data->data_len)] = '\0';
    reader = reader + ntohs(answers[i].r_data->data_len);
    }


    //显示查询结果
    if (ntohs(answers[i].r_data->type) == 1) //判断查询类型IPv4地址
    {
        long *p;
        p = (long*) answers[i].record_data;
        dnsaddr.sin_addr.s_addr = *p; 
        printf("IPv4地址:%s\n", inet_ntoa(dnsaddr.sin_addr));
    }
    
    return;

}

 
