/*******************************************************************************
*
*版  权：
*
*文  件：
*
*描  述：
*
*设  计：公共库
*
*作  者：
*
*修  改：2017.09
*
********************************************************************************
*/


/*.BH-----------------------------------------------------------------
**                  包含文件
**.EH-----------------------------------------------------------------
*/
#include "common_func.h"
#include "fault_diagnosis.h"

#include "code_unicodeTables.h"
#include "code_gbkTables.h"


/*.BH-----------------------------------------------------------------
**
**函数名：Common__Move
**
**功能：一次计算后的移位
**
**参数： 无
**
**返回值：无
**
**设计注记：
**
**.EH-----------------------------------------------------------------
*/
void  Common__Move(char *f, float *s,int p)  
{
	int i=0;
	int len = 0;
	len=strlen(f);
	for(i=p; i<len; i++)   /*将已经运算过的符号，空出来的位置用后面的符号来填充，*/      
	{      /*即把乘和除号的位置用后面的加和减号填充*/
		f[i]=f[i+1];
		s[i]=s[i+1];
	}
	s[i]=s[i+1];
	f[len-1]='\0';
}

/*.BH-----------------------------------------------------------------
**
**函数名：Common__Compute
**
**功能：根据故障方程操作数和操作符计算结果
**
**参数： number操作数，operate操作符
**
**返回值：返回值为1，方程成立，为0则不成立
**
**设计注记：
**
**.EH-----------------------------------------------------------------
*/
float Common__Compute(float* number,char* operate)  /*可递归函数*/
{     
	/******************************变量定义***********************/
	int num=0;
	int i=0; /*num保存新括号内的字符数,i 保存number里的字符位置*/
	int strleng = 0;
	int fstart = 0;
	int leftbracketpos = 0;
	int rightbracketpos = 0;
	int nbracketstate = 0;/*用于判断括号的对数（）是否成对出现*/
	int tempnum = 0;
	//int operatenumber = 0;
	int operatenumsum = 0;
	//int numcount = 0;
	//int numcount1 = 0;
	int operateindex = 0;
	int shiftnum = 0;
	//int ncount = 0;
	int numbergross = 1;/*括号内数的的总数*/
	int ncounter = 0;/*计数器*/
	int ncounter1 =0;
	int ncutnumpos = 0;/*截取数据的开始位置*/
	char tempf[80];
	float temps[80];
	/*******************************计算********************************/
	if((number == NULL)||(operate == NULL))
	{
		printf("Common__Compute: number or operate is NULL.\n");
		return 0;
	}
	while (operate[operateindex]!='\0')
	{
		switch (operate[operateindex])
		{
		case '(':
			nbracketstate++; /*一个新的字符表达式进行递归调用good函数计算。*/
			while(nbracketstate>0)
			{

				operatenumsum++;/*包括括号内所有的运算符号*/
				operateindex++;
				/*获取括号内计算用的计算符数组*/
				tempf[num]=operate[operateindex];
				num++;
				rightbracketpos++;

				if(operate[operateindex]==')')             
				{
					nbracketstate--;
				}
				else if(operate[operateindex]=='(')
				{
					nbracketstate++;

				}
				else if(
					(operate[operateindex-1]!='=')&&
					(operate[operateindex-1]!='!')&&
					(operate[operateindex-1]!='>')&&
					(operate[operateindex-1]!='<')&&
					(operate[operateindex-1]!='&')&&
					(operate[operateindex-1]!='|')
					)
				{
					/*括号操作数字的个数*/
					numbergross++;

				}
			}
			tempf[num-1]='\0';
			/*获取括号内计算用的计算数据数组*/
			ncounter = numbergross;
			ncounter1 = ncutnumpos;
			while (ncounter>0)
			{

				temps[tempnum] = number[ncounter1];
				ncounter1++;
				tempnum++;
				ncounter--;
			}
			num=0;/*完成一个括号内容的复制，其位置指针num=0*/
			/*在总的数据数组中去掉将要计算所使用的数*/
			ncounter = 0;
			while (ncounter<(numbergross-1))
			{
				strleng = strlen(operate);
				for(fstart =shiftnum; fstart<strleng;fstart++)
				{
					number[fstart]=number[fstart+1];
				}
				number[fstart]=number[fstart+1];
				ncounter++;

			}
			/*去掉左括号和右括号之间的运算符*/
			ncounter = leftbracketpos;/*leftbracketpos为左括号位置*/
			while (ncounter<=rightbracketpos)
			{
				strleng = strlen(operate);
				for(fstart =leftbracketpos; fstart<strleng;fstart++)
				{
					operate[fstart]=operate[fstart+1];
				}
				operate[strleng-1]='\0';
				ncounter++;

			}

			number[shiftnum] = Common__Compute(temps,tempf);


			memset(temps,0,sizeof(float)*80);
			memset(tempf,0,sizeof(char)*80);
			tempnum = 0;
			numbergross = 1;
			ncutnumpos = 0;
			operateindex = 0;
			shiftnum = 0;
			leftbracketpos = 0;
			rightbracketpos = 0;
			break;
		default:

			ncutnumpos++;
			leftbracketpos++;
			rightbracketpos++;
			operateindex++;
			shiftnum++;
			if(
				((operate[operateindex-1] == '&')&&(operate[operateindex] == '&'))||
				((operate[operateindex-1] == '|')&&(operate[operateindex] == '|'))||
				((operate[operateindex-1] == '!')&&(operate[operateindex] == '='))||
				((operate[operateindex-1] == '=')&&(operate[operateindex] == '='))||
				((operate[operateindex-1] == '>')&&(operate[operateindex] == '='))||
				((operate[operateindex-1] == '<')&&(operate[operateindex] == '='))||
				((operate[operateindex-1] == '<')&&(operate[operateindex] == '<'))||
				((operate[operateindex-1] == '>')&&(operate[operateindex] == '>'))
				)
			{
				ncutnumpos--;
				shiftnum--;
			}
			break;


		}

	}

	/*计算乘除*/
	i=0;
	while(operate[i]!='\0')
	{
		switch(operate[i])
		{
		case '*': 
			number[i+1]=number[i]*number[i+1];
			Common__Move(operate,number,i);
			break;
		case '/': 
			number[i+1]=number[i]/(float)number[i+1];
			Common__Move(operate,number,i);
			break;
		case '%': 
			number[i+1]=(int)number[i]%(int)number[i+1];
			Common__Move(operate,number,i);
			break;
		default:
			i++;
			break;
		}
	}
	/*计算加减*/
	i=0;
	while(operate[i]!='\0')
	{
		switch(operate[i])
		{
		case '+': number[i+1]=number[i]+number[i+1];
			Common__Move(operate,number,i);
			break;
		case '-': number[i+1]=number[i]-number[i+1];
			Common__Move(operate,number,i);
			break;
		default:
			i++;
			/*printf("operator error!");*/
			break;
		}
	}
	/*计算与或非*/
	i = 0;
	while (operate[i]!='\0')
	{
		switch (operate[i])
		{
		case '=':
			if(operate[i+1] == '=')
			{
				//number[i+1]=(number[i]==number[i+1]);
				if(fabs(number[i] - number[i+1]) < EPS)
					number[i+1]=1.0;
				else
					number[i+1]=0.0;
				strleng = strlen(operate);
				for(fstart = 0;fstart<(strleng-1);fstart++)
				{
					operate[i]=operate[i+1];
				}
				operate[strleng-1]='\0';
			}
			Common__Move(operate,number,i);
			break;
		case '>':
			if(operate[i+1] == '=')
			{
				//number[i+1]=(number[i]>=number[i+1]);
				if(number[i]>=number[i+1])
					number[i+1]=1;
				else
					number[i+1]=0;
				strleng = strlen(operate);
				for(fstart = 0;fstart<(strleng-1);fstart++)
				{
					operate[i]=operate[i+1];
				}
				operate[strleng-1]='\0';
			}
			else if(operate[i+1] == '>')
			{
				number[i+1]=((unsigned long)number[i]>>(unsigned long)number[i+1]);//
				strleng = strlen(operate);
				for(fstart = 0;fstart<(strleng-1);fstart++)
				{
					operate[i]=operate[i+1];
				}
				operate[strleng-1]='\0';
			}
			else
			{
				//number[i+1]=(number[i]>number[i+1]);
				if(number[i]>number[i+1])
					number[i+1]=1;
				else
					number[i+1]=0;
			}
			Common__Move(operate,number,i);
			break;

		case '<':
			if(operate[i+1] == '=')
			{
				//number[i+1]=(number[i]<=number[i+1]);
				if(number[i]<=number[i+1])
					number[i+1]=1;
				else
					number[i+1]=0;
				strleng = strlen(operate);
				for(fstart = 0;fstart<(strleng-1);fstart++)
				{
					operate[i]=operate[i+1];
				}
				operate[strleng-1]='\0';

			}
			else if (operate[i+1] == '<')
			{
				number[i+1]=((unsigned long)number[i]<<(unsigned long)number[i+1]);
				strleng = strlen(operate);
				for(fstart = 0;fstart<(strleng-1);fstart++)
				{
					operate[i]=operate[i+1];
				}
				operate[strleng-1]='\0';
			}
			else
			{
				//number[i+1]=(number[i]<number[i+1]);
				if(number[i]<number[i+1])
					number[i+1]=1;
				else
					number[i+1]=0;
			}
			Common__Move(operate,number,i);
			break;
		case '!':
			if(operate[i+1] == '=')
			{
				//number[i+1]=(number[i]!=number[i+1]);
				if(fabs(number[i] - number[i+1]) > EPS)
					number[i+1]=1;
				else
					number[i+1]=0;
				strleng = strlen(operate);
				for(fstart = 0;fstart<(strleng-1);fstart++)
				{
					operate[i]=operate[i+1];
				}
				operate[strleng-1]='\0';
			}
			else
			{
				//number[i+1]=(!number[i+1]);
				if(fabs(number[i+1]) < EPS)
					number[i+1]=1;
				else
					number[i+1]=0;
			}
			Common__Move(operate,number,i);
			break;
		case '&':
			if(operate[i+1] == '&')
			{
				//number[i+1]=(number[i]&&number[i+1]);
				if((fabs(number[i]) > EPS)&& (fabs(number[i+1]) > EPS))
					number[i+1]=1;
				else
					number[i+1]=0;
				strleng = strlen(operate);
				for(fstart = 0;fstart<(strleng-1);fstart++)
				{
					operate[i]=operate[i+1];
				}
				operate[strleng-1]='\0';
			}
			else
			{
				//number[i+1]=((int)number[i]&(int)number[i+1]);
				if((int)number[i]&(int)number[i+1])
					number[i+1]=1;
				else
					number[i+1]=0;

			}
			Common__Move(operate,number,i);
			break;
		case '|':
			if(operate[i+1] == '|')
			{
				//number[i+1]=(number[i]||number[i+1]);
				if((fabs(number[i]) > EPS)||(fabs(number[i+1]) > EPS))
					number[i+1]=1;
				else
					number[i+1]=0;
				strleng = strlen(operate);
				for(fstart = 0;fstart<(strleng-1);fstart++)
				{
					operate[i]=operate[i+1];
				}
				operate[strleng-1]='\0';
			}
			else
			{
				//number[i+1]=((int)number[i]|(int)number[i+1]);
				if((int)number[i]|(int)number[i+1])
					number[i+1]=1;
				else
					number[i+1]=0;
			}
			Common__Move(operate,number,i);
			break;
		default:
			break;
		}

	}
	return number[0];
}



/*.BH-----------------------------------------------------------------
**
**函数名：Common__Left
**
**功能：从字符串的左边截取n个字符
**
**参数： 无
**
**返回值：无
**
**设计注记：
**
**.EH-----------------------------------------------------------------
*/
char * Common__Left(char *dst,char *src, int n)
{
	char *p = src;
	char *q = dst;
	int len = strlen(src);
	if(src==NULL)
		return NULL;
	if(n>len)
		n = len;
	while(n--)
	{
		*(q++) = *(p++);
	}
	*(q++)='\0'; /*有必要吗？很有必要*/
	return dst;
}

/*.BH-----------------------------------------------------------------
**
**函数名：
**
**功能：从字符串的中间截取n个字符
**
**参数： 无
**
**返回值：无
**
**设计注记：
**
**.EH-----------------------------------------------------------------
*/
char * Common__Mid(char *dst,char *src, int n,int m) /*n为长度，m为位置*/
{
	char *p = src;
	char *q = dst;
	int len = strlen(src);
	if(n>len)
		n = len-m;    /*从第m个到最后*/
	if(m<0)
		m=0;    /*从第一个开始*/
	if(m>len)
		return NULL;
	p += m;
	while(n--)
	{
		*(q++) = *(p++);
	}
	*(q++)='\0'; /*有必要吗？很有必要*/
	return dst;
}

/*.BH-----------------------------------------------------------------
**
**函数名：
**
**功能：从字符串的右边截取n个字符
**
**参数： 无
**
**返回值：无
**
**设计注记：
**
**.EH-----------------------------------------------------------------
*/
char * Common__Right(char *dst,char *src, int n)
{
	char *p = src;
	char *q = dst;
	int len = strlen(src);
	if(n>len) n = len;
	p += (len-n);   /*从右边第n个字符开始*/
	while(*(q++) = *(p++));
	return dst;
}

/*.BH-----------------------------------------------------------------
**
**函数名：
**
**功能：查找字符位置
**
**参数： 无
**
**返回值：无
**
**设计注记：
**
**.EH-----------------------------------------------------------------
*/
int Common__FindCharPos(char* src,char dst)
{
	int pos = 0;
	char* tempsrc = src;
	while (*tempsrc != '\0')
	{
		if(*tempsrc == dst)
		{
			return pos;
		}
		else
		{
			pos++;
			tempsrc++;
		}

	}
	printf("dont have char that you want to get\n");
	
	return -1;

}


/*.BH-----------------------------------------------------------------
**
**函数名：
**
**功能：获取32位数中某几位的值
**
**参数： 无
**
**返回值：无
**
**设计注记：
**
**.EH-----------------------------------------------------------------
*/
unsigned long Common__GetBitValue(unsigned long U32data,int startpos,int endpos,int nsign)
{
	unsigned long l_tempdata = 0;
	l_tempdata = (U32data<<(32-endpos))>>(32-(endpos-startpos+1));
	if(nsign == 1)
	{
		l_tempdata = (-1)*l_tempdata; 
	}
	return l_tempdata;	
}

/*.BH-----------------------------------------------------------------
**
**函数名：
**
**功能：获取系统名称，以_为分隔符获取倒数第2个字符串，例如aa_bb_cc，返回bb
**
**参数： 无
**
**返回值：无
**
**设计注记：占用空间有点多，可以设计改进
**
**.EH-----------------------------------------------------------------
*/
const char *Common_Split(char *str, char split_char)
{
	char buf[32]={0};//每个字符串最大32字节
	int str_id = 0;
	int arr_id = 0;
	int buf_id = 0;
	int buf_len = 0;
	int str_len = strlen(str);
	char array[10][32]={0};//最大10个字符串，每串32个字符

	for(str_id=0,arr_id=0,buf_id=0; str_id<str_len; str_id++)
	{
		if(str[str_id] != split_char)
		{
			buf[buf_id++] = str[str_id];
		}
		else//找到字符
		{
			buf_len = strlen(buf);
			if(buf_len > 0)
			{
				memcpy(array[arr_id], buf, buf_len);
				//strcpy(array[arr_id], buf);
				arr_id++;
				memset(buf, 0x0, buf_len);
				buf_id=0;
				continue;
			}
			else{
				continue;
			}
		}
		if(str_id == str_len-1)//最后一个字符串
		{
			buf_len = strlen(buf);
			if(buf_len > 0)
			{
				memcpy(array[arr_id], buf, buf_len);
				arr_id++;
				memset(buf, 0x0, buf_len);
			}
		}
	}
	/*for(i=0; i<arr_id; i++)
		printf("%d: %s,%s\n",i,array[i],array[arr_id-2]);*/
	return array[arr_id-2];
}


/*对一个字节进行位倒序*/
unsigned int bit_tern(unsigned int input)
{
	unsigned int output=0,i,temp;  
	for(i=0;i<8;i++)
	{  
		temp = input&0x01;
		input=input>>1;  
		output=output+temp;  
		if(i != 7)  
			output=output<<1;  
	}  
	return output;
}

#if 0
char * Common_GetStringValue(char *src)
{
	const char *split = "_";
	char *dst_tmp[10]={NULL};
	char *p=NULL;
	char *result = NULL;
	int i=0;
	int char_pos=0;
	
	p = strtok(src,split);
	while(p!=NULL)
	{
		dst_tmp[i++]=p;
		p=strtok(NULL,split);
	}
	return dst_tmp[i-2];
}
#endif

/*.BH-----------------------------------------------------------------
**
**函数名：
**
**功能：将输入的字符串先将其小数点以前的部分复制到temp[]数组中，
**若有小数点，则将小数点之后的数值，也就是小数部分先进行计算,值存入num中
**计算完成后，再对整数部分进行计算，值加上小数部分的值，存入num中。
**
**参数： char *c :由数字和小数点组成的字符，用以转换成float型的数值。
**
**返回值：num:返回转换好的值
**
**设计注记：
**
**.EH-----------------------------------------------------------------
*/
float Common__ConvNum(char *c)
{
	float num=0.0;
	float a=1.0;
	int i=0,p=0,len=0;
	char temp[100];
	int tempi=0;
	int start=0;
	int f=1;   /*正负符号指示器，若为1则为正数，为－1，此数为负数*/

	len=strlen(c);

	if(c[0]=='-')
	{
		start=1;
		f=-1;
	}
	for(i=start; i<len; i++)
	{
		if(c[i]=='.') 
		{
			p=i;
			break;
		}
		temp[tempi++]=c[i];  /*将整数部分复制到temp[]中*/
	}
	temp[tempi]='\0';

	if(p!=0)
	{
		for(i=p+1;i<len;i++) /*将小数部分计算出来*/
		{
			if(c[i]=='.')  /*如果有多余的小数点，则表示输入错误*/
			{
				printf("there is more that one dot '.' in number!error!\n");
				exit(0);
			}
			a=a*0.1;
			num+=(a*(c[i]-48));
		}
	}
	a=1.0;
	len=strlen(temp);           /*计算整数部分*/
	for(i=len-1;i>=0; i--)
	{
		num=num+(a*(temp[i]-48));
		a*=10;
	}
	num=num*f;
	return num;
}



/*.BH-----------------------------------------------------------------
**
**函数名：
**
**功能：八进制转十进制
**
**参数： 无
**
**返回值：无
**
**设计注记：
**
**.EH-----------------------------------------------------------------
*/
int Common_Dec2Oct(int dec_num )
{
	int oct_num=0;
	char tmp_buffer[10]={0};
	sprintf(tmp_buffer,"%o",dec_num);
	sscanf(tmp_buffer,"%d",&oct_num);

	return oct_num;
}


/*.BH-----------------------------------------------------------------
**
**函数名：
**
**功能：十进制转八进制
**
**参数： 无
**
**返回值：无
**
**设计注记：
**
**.EH-----------------------------------------------------------------
*/
int Common_Oct2Dec(int oct_num )
{
	int dec_num=0;
	char tmp_buffer[10]={0};
	sprintf(tmp_buffer,"%d",oct_num);
	sscanf(tmp_buffer,"%o",&dec_num);

	return dec_num;
}

/*.BH-----------------------------------------------------------------
**
**函数名：
**
**功能：字符串转整型代替atoi
**
**参数： 无
**
**返回值：无
**
**设计注记：
**
**.EH-----------------------------------------------------------------
*/
int Common_Str2Int(const char *str, int len)
{
	const char *ptr = str; //ptr保存str开头
	int out=0;
	int i;
	if(*str == '-' || *str == '+')//如果第一个字符为正负号
	{
		str++;
	}
	for(i=0; (i<len)&&(*str != '\0'); i++)
	{
		if((*str < '0') || (*str > '9'))//如果不是数字
		{
			printf("Str2Int ERROR: not number\n");
			break;
		}
		out = out*10 + (*str - '0');
		str++;
	}
	if(*ptr == '-')
	{
		out = -out;
	}
	return out;
}
#if 0
/*方法二*/
int Common_Str2Int(const char *str_int, int len)
{
	if(str == NULL)
		return -1;
	int out_int=0;
	 if(EOF == sscanf(str_int, "%d", &out_int))
	 {
	 	logMsg("Common_Str2Int ERROR: %d\n",str_int,2,3,4,5,6);
		return -1;
	 }
	 return out_int;
}
#endif


/*整型转字符串
char * Common_Int2Str(int num)
{
	char str_int[10]={0};
	sprintf(str_int, "%d", num);
	return str_int;
}*/

/*.BH-----------------------------------------------------------------
**
**函数名：
**
**功能：字符串转float
**
**参数： 无
**
**返回值：无
**
**设计注记：
**
**.EH-----------------------------------------------------------------
*/
float Common_Str2Double(const char *str)
{
	float out=0.0;
	if(str == NULL)
		return 0.0;
	if(EOF == sscanf(str, "%f", &out))
	{
	 	printf("Common_Str2Double ERROR: %s\n",str);
		return 0.0;
	}
	return out;
}

/*字符串比较*/
int Common_CharArrayCmp(char *s1, int l1, char *s2, int l2)
{
    int lmin = l1>l2? l2:l1; //较小长度者。
    int i;
     
    for(i = 0; i < lmin; i ++)
        if(s1[i] > s2[i]) return 1;
        else if(s1[i] < s2[i]) return -1;
     
    //运行到这里，表示所有已判断元素均相等。
    if(l1 == l2) return 0;
    if(l1 > l2) return 1;
     
    return -1;//l1 < l2的情况。
}


/*=================hash table function======================*/
/*.BH-----------------------------------------------------------------
**
**函数名：
**
**功能：string hash function
**
**参数： 无
**
**返回值：无
**
**设计注记：
**
**.EH-----------------------------------------------------------------
*/
unsigned int hash_table_hash_str(const char* skey)
{
    const signed char *p = (const signed char*)skey;
    unsigned int h = *p;
    if(h)
    {
        for(p += 1; *p != '\0'; ++p){
            h = (h << 5) - h + *p;
        	}
    }
    return h;
}


/*.BH-----------------------------------------------------------------
**
**函数名：
**
**功能：insert key-value into hash table
**
**参数： 无
**
**返回值：无
**
**设计注记：
**
**.EH-----------------------------------------------------------------
*/
int hash_table_insert(const HashKeyType skey, HashValueType nvalue)
{
	unsigned int pos = 0;
	HashNode* pHead = NULL;
	HashNode* pNewNode = NULL;

	if (hash_table_size >= HASH_TABLE_MAX_SIZE)
	{
		printf("out of hash table memory!\n");
		return 0;
	}

	pos = skey % HASH_TABLE_MAX_SIZE;
	pHead = hashTable[pos];
	while (pHead)
	{
		if (pHead->sKey == skey)
		{
			printf("hash_table_insert: key %d already exists!\n", skey);
			return 0;
		}
		pHead = pHead->pNext;
	}

	pNewNode = (HashNode*)malloc(sizeof(HashNode));
	memset(pNewNode, 0, sizeof(HashNode));
	pNewNode->sKey = skey;
	memcpy(&pNewNode->nValue, &nvalue, sizeof(HashValueType));

	pNewNode->pNext = hashTable[pos];
	hashTable[pos] = pNewNode;


	hash_table_size++;
	return 1;
}

/*.BH-----------------------------------------------------------------
**
**函数名：
**
**功能：lookup a key in the hash table
**
**参数： 无
**
**返回值：无
**
**设计注记：
**
**.EH-----------------------------------------------------------------
*/
HashNode* hash_table_find(const HashKeyType skey)
{
	unsigned int pos = 0;
	
	pos = skey % HASH_TABLE_MAX_SIZE;
	if (hashTable[pos])
	{
		HashNode* pHead = hashTable[pos];
		while (pHead)
		{
			if (skey == pHead->sKey)
				return pHead;
			pHead = pHead->pNext;
		}
	}
	return NULL;
}

/*.BH-----------------------------------------------------------------
**
**函数名：
**
**功能：free the memory of the hash table
**
**参数： 无
**
**返回值：无
**
**设计注记：
**
**.EH-----------------------------------------------------------------
*/
void hash_table_release()
{
	int i;
	for (i = 0; i < HASH_TABLE_MAX_SIZE; ++i)
	{
		if (hashTable[i])
		{
			HashNode* pHead = hashTable[i];
			while (pHead)
			{
				HashNode* pTemp = pHead;
				pHead = pHead->pNext;
				if (pTemp)
				{
					free(pTemp);
				}

			}
		}
	}
}

//remove key-value frome the hash table
/*.BH-----------------------------------------------------------------
**
**函数名：
**
**功能：string hash function
**
**参数： 无
**
**返回值：无
**
**设计注记：
**
**.EH-----------------------------------------------------------------
*/
void hash_table_remove(const HashKeyType skey)
{
	unsigned int pos = skey % HASH_TABLE_MAX_SIZE;
	if (hashTable[pos])
	{
		HashNode* pHead = hashTable[pos];
		HashNode* pLast = NULL;
		HashNode* pRemove = NULL;
		while (pHead)
		{
			if (skey == pHead->sKey)
			{
				pRemove = pHead;
				break;
			}
			pLast = pHead;
			pHead = pHead->pNext;
		}
		if (pRemove)
		{
			if (pLast)
				pLast->pNext = pRemove->pNext;
			else
				hashTable[pos] = NULL;

			free(pRemove);
		}
	}
	hash_table_size--;
}

/*.BH-----------------------------------------------------------------
**
**函数名：
**
**功能：print the content in the hash table
**
**参数： 无
**
**返回值：无
**
**设计注记：
**
**.EH-----------------------------------------------------------------
*/
void hash_table_print()
{
	int i;
	printf("===========content of hash table===========\n");
	for (i = 0; i < HASH_TABLE_MAX_SIZE; ++i){
		if (hashTable[i])
		{
			HashNode* pHead = hashTable[i];
			printf("%d=>", i);
			while (pHead)
			{
				printf("%d:%d  ", pHead->sKey, pHead->nValue.begin_time);
				pHead = pHead->pNext;
			}
			printf("\n");
		}
	}
}

/*.BH-----------------------------------------------------------------
**
**函数名：
**
**功能：初始化系统名称的hashTable，插入所有系统名称
**
**参数： 无
**
**返回值：无
**
**设计注记：
**
**.EH-----------------------------------------------------------------
*/
void Common_InitHashTable()
{
	
    hash_table_size = 0;
    memset(hashTable, 0, sizeof(HashNode*) * HASH_TABLE_MAX_SIZE);
}


/*.BH-----------------------------------------------------------------
**
**函数名：
**
**功能：2字节的字节序转换
**
**参数： 无
**
**返回值：无
**
**设计注记：
**
**.EH-----------------------------------------------------------------
*/
void swap2byte(unsigned short *addr) 
{
 	unsigned short tmp = *addr;
	unsigned short tmp1, tmp2;
	tmp1 = tmp & 0xff;
	tmp2 = (tmp >> 8) & 0xff;
	*addr = (tmp1 << 8) | (tmp2);
}

/*.BH-----------------------------------------------------------------
**
**函数名：
**
**功能：4字节的字节序转换
**
**参数： 无
**
**返回值：无
**
**设计注记：
**
**.EH-----------------------------------------------------------------
*/
void swap4byte(unsigned int *addr) 
{
	unsigned int tmp = *addr;
	unsigned int tmp1, tmp2, tmp3, tmp4;
	tmp1 = tmp & 0xff;
	tmp2 = (tmp >> 8) & 0xff;
	tmp3 = (tmp >> 16) & 0xff;
	tmp4 = (tmp >> 24) & 0xff;
	*addr = (tmp1 << 24) | (tmp2 << 16) | (tmp3 << 8) | (tmp4);
}


/*.BH-----------------------------------------------------------------
**
**函数名：
**
**功能：链表实现消息队列，支持任意类型数据
**
**参数： 
**
**返回值：
**
**设计注记：
**
**.EH-----------------------------------------------------------------
*/
 //初始化队列
int Queue_Init(LinkQueue* que)
{
    que->head=que->rear=(QueuePtr)malloc(sizeof(QNode));
    if(!que->head)  //这段代码对队列里面的用户自定义数据类型进行了初始化
        return ERROR;
    return OK;
}
 
//回收队列
int Queue_Destory(LinkQueue* que) 
{
    while(que->head)
    {
        que->rear = que->head->next;
        free(que->head);
        que->head=que->rear;
    }
    return OK;
}

 /*将元素插到尾部*/
int Queue_Push(LinkQueue* que,QElemtype e)
{
    QueuePtr p = (QueuePtr)malloc(sizeof(QNode));
    if(!p) //若未能申请到空间，便退出
        return ERROR;
    p->data=e;
    p->next=NULL;

    que->rear->next = p;
    que->rear=p;
    return OK;
}

 /*指针que指向头节点的后一节点，既要出队的节点*/
int Queue_Pop(LinkQueue* que,QElemtype *t)
{
    if(que->rear==que->head)
        return ERROR; //队列为空

    QueuePtr p = que->head->next;
    *t=p->data;

    que->head->next=p->next;
    if(que->rear==p) //这个判断是 确保在清空队列的时候，让rear指针归位。
        que->rear=que->head;
    free(p);
    return OK;
}

/*遍历队列*/
int Queue_View(LinkQueue* que)
{
    if(que->rear == que->head)
        return ERROR;
    
    QueuePtr p =que->head->next;
    while(p)
    {
        printf("val:%d",p->data);
        p=p->next;
    }
    return OK;
}




/*.BH-----------------------------------------------------------------
**
**函数名：
**
**功能：将Unicode（UCS-2LE）编码转换成UTF8编码  低地址存低位字节
**
**参数： 
**    char* pInput     输入字符串
**    char*pOutput   输出字符串
**
**返回值：转换后的Utf8字符串的字节数，如果出错则返回-1
**
**设计注记：
**
**.EH-----------------------------------------------------------------
*/
int OMS_UnicodeToUtf8(char* pInput, char *pOutput)
{
	int len = 0; //记录转换后的Utf8字符串的字节数
	while (*pInput)
	{
		//处理一个unicode字符
		char low = *pInput;//取出unicode字符的低8位
		pInput++;
		char high = *pInput;//取出unicode字符的高8位
		int w = high << 8;
		unsigned  wchar = (high << 8) + low;//高8位和低8位组成一个unicode字符,加法运算级别高

		if (wchar <= 0x7F) //英文字符
		{
			pOutput[len] = (char)wchar;  //取wchar的低8位
			len++;
		}
		else if (wchar >= 0x80 && wchar <= 0x7FF)  //可以转换成双字节pOutput字符
		{
			pOutput[len] = 0xc0 | ((wchar >> 6) & 0x1f);  //取出unicode编码低6位后的5位，填充到110yyyyy 10zzzzzz 的yyyyy中
			len++;
			pOutput[len] = 0x80 | (wchar & 0x3f);  //取出unicode编码的低6位，填充到110yyyyy 10zzzzzz 的zzzzzz中
			len++;
		}
		else if (wchar >= 0x800 && wchar < 0xFFFF)  //可以转换成3个字节的pOutput字符
		{
			pOutput[len] = 0xe0 | ((wchar >> 12) & 0x0f);  //高四位填入1110xxxx 10yyyyyy 10zzzzzz中的xxxx
			len++;
			pOutput[len] = 0x80 | ((wchar >> 6) & 0x3f);  //中间6位填入1110xxxx 10yyyyyy 10zzzzzz中的yyyyyy
			len++;
			pOutput[len] = 0x80 | (wchar & 0x3f);  //低6位填入1110xxxx 10yyyyyy 10zzzzzz中的zzzzzz
			len++;
		}

		else //对于其他字节数的unicode字符不进行处理
		{
			return -1;
		}
		pInput++;//处理下一个unicode字符
	}
	//utf8字符串后面，有个\0
	pOutput[len] = 0;
	return len;
}


/*.BH-----------------------------------------------------------------
**
**函数名：
**
**功能：将UTF8编码转换成Unicode（UCS-2LE）编码  低地址存低位字节
**
**参数：
**    char* pInput     输入字符串
**    char*pOutput   输出字符串
**
**返回值：转换后的Unicode字符串的字节数，如果出错则返回-1
**
**设计注记：
**
**.EH-----------------------------------------------------------------
*/
int OMS_Utf8ToUnicode(char* pInput, char* pOutput)
{
	int outputSize = 0; //记录转换后的Unicode字符串的字节数

	while (*pInput)
	{
		if (*pInput > 0x00 && *pInput <= 0x7F) //处理单字节UTF8字符（英文字母、数字）
		{
			*pOutput = *pInput;
			pOutput++;
			*pOutput = 0; //小端法表示，在高地址填补0
		}
		else if (((*pInput) & 0xE0) == 0xC0) //处理双字节UTF8字符
		{
			char high = *pInput;
			pInput++;
			char low = *pInput;
			if ((low & 0xC0) != 0x80)  //检查是否为合法的UTF8字符表示
			{
				return -1; //如果不是则报错
			}

			*pOutput = (high << 6) + (low & 0x3F);
			pOutput++;
			*pOutput = (high >> 2) & 0x07;
		}
		else if (((*pInput) & 0xF0) == 0xE0) //处理三字节UTF8字符
		{
			char high = *pInput;
			pInput++;
			char middle = *pInput;
			pInput++;
			char low = *pInput;
			if (((middle & 0xC0) != 0x80) || ((low & 0xC0) != 0x80))
			{
				return -1;
			}
			*pOutput = (middle << 6) + (low & 0x3F);//取出middle的低两位与low的低6位，组合成unicode字符的低8位
			pOutput++;
			*pOutput = (high << 4) + ((middle >> 2) & 0x0F); //取出high的低四位与middle的中间四位，组合成unicode字符的高8位
		}
		else //对于其他字节数的UTF8字符不进行处理
		{
			return -1;
		}
		pInput++;//处理下一个utf8字符
		pOutput++;
		outputSize += 2;
		
		swap2byte(pOutput-2);
	}
	//unicode字符串后面，有两个\0
	*pOutput = 0;
	pOutput++;
	*pOutput = 0;
	return outputSize;
}


/*.BH-----------------------------------------------------------------
**
**函数名：
**
**功能：将Gbk编码转换成Unicode（UCS-2LE）编码  低地址存低位字节
**
**参数：
**    char* gbk_buf			输入字符串
**    char* unicode_buf		输出字符串
**    int max_unicode_buf_size
**
**返回值：转换后的Unicode字符串的字节数，如果出错则返回-1
**
**设计注记：
**
**.EH-----------------------------------------------------------------
*/
int OMS_GbkToUnicode(char *gbk_buf, unsigned short *unicode_buf, int max_unicode_buf_size)
{
    unsigned short word;
    unsigned char *gbk_ptr =(unsigned char *) gbk_buf;
    unsigned short *uni_ptr = unicode_buf;
	unsigned int uni_ind = 0, gbk_ind = 0, uni_num = 0;
	unsigned char ch;
    int word_pos;

    if( !gbk_buf || !unicode_buf )
        return -1;

    while(1)
    {
    	ch = *(gbk_ptr + gbk_ind);

		if(ch == 0x00)
			break;
		
        if( ch > 0x80 )
        {
			//将gbk编码的中文字符的两个字节组合成一个    unsigned short word;
				word = *(gbk_ptr + gbk_ind);
				word <<= 8;
				word += *(gbk_ptr + gbk_ind+1);
				gbk_ind += 2;

            word_pos = word - gbk_first_code;
            if(word >= gbk_first_code && word <= gbk_last_code  && (word_pos < unicode_buf_size))
            {
				*(uni_ptr + uni_ind) = unicodeTables[word_pos];
				uni_ind++;
				uni_num++;
            }
        }
	else
	{
			gbk_ind++;
			*(uni_ptr + uni_ind) = ch;
			uni_ind++;
			uni_num++;
        }
        
        if(uni_num > max_unicode_buf_size - 1)
			break;
    }

    return uni_num;
}

//unicode->gbk
/*.BH-----------------------------------------------------------------
**
**函数名：
**
**功能：将Unicode（UCS-2LE）编码转换成gbk编码  低地址存低位字节
**
**参数：
**    char* unicode_buf		输入字符串
**    char* gbk_buf			输出字符串
**    int max_gbk_buf_size
**
**返回值：转换后的gbk字符串的字节数，如果出错则返回-1
**
**设计注记：
**
**.EH-----------------------------------------------------------------
*/
int OMS_UnicodeToGbk(unsigned short *unicode_buf, char *gbk_buf, int max_gbk_buf_size)
{
	unsigned short word;
	unsigned short gbk_word;
    unsigned char *gbk_ptr =(unsigned char *) gbk_buf;
    unsigned short *uni_ptr = unicode_buf;
	unsigned int uni_ind = 0, gbk_ind = 0, gbk_num = 0;
    int word_pos;

    if( !gbk_buf || !unicode_buf )
        return -1;

	while(1)
	{
		word = *(uni_ptr + uni_ind);
		//swap2byte(&word);
		uni_ind++;

		if(word == 0x0000)  //字符串结束符
			break;
		
		if(word < 0x80)  /*ASCII不用查表*/
		{
			*(gbk_ptr + gbk_ind) = (unsigned char)word;
			gbk_ind++;
		}
		else
		{
			word_pos = word - unicode_first_code;
			if(word >= unicode_first_code && word <= unicode_last_code && word_pos < gbk_buf_size)
			{
				gbk_word = gbkTables[word_pos];//gbk_word是gbk编码，但是为unsigned short类型，需要拆分成两个字节

				*(gbk_ptr + gbk_ind) = (unsigned char)((gbk_word & 0xff00) >> 8);//提取高8位
				gbk_ind++;
				*(gbk_ptr + gbk_ind) = (unsigned char)(gbk_word & 0x00ff);//提取低8位
				gbk_ind++;
				gbk_num +=2;//gbk字符加2个
			}
		}

		if(gbk_num > max_gbk_buf_size - 1)
			break;
	}

    return gbk_num;
}

