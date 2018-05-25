/*******************************************************************************
*
*版  权：
*
*文  件：
*
*描  述：
*
*设  计：通用C语言库
*
*作  者：
*
*修  改：2017.09
*
********************************************************************************
*/
#ifndef __COMMON_FUNC_H__
#define __COMMON_FUNC_H__

/*.BH-----------------------------------------------------------------
**                  包含文件
**.EH-----------------------------------------------------------------
*/
#include "stdio.h" /*库文件包含*/
#include "string.h" /*用于字符串操作*/
#include "stdlib.h"
#include "math.h" 
#include "ctype.h" 
#include "math.h"
//#include "tchar.h"
#include "DB_fault_status_word.h"
#include "DB_fault_report.h"


/*.BH-----------------------------------------------------------------
**                  宏定义
**.EH-----------------------------------------------------------------
*/
/***********************************************
                                        位操作
************************************************/
/*
大小端转化
#define  LITTLE_ENDIAN


#if defined BIG_ENDIAN && !defined LITTLE_ENDIAN


#define htons(A)  (A)  
#define htonl(A)  (A)  
#define ntohs(A)  (A)  
#define ntohl(A)  (A)  


#elif defined LITTLE_ENDIAN && !defined BIG_ENDIAN


#define htons(A)  (\
                   (((uint16_t)(A) & 0xff00) >> 8 )|\
                   (((uint16_t)(A) & 0x00ff) << 8 )\
                   )  
#define htonl(A)  (\
                   (((uint32_t)(A) & 0xff000000) >> 24)|\
                   (((uint32_t)(A) & 0x00ff0000) >> 8 )|\
				   (((uint32_t)(A) & 0x0000ff00) << 8 )|\
				   (((uint32_t)(A) & 0x000000ff) << 24)\
				   )  
#define ntohs     htons  
#define ntohl     htonl

#else  

#error Either BIG_ENDIAN or LITTLE_ENDIAN must be #defined, but not both.  

#endif
*/


/*用于判断float是否为0*/
#define EPS 0.0001


/*hash table max size*/
#define HASH_TABLE_MAX_SIZE 40
/*hash table大小*/
int hash_table_size;

/*.BH-----------------------------------------------------------------
**                 结构体定义
**.EH-----------------------------------------------------------------
*/
/*hashTable结构*/
typedef SysIndexs HashKeyType;
typedef struct{     
    OMS_TYPE__CurrFaultReport curr_fault_report;
    unsigned int begin_time[SYS_FAULT_REPORT_MAX_NUM];
    unsigned int end_time[SYS_FAULT_REPORT_MAX_NUM];
    unsigned int report_valid[SYS_FAULT_REPORT_MAX_NUM];
}HashValueType;
typedef struct HashNode_Struct HashNode;
struct HashNode_Struct
{
    HashKeyType sKey;
    HashValueType nValue;
    HashNode* pNext;
};
HashNode* hashTable[HASH_TABLE_MAX_SIZE]; //hash table data strcutrue




/*FIFO队列中参数的类型*/
typedef int QElemtype;
 /*对节点的结构定义*/
typedef struct QNode
{
    QElemtype data;
    struct QNode *next;
}QNode,*QueuePtr;
 
/*FIFO队列的结构定义*/
typedef struct{     
    QueuePtr head;
    QueuePtr rear;
}LinkQueue;
LinkQueue updateListMsgQ;


/*.BH-----------------------------------------------------------------
**                  函数声明
**.EH-----------------------------------------------------------------
*/

/***************************************************************
                                     方程计算
****************************************************************/

/**************************************************************
功能：
将当前已经完成运算的运算符消去，同时将数值数组的位置调整以进行下一次运算。
传入值p若为3
则当前符号的数组位置为3.
f[3]=f[3+1].......f[len-2]=f[len-1]  f[len-1]='\0';
s[i]=s[i+1].......s[len-1]=s[len]  因为数值比运算符多一个

输入参数：
char *f : 运算符数组
double *s: 数值数组
int p:  当前运算符数组位置。

返回参数:
无
**************************************************************/
void  Common__Move(char *f, float *s,int p);



/**************************************************************
功能：
根据操作数、操作符进行计算不等式结果。例如: 
输入{1,2,3}和{"+=="}，计算1+2==3，输出1
输入{1,2,1}和{"+=="}，计算1+2==1，输出0

输入参数：
double* number: 运算数数组
char* operate: 运算符数组

返回参数：
double number[0] : 1方程计算结果为真(故障)，0结果为假(正常)
**************************************************************/
float Common__Compute(float* number,char* operate);



/**************************************************************
功能：
将输入的字符串先将其小数点以前的部分复制到temp[]数组中，
若有小数点，则将小数点之后的数值，也就是小数部分先进行计算,值存入num中
计算完成后，再对整数部分进行计算，值加上小数部分的值，存入num中。

输入参数：
char *c :由数字和小数点组成的字符，用以转换成double型的数值。

返回参数：
num:返回转换好的值。
**************************************************************/
float Common__ConvNum(char *c);


/***************************************************************
                                      字符串操作
****************************************************************/

/*查找字符位置*/
int Common__FindCharPos(char* src,char dst);

/*从字符串的左边截取n个字符*/
char * Common__Left(char *dst,char *src, int n);

/*从字符串的中间截取n个字符*/
char * Common__Mid(char *dst,char *src, int n,int m);/*n为长度，m为位置*/

/*从字符串的右边截取n个字符*/
char * Common__Right(char *dst,char *src, int n);

/*位截取*/
unsigned long Common__GetBitValue(unsigned long U32data,int startpos,int endpos,int nsign);

/*字符串按_分割，截取倒数第二个字符串系统名称*/
const char *Common_Split(char *str, char split_char);
//char * Common_GetStringValue(char *src);

/*八进制转十进制*/
int Common_Dec2Oct(int dec_num);

/*十进制转八进制*/
int Common_Oct2Dec(int oct_num );

/*字符串转整型*/
int Common_Str2Int(const char *str, int len);

/*字符串转double*/
float Common_Str2Double(const char *str);

/*整型转字符串*/
char * Common_Int2Str(int num);

int Common_CharArrayCmp(char *s1, int l1, char *s2, int l2);

/*对一个字节进行位倒序*/
unsigned int bit_tern(unsigned int input);

/***************************************************************
                                      大小端转换操作
****************************************************************/
/*2字节的字节序转换*/
void swap2byte(unsigned short *addr) ;

/*4字节的字节序转换*/
void swap4byte(unsigned int *addr) ;


/***************************************************************
                                      hash表操作
****************************************************************/
//string hash function
unsigned int hash_table_hash_str(const char* skey);

//insert key-value into hash table
int hash_table_insert(const HashKeyType skey, HashValueType nvalue);

//lookup a key in the hash table
HashNode* hash_table_lookup(const HashKeyType skey);

//print the content in the hash table
void hash_table_print();

//free the memory of the hash table
void hash_table_release();

//remove key-value frome the hash table
void hash_table_remove(const HashKeyType skey);

/*初始化HashTable*/
void Common_InitHashTable();


/***************************************************************
                                     FIFO队列操作
****************************************************************/
int Queue_Init(LinkQueue* que);
int Queue_Destory(LinkQueue* que) ;
int Queue_Push(LinkQueue* que,QElemtype e);
int Queue_Pop(LinkQueue* que,QElemtype *t);
int Queue_View(LinkQueue* que);


/***************************************************************
                                     转码操作
****************************************************************/
/*Utf8->Unicode*/
int OMS_Utf8ToUnicode(char* pInput, char* pOutput);
/*Unicode->Gbk*/
int OMS_UnicodeToGbk(unsigned short *unicode_buf, char *gbk_buf, int max_gbk_buf_size);

/*Gbk->Unicode*/
int OMS_GbkToUnicode(char *gbk_buf, unsigned short *unicode_buf, int max_unicode_buf_size);
/*Unicode->Utf8*/
int OMS_UnicodeToUtf8(char* pInput, char *pOutput);


#endif
