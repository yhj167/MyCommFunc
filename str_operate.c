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
/*方法二*/
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
