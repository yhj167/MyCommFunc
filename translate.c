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


/*整型转字符串*/
char * Common_Int2Str(int num)
{
	char str_int[10]={0};
	sprintf(str_int, "%d", num);
	return str_int;
}

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
