#include <stdio.h>
#include "func.h"
#include "dataconvert.h"

#ifndef WIN32
extern "C"
{
#include <iconv.h>
}
#endif

char* itoa_character_set(TEncodingType type_uint)
{
	switch (type_uint)
	{
		case data_encoding_ascii:
			return "ASCII";
		case data_encoding_ucs2:
			return "UCS-2BE";
		case data_encoding_utf8:
			return "UTF-8";
		case data_encoding_gb18030:
			return "GB18030";
		case data_encoding_default:
			return "GB18030";		
		default:
			return "UNKNOWN";

	}
}

TEncodingType atoi_character_set(const char* type_char)
{
	if(type_char==NULL) return data_encoding_default;
	char temp[64];
	strcpy(temp,type_char);
	if(strnotcasecmp(temp,"GB18030")==0)
		return data_encoding_gb18030;
	else if(strnotcasecmp(temp,"GBK")==0)
		return data_encoding_gb18030;
	else if(strnotcasecmp(temp,"UCS-2")==0)
		return data_encoding_ucs2;
	else if(strnotcasecmp(temp,"UTF-8")==0)
		return data_encoding_utf8;
	else if(strnotcasecmp(temp,"ASCII")==0)
		return data_encoding_ascii;
	else
		return data_encoding_default;
	
}

/////////////////////////////
// 字符编码方式转换
/////////////////////////////
//输入参数：
//	TEncodingType srcEncoding：	源编码类型
//   char*         srcContent ：		源字符串内容
//	unsigned int 	    srcLength:			源字符串长度

//	TEncodingType destEncoding：	需要转换成的目标编码类型
//	char*  		destContent: 		目标缓冲区首指针
//    unsigned int 	   	&destLength:  		目标缓冲区的大小

//输出参数：
//  char*         destContent:  转换后的字符串内容首指针
//  unsigned int 		   &destLength:  转换后的字符串长度

//返回值：
//      1:  成功
//      0: 失败
//	   注意: 转换失败destLength会被置为0

int dataEncodingConvert(TEncodingType srcEncoding,char* srcContent,const unsigned int srcLength,TEncodingType destEncoding,char* destContent,unsigned int &destLength)
{
#ifndef WIN32
	iconv_t cd;					  /* conversion descriptor */
	size_t ret_val;				  /* number of conversions */
	size_t inbytesleft=srcLength;
	size_t outbytesleft=destLength; 

	if ((NULL==srcContent)||(NULL==destContent))
	{
		destLength=0;
		return 0;
	}

	/* Initiate conversion -- get conversion descriptor */
	if ((cd = iconv_open(itoa_character_set(destEncoding), itoa_character_set(srcEncoding))) == (iconv_t)-1) 
	{
		perror("iconv_open() error:");
		destLength=0;
		return 0;
	}

	ret_val = iconv(cd, &srcContent, &inbytesleft, &destContent, &outbytesleft);

	if (ret_val<0)
	{
		perror("iconv() error:");
		destLength=0;
		return 0;
	}

	if (iconv_close(cd)<0)
	{
		perror("iconv_close() error:");
	}

	destLength = destLength-outbytesleft;
	return 1;
#else
	return 0;
#endif
}


int dataEncodingPrint(char* srcBuf, int srcLen, TEncodingType encoding, char* destBuf, int& dstLen)
{
	//将编码后的字符串按照一定规则打印出来
	switch(encoding)
	{
		case data_encoding_ucs2:
			//UCS2的编码，每个字按照  %u+十六进制数 打印出来
			if( srcLen%2 > 0 ) return 0;
			dstLen=0;
			for(int i=0;i<srcLen/2;i++)
			{
				sprintf(destBuf,"%%u%02x%02x",(unsigned char)srcBuf[i],(unsigned char)srcBuf[i+1]);
				i=i+2;
				dstLen+=6;
			}
			return 1;
		default:
			return 0;
	}
	return 0;
}



