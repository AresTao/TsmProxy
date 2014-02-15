//comtypedef_fchar.h
//By LXM. 2009-02-26
#ifndef __COMTYPEDEF_FCHAR_H__
#define __COMTYPEDEF_FCHAR_H__

#include "comtypedef.h"

/**
 * Class：		CFixedChar
 * Author:         	Lxm
 * Parent Class：
 * Date:          	2009.02.26
 * Description:  	固定长度字符数组类-CFixedChar。
   使用说明：
   1. CFixedChar的size是固定的，使用前应该调用setSize()设定其固定长度。或者使用CFixedChar(int Size)构造函数。
      CFixedChar总是会分配size+1个字符内存空间。第size+1总是会被置为0.
   2. CFixedChar支持以'\0'结尾的字符串，同时也支持中间有'\0'的字符数组，例如UCS-2编码的字符。
   3. 赋值方式：
      有两种赋值方式，1）调用set() 2） 直接使用=号赋值。
	  CFixedChar重载了操作符=, 右值可以是CHAR*, CFixedChar,以及CVarChar8,...,CVarChar256,CVarChar.
	  另外，如果所赋值的长度超过了size()，那么后面的部分被截断，且实际length为size。由于第size+1个字符位置会补'\0'，因此调用c_str()仍将获得一个有结尾符的有效字符串。
	  如果所赋值的长度小于size(), 那么后面会用'\0'补齐。
	  
	  以下分别说明:
	  A. 所赋值是CHAR* 类型时：
      对于UCS-2等中间可能出现'\0'的字符数组，赋值时必须使用set(CHAR* str,INT len)，len必须显式的传入。
	  而对于以'\0'结尾的字符串，调用set(CHAR* str)即可，或者直接使用 = 号赋值。
	  eg.
	  CHAR* s="123456";
	  CFixedChar fchar;
	  fchar.setSize(21);//固定长度为21.
	  以下赋值都是可以的：
	  fchar.set(s);
	  or: fchar.set(s,strlen(s));
	  or: fchar = s;

      此时，调用fchar.size()返回21；调用fchar.length()返回6.  
     
	  B. 右值是CVarCharXX或CFixedChar
	  直接用=号赋值。由于CVarCharXX也是支持中间是'\0'的字符数组的，赋值后两者的length是一致的。
	  eg:
	  CVarChar32 vchar;
	  CFixedChar fchar;
	  fchar.setSize(20);
	  fchar = vchar;
	  
  4. 取值方式：
     1）获得字符串首指针：c_str();
	 2) 获得字符串长度：length()；
	 3）获得最大固定长度：size()；
	 length <= size;

  5. 编解码时，总是会编码size()个字节，解码也是size()个字节。

 * Last Modified:
 * Notes:
 		通常用于协议栈。如SMGP协议。
 */
#include <strstream>

using namespace std;

_CLASSDEF(CVarChar8)
_CLASSDEF(CVarChar16)
_CLASSDEF(CVarChar32)
_CLASSDEF(CVarChar64)
_CLASSDEF(CVarChar128)
_CLASSDEF(CVarChar256)
_CLASSDEF(CVarChar)

class CFixedChar
{
private:
	INT  m_length; //此字符串的实际长度，以'\0'结尾判断
	INT  m_size;   //此字符数组的固定最大长度

	CHAR *m_content;

public:

	CFixedChar();
	CFixedChar(INT size);
	~CFixedChar(); 
	CFixedChar(const CFixedChar &r);
	CFixedChar &operator=(const CFixedChar &r);
	CFixedChar &operator=(const CHAR* r);
	CFixedChar &operator=(const CVarChar8 &r);
	CFixedChar &operator=(const CVarChar16 &r);
	CFixedChar &operator=(const CVarChar32 &r);
	CFixedChar &operator=(const CVarChar64 &r);
	CFixedChar &operator=(const CVarChar128 &r);
	CFixedChar &operator=(const CVarChar256 &r);
	CFixedChar &operator=(const CVarChar &r);

	BOOL operator == (const CFixedChar&);

	BOOL setSize(INT size);//重新分配空间
	INT size() const ;	//此字符数组的固定最大长度，编解码用
	INT length() const ;	//此字符数组的实际长度，以'\0'结尾判断

	CHAR* c_str() const; 
	void set(const CHAR*,INT);
	void set(const CHAR*);

	INT encode( CHAR* &buff ) const;
	INT decode( CHAR* &buff );

	void print(ostrstream &st);

};

#endif
