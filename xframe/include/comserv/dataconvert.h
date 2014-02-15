#ifndef _DATACONVERT_H 
#define  _DATACONVERT_H 

using namespace std;

//数据编码类型
enum TEncodingType
{
	data_encoding_default=0,
	data_encoding_ascii=1,
	data_encoding_ucs2=8,	//相当于unicode16
	data_encoding_utf8=9,	//自定义（只在中间消息中存在。到SMPP时会被转换成gbk或ucs2）
	data_encoding_gb18030=15,	//GBK=GB18030, 18030等同于GBK（到二级网关时不作转换，但编码类型填15）
	data_encoding_gb2312=13,	//自定义，GBK的子集（到二级网关时不作转换，但编码类型填15）
	data_encoding_gb13000=14,	//自定义，GBK的子集（到二级网关时不作转换，但编码类型填15）
	data_encoding_big5=17,
};

char* itoa_character_set(TEncodingType type_uint);
TEncodingType atoi_character_set(const char* type_char);
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
int dataEncodingConvert(TEncodingType srcEncoding,char* srcContent,const unsigned int srcLength,TEncodingType destEncoding,char* destContent,unsigned int &destLength);

/////////////////////////////
// 字符打印
/////////////////////////////
int dataEncodingPrint(char* srcBuf, int srcLen, TEncodingType encoding, char* destBuf, int& dstLen);

#endif
