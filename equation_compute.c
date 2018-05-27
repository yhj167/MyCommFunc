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
