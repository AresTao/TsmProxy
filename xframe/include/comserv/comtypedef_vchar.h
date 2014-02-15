 /******************************************************************************
 *Copyright(c)2005, by MT2
 *All rights reserved

 *FileName:   comtypedef_vchar.h
 *System:     UniFrame
 *SubSystem:
 *Author:     Li Jinglin
 *Date：     2005.11.14
 *Version：     4.0
 *Description：
      CVarCharXX的定义

 *
 * Last Modified:
    2005-11-13 v1.1 李静林  替代了原来的commsgtype.h头文件。
   2005-11-18 v1.2 李静林  需要using namespace std;
   2005-11-23 v1.3 李静林  修订strstream头文件，保证vc6中能够被正确编译;
   2005-11-29 v1.4 李静林 将头文件修改为<strstream >，多种编译器 都能编译通过 
   2006-04-19 v1.5 龙湘明， 将ENCODE、DECODE等宏移到msg.h中。
   2006-04-26 v1.6 龙湘明， 修改==操作定义和实现，由friend改为内嵌方法形式；
                            修正size()实现中的错误。  
    2009-02-28 龙湘明. 增加增加c_str(),length()等方法，更便于使用；
               重载operator=，可以以字符串或CVarCharxx直接赋值。    

 * 原始记录
 * Description：   defined common str type used by msg.
 *               编解码函数实现在文件 commsgtype.c 中
 *  2003.11.13, 完成初始版本
 *     By         Shuang Kai
 *
 *Last Modified:
 *  2003.11.26,完成各可变长字符数组类的方法的实现
 *       By      Peng Jin
 *    2003.12.23, 增加了using namespace std; 行；修改类的属性前缀：BYTE：m_uc开头； CHAR 以m_c开头
       By       Shuangkai
 *  2003.12.24, 将各CVarChar类的GetVarCharLen成员函数改为常成员函数
 *      By      PengJin
 *  2004.02.09, 重载各CVarChar类的"=="运算符
 *      By      PengJin
 *  2004.03.02, 将各类的print方法的参数类型从ostringstream修改为ostrstream
 *      By      PengJin
 *   2004.03.23 by PengJin
 *            因为HP的部分产品（ISUP协议栈）不能支持-AA编译选项，故将本文件中的命名空间移除
 *   2004.04.07 by Shuangkai
 *            修改 ENCODE DECODE 的宏定义
 *   2004.04.09 by Yeli
 *            把所有的 GetVarCharContent 函数的无效返回值从 return NULL; 改为 return 0;
 *            消除 g++ 编译时的 warning
 *   2004.04.13 by PengJin
 *            在所有的SetVarCharContent(CHAR*,INT)将m_cVarCharContent的有效长度字符的下一字符设置为终结符
 *  2004.06.05 by Shuangkai
 *            将各个类的构造函数中添加如下语句 m_cVarCharContent[0] = '\0';
 *  2007.07.29 by Long Xiangming.
 *              为每个类增加SetVarCharContent(CHAR*)方法
 * 
 **********************************************************************/



#ifndef  _COMTYPEDEF_VCHAR_H
#define  _COMTYPEDEF_VCHAR_H

#include <strstream>    //added by ljl 2005-11-29 ostrstream标准库，VC、G++、MinGW都支持  
#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>
#include "comtypedef.h"

_CLASSDEF(CFixedChar)
using namespace std;

/* 说明：
   1. CVarChar8,CVarChar16,...,CVarChar256和CVarChar用以定义可变长字符数组。
      其中CVarChar8最大字符长度为8，CVarChar16最大长度为16.其他类似。 但CVarChar256最大长度为255（因为内部len类型为BYTE，最大取值为255）。
     而CVarChar主要用于定义超过255的字符串，但最大长度不能超过MAX_VCHAR_LENGTH(comconst.h定义，目前为8192）
   2. CVarCharXX 和 CVarChar 支持以'\0'结尾的字符串，同时也支持中间有'\0'的字符数组，例如UCS-2编码的字符。
   3. 赋值方式：
      有两种赋值方式，1）调用set() 2） 直接使用=号赋值。
     CVarCharXX和CVarChar重载了操作符=, 右值可以是CHAR*, CFixedChar,以及CVarChar8,...,CVarChar256,CVarChar.
     
     但是，对于CVarCharNN = CVarCharMM，NN必须>=MM。
     例如，CVarChar32 c1; CVarChar16 c2;
     c1 = c2; 是允许的；但c2 = c1则是不允许的。（但可以使用c2 = c1.c_str()的方式进行赋值，超过c2最大长度的部分会被截断）

     另外，如果所赋值的长度超过了其最大长度，那么后面的部分被截断，由于第length+1个字符位置会补'\0'，因此调用c_str()仍将获得一个有结尾符的有效字符串。
     
     
     所赋值是CHAR* 类型时：
      对于UCS-2等中间可能出现'\0'的字符数组，赋值时必须使用set(CHAR* str,INT len)，len必须显式的传入。
     而对于以'\0'结尾的字符串，调用set(CHAR* str)即可，或者直接使用 = 号赋值。
     
  4. 取值方式：
     1）获得字符串首指针：c_str(); 相当于原来的GetVarCharContentPoint().
    2) 获得字符串长度：length()；相当于原来的GetVarCharLen();
   
  5. 编解码时，会编码size()个字节. 
     对于CVarCharXX: size = length() + 1; (前面有一个BYTE类型作为长度标志）
     对于CVarChar:   size = length() + 4; (前面有一个INT类型作为长度标志）


  eg.
     CHAR* s[20];
    CVarChar16 vchar;
    strcpy(s,"123456");

    可以如下给vchar赋值：
    vchar = s;
    或vchar.set(s);
    或vchar.set(s,strlen(s));
    或vchar.SetVarCharContent(s);
    或vchar.SetVarCharContent(s,strlen(s));

    此时，调用vchar.length()返回6。

     但是，如果s中间包含'\0'（适用UCS-2等编码)，如下：
    s[0]='1'; s[1]= '\0'; s[2]='2';
    vchar.set(s,3);
    此时，调用vchar.length()返回3. vchar中前三个字符分别为'1','\0','2'. 第4个字符为结尾符'\0';
 */


/**
 * Class：      CVarChar8
 * Author:            Shuang Kai
 * Parent Class：
 * Date:             2003.11.13
 * Description:     可变长字符数组类-CVarChar8。
 * Last Modified:
    2003.11.26,         完成初始定义。
                 By Peng Jin
 * Notes:
    最多可容纳8个字符，可作为字符串输出

 */

class CVarChar;
class CVarChar8
{
private:
   BYTE m_ucVarCharLen;      //数组中有效字符的长度
   CHAR *m_cVarCharContent;    //大小为9,可容纳8个有效字符，第9个字符为'\0'


public:

   CVarChar8();
   CVarChar8(const CVarChar8 &r);
   CVarChar8 &operator=(const CVarChar8 &r);
   CVarChar8 &operator=(const CHAR* r); //added by lxm.2009-02-26。 可以直接赋值为字符串
   CVarChar8 &operator=(const CFixedChar &r);
   CVarChar8 &operator=(const CVarChar &r);
   ~CVarChar8(){if(m_cVarCharContent!=NULL) delete[] m_cVarCharContent;}
    
   //在Windows下调用COMPARE_FORCE_VCHAR宏时编译出错。故由friend形式改成内嵌的形式。
   //friend BOOL operator == (const CVarChar8&, const CVarChar8&);
   BOOL operator == (const CVarChar8&);

    INT GetVarCharLen() const;
    void SetVarCharLen(INT);

    CHAR GetVarCharContent(INT);
    void SetVarCharContent(INT,CHAR);

    CHAR*  GetVarCharContentPoint() const;
    void SetVarCharContent(const CHAR*,INT);
    void SetVarCharContent(const CHAR*);

    //added by LXM. 2009-02-26
    CHAR* c_str() const; //相当于GetVarCharContentPoint
    INT  length() const;//相当于GetVarCharLen
    void set(const CHAR*,INT);//相当于SetVarCharContent(const CHAR*,INT);
    void set(const CHAR*);//SetVarCharContent(const CHAR*);

    INT size(  ) const ;//用于编解码，比length()多1个字节

   INT encode( CHAR* &buff ) const;
   INT decode( CHAR* &buff );

   void print(ostrstream &st);
};


/**
 * Class：      CVarChar16
 * Author:            Shuang Kai
 * Parent Class：
 * Date:             2003.11.13
 * Description:     可变长字符数组类-CVarChar16。
 * Last Modified:
    2003.11.26,         完成初始定义。
                 By Peng Jin
 * Notes:
    最多可容纳16个字符，可作为字符串输出

 */


class CVarChar16
{
private:
   BYTE m_ucVarCharLen;
   CHAR *m_cVarCharContent;

public:


    CVarChar16();
   ~CVarChar16(){if(m_cVarCharContent!=NULL) delete[] m_cVarCharContent;}
   CVarChar16(const CVarChar16 &r);
   CVarChar16 &operator=(const CVarChar16 &r);
   CVarChar16 &operator=(const CHAR* r);
   CVarChar16 &operator=(const CFixedChar &r);
   CVarChar16 &operator=(const CVarChar8 &r);
   CVarChar16 &operator=(const CVarChar &r);

   //friend BOOL operator == (const CVarChar16&, const CVarChar16&);
   BOOL operator == (const CVarChar16&);

    INT GetVarCharLen() const;
    void SetVarCharLen(INT);

    CHAR GetVarCharContent(INT);
    void SetVarCharContent(INT,CHAR);

    CHAR*  GetVarCharContentPoint() const;
    void SetVarCharContent(const CHAR*,INT);
    void SetVarCharContent(const CHAR*);

    //added by LXM. 2009-02-27
    CHAR* c_str() const; //相当于GetVarCharContentPoint
    INT  length() const;//相当于GetVarCharLen
    void set(const CHAR*,INT);//相当于SetVarCharContent(const CHAR*,INT);
    void set(const CHAR*);//SetVarCharContent(const CHAR*);

    INT size(  ) const ;

   INT encode( CHAR* &buff ) const;
   INT decode( CHAR* &buff );

   void print(ostrstream &st);

};


/**
 * Class：      CVarChar32
 * Author:            Shuang Kai
 * Parent Class：
 * Date:             2003.11.13
 * Description:     可变长字符数组类-CVarChar32。
 * Last Modified:
    2003.11.26,         完成初始定义。
                 By Peng Jin
 * Notes:
    最多可容纳32个字符，可作为字符串输出

 */

class CVarChar32
{
private:
   BYTE m_ucVarCharLen;
   CHAR *m_cVarCharContent;

public:

    CVarChar32();
   ~CVarChar32(){if(m_cVarCharContent!=NULL) delete[] m_cVarCharContent;}
   CVarChar32(const CVarChar32 &r);
   CVarChar32 &operator=(const CVarChar32 &r);
   CVarChar32 &operator=(const CHAR *r);
   CVarChar32 &operator=(const CFixedChar &r);
   CVarChar32 &operator=(const CVarChar8 &r);
   CVarChar32 &operator=(const CVarChar16 &r);
   CVarChar32 &operator=(const CVarChar &r);

   //friend BOOL operator == ( const CVarChar32&, const CVarChar32&);
   BOOL operator == (const CVarChar32&);

    INT GetVarCharLen() const;
    void SetVarCharLen(INT);

    CHAR GetVarCharContent(INT);
    void SetVarCharContent(INT,CHAR);

    CHAR*  GetVarCharContentPoint() const;
    void SetVarCharContent(const CHAR*,INT);
    void SetVarCharContent(const CHAR*);

    //added by LXM. 2009-02-27
    CHAR* c_str() const; //相当于GetVarCharContentPoint
    INT  length() const;//相当于GetVarCharLen
    void set(const CHAR*,INT);//相当于SetVarCharContent(const CHAR*,INT);
    void set(const CHAR*);//SetVarCharContent(const CHAR*);


    INT size(  ) const ;

   INT encode( CHAR* &buff ) const;
   INT decode( CHAR* &buff );

   void print(ostrstream &st);

};


/**
 * Class：      CVarChar64
 * Author:            Shuang Kai
 * Parent Class：
 * Date:             2003.11.13
 * Description:     可变长字符数组类-CVarChar64。
 * Last Modified:
    2003.11.26,         完成初始定义。
                 By Peng Jin
 * Notes:
    最多可容纳64个字符，可作为字符串输出

 */

class CVarChar64
{
private:
   BYTE m_ucVarCharLen;
   CHAR *m_cVarCharContent;

public:

    CVarChar64();
   ~CVarChar64(){if(m_cVarCharContent!=NULL) delete[] m_cVarCharContent;}
   CVarChar64(const CVarChar64 &r);
   CVarChar64 &operator=(const CVarChar64 &r);
   CVarChar64 &operator=(const CHAR* r);
   CVarChar64 &operator=(const CFixedChar &r);
   CVarChar64 &operator=(const CVarChar8 &r);
   CVarChar64 &operator=(const CVarChar16 &r);
   CVarChar64 &operator=(const CVarChar32 &r);
   CVarChar64 &operator=(const CVarChar &r);

   //friend BOOL operator == (const CVarChar64&, const CVarChar64&);
   BOOL operator == (const CVarChar64&);

    INT GetVarCharLen() const;
    void SetVarCharLen(INT);

    CHAR GetVarCharContent(INT);
    void SetVarCharContent(INT,CHAR);

    CHAR*  GetVarCharContentPoint() const;
    void SetVarCharContent(const CHAR*,INT);
    void SetVarCharContent(const CHAR*);

    //added by LXM. 2009-02-27
    CHAR* c_str() const; //相当于GetVarCharContentPoint
    INT  length() const;//相当于GetVarCharLen
    void set(const CHAR*,INT);//相当于SetVarCharContent(const CHAR*,INT);
    void set(const CHAR*);//SetVarCharContent(const CHAR*);


    INT size(  ) const ;

   INT encode( CHAR* &buff ) const;
   INT decode( CHAR* &buff );

   void print(ostrstream &st);

};


/**
 * Class：      CVarChar128
 * Author:            Shuang Kai
 * Parent Class：
 * Date:             2003.11.13
 * Description:     可变长字符数组类-CVarChar128。
 * Last Modified:
    2003.11.26,         完成初始定义。
                 By Peng Jin
 * Notes:
    最多可容纳128个字符，可作为字符串输出

 */

class CVarChar128
{
private:
   BYTE m_ucVarCharLen;
   CHAR *m_cVarCharContent;


public:

   CVarChar128();
   ~CVarChar128(){if(m_cVarCharContent!=NULL) delete[] m_cVarCharContent;}
   CVarChar128(const CVarChar128 &r);
   CVarChar128 &operator=(const CVarChar128 &r);
   CVarChar128 &operator=(const CHAR* r);
   CVarChar128 &operator=(const CFixedChar &r);
   CVarChar128 &operator=(const CVarChar8 &r);
   CVarChar128 &operator=(const CVarChar16 &r);
   CVarChar128 &operator=(const CVarChar32 &r);
   CVarChar128 &operator=(const CVarChar64 &r);
   CVarChar128 &operator=(const CVarChar &r);

   //friend BOOL operator == ( const CVarChar128&,  const CVarChar128&);
   BOOL operator == (const CVarChar128&);

    INT GetVarCharLen() const;
    void SetVarCharLen(INT);

    CHAR GetVarCharContent(INT);
    void SetVarCharContent(INT,CHAR);

    CHAR*  GetVarCharContentPoint() const;
    void SetVarCharContent(const CHAR*,INT);
    void SetVarCharContent(const CHAR*);

    //added by LXM. 2009-02-27
    CHAR* c_str() const; //相当于GetVarCharContentPoint
    INT  length() const;//相当于GetVarCharLen
    void set(const CHAR*,INT);//相当于SetVarCharContent(const CHAR*,INT);
    void set(const CHAR*);//SetVarCharContent(const CHAR*);


    INT size(  ) const ;

   INT encode( CHAR* &buff ) const;
   INT decode( CHAR* &buff );

   void print(ostrstream &st);

};


/**
 * Class：      CVarChar256
 * Author:            Shuang Kai
 * Parent Class：
 * Date:             2003.11.13
 * Description:     可变长字符数组类-CVarChar256。
 * Last Modified:
    2003.11.26,         完成初始定义。
                 By Peng Jin
 * Notes:
    最多可容纳256个字符，可作为字符串输出

 */

class CVarChar256
{
private:
   //INT m_ucVarCharLen;
   BYTE  m_ucVarCharLen;
   CHAR *m_cVarCharContent;

public:

   CVarChar256();
   ~CVarChar256(){if(m_cVarCharContent!=NULL) delete[] m_cVarCharContent;}
   CVarChar256(const CVarChar256 &r);
   CVarChar256 &operator=(const CVarChar256 &r);
   CVarChar256 &operator=(const CHAR* r);
   CVarChar256 &operator=(const CFixedChar &r);
   CVarChar256 &operator=(const CVarChar8 &r);
   CVarChar256 &operator=(const CVarChar16 &r);
   CVarChar256 &operator=(const CVarChar32 &r);
   CVarChar256 &operator=(const CVarChar64 &r);
   CVarChar256 &operator=(const CVarChar128 &r);
   CVarChar256 &operator=(const CVarChar &r);

   //friend BOOL operator == (const CVarChar256&, const CVarChar256&);
   BOOL operator == (const CVarChar256&);

    INT GetVarCharLen() const;
    void SetVarCharLen(INT);

    CHAR GetVarCharContent(INT);
    void SetVarCharContent(INT,CHAR);

    CHAR*  GetVarCharContentPoint() const;
    void SetVarCharContent(const CHAR*,INT);
    void SetVarCharContent(const CHAR*);

    //added by LXM. 2009-02-26
    CHAR* c_str() const; //相当于GetVarCharContentPoint
    INT  length() const;//相当于GetVarCharLen
    void set(const CHAR*,INT);//相当于SetVarCharContent(const CHAR*,INT);
    void set(const CHAR*);//SetVarCharContent(const CHAR*);


    INT size() const ;

   INT encode( CHAR* &buff ) const;
   INT decode( CHAR* &buff );

   void print(ostrstream &st);

};


/**
 * Class：      CVarChar
 * Author:            LJL
 * Parent Class：
 * Date:             2007.1.9
 * Description:     可变长字符数组类-CVarChar。
 * Last Modified:
 * Notes:
       不限长度输出，可作为字符指针的封装，用于缓冲区等一类应用
*/

class CVarChar
{
private:
   //只有CVarChar的m_ucVarCharLen为INT类型，其他CVarChar8...CVarChar256等都使用BYTE类型。
   INT m_ucVarCharLen;
   CHAR *m_cVarCharContent;

public:

    CVarChar();
   ~CVarChar(){if (m_cVarCharContent!=NULL) delete[] m_cVarCharContent;}
   CVarChar(const CVarChar &r);
   CVarChar &operator=(const CVarChar &r);
   CVarChar &operator=(const CHAR* r);
   CVarChar &operator=(const CFixedChar &r);
   CVarChar &operator=(const CVarChar8 &r);
   CVarChar &operator=(const CVarChar16 &r);
   CVarChar &operator=(const CVarChar32 &r);
   CVarChar &operator=(const CVarChar64 &r);
   CVarChar &operator=(const CVarChar128 &r);
   CVarChar &operator=(const CVarChar256 &r);

   BOOL operator == (const CVarChar&);

    INT GetVarCharLen() const;      //获得缓冲区内容长度
    void SetVarCharLen(INT);         //可以将本来很长的缓冲区截短，但是不能将短缓冲区增长

    CHAR GetVarCharContent(INT);
    void SetVarCharContent(INT,CHAR);

    CHAR*  GetVarCharContentPoint() const;
    void SetVarCharContent(const CHAR*,INT);   //设置缓冲区内容
    void SetVarCharContent(const CHAR*);

    //added by LXM. 2009-02-27
    CHAR* c_str() const; //相当于GetVarCharContentPoint
    INT  length() const;//相当于GetVarCharLen
    void set(const CHAR*,INT);//相当于SetVarCharContent(const CHAR*,INT);
    void set(const CHAR*);//SetVarCharContent(const CHAR*);


   INT size(  ) const ;      //获得整个 vchar 编码长度

   INT encode( CHAR* &buff ) const;
   INT decode( CHAR* &buff );

   void print(ostrstream &st);
};

class CStr
{
   public:
      CStr();
      CStr(const char* str);
      CStr(const CStr& a);
      CStr(int value);
      ~CStr();

      void operator=(const char* str);
      void operator=(int value);
      void operator=(const CStr& str);
      int  operator==(const char* str) const;
      int  operator==(const CStr& str) const;
      int  operator!=(const char* str) const;
      int  operator!=(const CStr& str) const;

      virtual int  operator>(const char* str) const;
      virtual int  operator>(const CStr& str) const;
      virtual int  operator<(const char* str) const;
      virtual int  operator<(const CStr& str) const;
      virtual int  operator<=(const char* str) const;
      virtual int  operator<=(const CStr& str) const;
      virtual int  operator>=(const char* str) const;
      virtual int  operator>=(const CStr& str) const;

      CStr& operator+=(const CStr& d);
      CStr& operator+=(const char*);
      CStr& operator+=(const char c);

      CStr& operator<<(const char*);
      CStr& operator<<(int);
      CStr& operator<<(CStr&);

      int split(CStr* argvArray, int maxArgc) const;
      int split(CStr* argvArray, int maxArgc, const char* sep) const;
      int merge(CStr* argvArray, int argc);

      void setByte(char c, int pos);
      char getByte(int pos);
      const char* cut(int length);
      void cut(const char* sep, CStr& result);
      const char* trim();
      const char* trimAll(); // delete all char not isalnum()
      void clear();

      HASH_CODE hash();

      // return the content and clean this
      char* steal(int& len);

      // the most short complement integer
      CStr& intCatShort(int i);

      // fix 4 byte integer
      CStr& intCat4(int i);

      // 1xxxxxxx 1xxxxxxx ... 0xxxxxxx
      CStr& intCat7(int i);

      // unicode integer to utf8 format string
      CStr& intCatUtf8(int i);

      CStr& nCat(const char* str, int length);
      CStr& fCat(const char* fmt, ...);
      CStr& fCatV(const char* fmt, va_list args);
      CStr& fCatBin(const char* content, int length);
      CStr& fCatBin2(const char* content, int length, int indent);
      CStr& fCatShortBin(const char* content, int length);
      CStr& fCatTitle(const char* title, char fillChar, int length);
      CStr& fCatChars(char fillChar, int length);
      CStr& fileCat(const char* fileName);
      CStr& replace(const char* pattern, const char* replace);

      CHAR* GetVarCharContentPoint() const;
      void SetVarCharContent(INT num, CHAR character);
      void SetVarCharContent(const CHAR*,INT);
      void getFieldName(const char** p, int& arrayIndex);
      const char* c_strList() const;
      inline const char* c_str() const
      {
         m_buffer[m_length]=0;
         return m_buffer;
      }
      int toInt() const {  return atoi(m_buffer); }
      inline int length() const { return m_length; }
      int empty() const {return length()==0;}

      CStr& toLower();
      CStr& toUpper();
      CStr& htmlEncode();
      CStr& uricEncode();
      CStr& uricDecode();
      CStr& iconv(const char* from, const char* to);

      void print(ostrstream &st);
      virtual int decode(char* &buf);
      virtual int encode(char* &buf) const;
      virtual int size() const;
      void set(const char*, int length);

   protected:
      void resize(int);

      int m_length;
      int m_capacity;
      char* m_buffer;
#define MaxCStrBufferLen 32
      char _m_buffer[MaxCStrBufferLen];
};
HASH_CODE _GetHash(CStr);

class CStrShort : public CStr
{
public:
   int decode(char* &buf);
   int encode(char* &buf) const;
   int size() const;
};

class CStrInt : public CStr
{
public:
   int  operator>(const char* str) const;
   int  operator<(const char* str) const;
   int  operator<=(const char* str) const;
   int  operator>=(const char* str) const;

   int  operator>(const CStr& str) const;
   int  operator<(const CStr& str) const;
   int  operator<=(const CStr& str) const;
   int  operator>=(const CStr& str) const;
};

//对数组类型，解码时需要判断size是否越界（收到的code中的length可能超过数组最大长度)
//comtypedef_vchar.C中使用
#ifndef CHECK_DECODE_VCHAR_SIZE
#define CHECK_DECODE_VCHAR_SIZE(length,max_array_size)\
{\
   if(length>max_array_size || length<0) \
   {\
      UniERROR("decode CVarChar%d error: The length(%d) is out of the max array size(%d).",max_array_size,length,max_array_size);\
      length = 0;\
      return 0;\
   }\
}
#define CHECK_DECODE_VCHAR_SIZE_U(length,max_array_size)\
{\
   if(length>max_array_size) \
   {\
      UniERROR("decode CVarChar%d error: The length(%d) is out of the max array size(%d).",max_array_size,length,max_array_size);\
      length = 0;\
      return 0;\
   }\
}
#endif

//解码时，根据数组定义时的长度，判断length是否越界
#ifndef CHECK_DECODE_ARRAY_SIZE
#define CHECK_DECODE_ARRAY_SIZE(length,array_deflen,array_varname,array_type)\
{\
   if(length>array_deflen || length<0) \
   {\
      UniERROR("decode %s %s[] array error: The length(%s=%d) is out of the max array size(%d).",#array_type,#array_varname,#length,length,array_deflen);\
      length = 0;\
      return 0;\
   }\
}
#endif

//对数组类型，解码时需要判断size是否越界（收到的code中的length可能超过数组最大长度)
//限制数组的最大长度为MAX_ARRAY_SIZE(1024)
#ifndef CHECK_MAX_ARRAY_SIZE
#define CHECK_MAX_ARRAY_SIZE(length,decodename)\
{\
   if(length>MAX_ARRAY_SIZE || length<0) \
   {\
      UniERROR("decode array(%s) error: The length(%d) is out of the max array size(%d).",#decodename,length,MAX_ARRAY_SIZE);\
      length = 0;\
      return 0;\
   }\
}
#define CHECK_MAX_ARRAY_SIZE_U(length,decodename)\
{\
   if(length>MAX_ARRAY_SIZE) \
   {\
      UniERROR("decode array(%s) error: The length(%d) is out of the max array size(%d).",#decodename,length,MAX_ARRAY_SIZE);\
      length = 0;\
      return 0;\
   }\
}
#endif

#define ENCODE_INT( BUFFER, VAR ) \
{\
   UINT iTemp = htonl(VAR);\
   memcpy( BUFFER, &iTemp, sizeof(VAR) ); \
   BUFFER += sizeof(VAR);\
}

#define DECODE_INT( VAR, BUFFER ) \
{\
   UINT iTemp = 0;\
   memcpy( &iTemp, BUFFER, sizeof(VAR) ); \
   VAR = ntohl(iTemp);\
   BUFFER += sizeof(VAR);\
}

#define ENCODE( BUFFER, VAR ) \
{\
   memcpy( BUFFER, &VAR, sizeof(VAR) ); \
   BUFFER += sizeof(VAR);\
}

#define DECODE( VAR, BUFFER ) \
{\
   memcpy( &VAR, BUFFER, sizeof(VAR) ); \
   BUFFER += sizeof(VAR);\
}

#define ENCODE_ARRAY( BUFFER, ARRAY, LENGTH ) \
{\
   memcpy( BUFFER, ARRAY, sizeof(ARRAY[0])*LENGTH ); \
   BUFFER += sizeof(ARRAY[0])*LENGTH;\
}

#define DECODE_ARRAY( ARRAY, BUFFER, LENGTH )\
{\
   CHECK_MAX_ARRAY_SIZE(LENGTH,ARRAY);\
   memcpy( ARRAY, BUFFER, sizeof(ARRAY[0])*LENGTH ); \
   BUFFER += sizeof(ARRAY[0])*LENGTH;\
}
// for length is unsigned char
#define DECODE_ARRAY_U( ARRAY, BUFFER, LENGTH )\
{\
   memcpy( ARRAY, BUFFER, sizeof(ARRAY[0])*LENGTH ); \
   BUFFER += sizeof(ARRAY[0])*LENGTH;\
}
#endif

