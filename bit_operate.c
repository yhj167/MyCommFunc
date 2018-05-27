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

