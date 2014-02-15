 /******************************************************************************
 *Copyright(c)2005, by MT2
 *All rights reserved

 *FileName:   sscstrdef_addr.h
 *System:     softswitch
 *SubSystem:
 *Author:     Li Jinglin
 *Date：         2005.11.14
 *Version：      4.0
 *Description：
      VChar的实现
 *
 * Last Modified:
    2005-11-13 v1.1 李静林  替代了原来的commsgtype.C文件。

 *
 * Description：   各个子系统编解码公用的数据结构的编解码
 *
 *    2003.12.23, 各个类的print 函数
       By       Shuangkai
 *  2004.02.09, 重载各VarChar类的"=="运算符
      By      PengJin
 *  2004.03.02, 将各类的print方法的参数类型从ostringstream修改为ostrstream
 *      By      PengJin
 *   2004.03.23  by PengJin
 *      因为HP的部分产品（ISUP协议栈）不能支持-AA编译选项，故将本文件中的命名空间移除
 *   2004.05.04  by ShuangKai
 *      在所有的decode函数中添加 : m_cVarCharContent[m_ucVarCharLen]=0; 这样做的结果是解码出来的
 *      就是一个标准的字符串，因为编码时并未将字符串末尾的'\0'加入到CCode中，故在decode函数中加入此语句。
 *********************************************************************/

#include <memory>
#include <iomanip>

#include "comtypedef_vchar.h"
#include "comtypedef_fchar.h"
#include "func.h"
#include "iconv.h"
#include "errno.h"
#include "info.h"

CFixedChar::CFixedChar()
{
	m_length = 0;
	m_size   = 1;
	m_content = new CHAR[m_size+1];
	memset(m_content,0,m_size+1);

}

CFixedChar::CFixedChar(INT size)
{
	INT tmp = size;
	if(tmp<=0)
		tmp = 1;
	if(tmp>MAX_FCHAR_LENGTH)
	{
		UniERROR("ERROR in CFixedChar::CFixedChar(INT size): size is out bound: %d",MAX_FCHAR_LENGTH);
		tmp = MAX_FCHAR_LENGTH;
	}

	m_size = tmp;
	m_content = new CHAR[m_size+1];
	memset(m_content,0,m_size+1);
	m_length = 0;


}

CFixedChar::CFixedChar(const CFixedChar &r)
{
	m_length = r.m_length;
	m_size = r.m_size;
	m_content = new CHAR[r.m_size+1];
	memcpy_safec(m_content, r.m_content, m_size+1);
}

CFixedChar &CFixedChar::operator=(const CFixedChar &r)
{
	//不能改变m_size
	//if ( m_content != NULL ) delete[] m_content;
	INT len = r.m_length;
	if(len>m_size)
		len = m_size;
	m_length = len;
	//m_content = new CHAR[m_size+1];
	memset(m_content,0,m_size+1);
	memcpy_safe(m_content, r.m_content, len);
	return *this;
}

CFixedChar &CFixedChar::operator=(const CHAR* r)
{
	//不能改变m_size
	set(r);
	return *this;
}

CFixedChar &CFixedChar::operator=(const CVarChar8 &r)
{
	set(r.c_str(),r.length());
	return *this;

}

CFixedChar &CFixedChar::operator=(const CVarChar16 &r)
{
	set(r.c_str(),r.length());
	return *this;

}

CFixedChar &CFixedChar::operator=(const CVarChar32 &r)
{
	set(r.c_str(),r.length());
	return *this;

}

CFixedChar &CFixedChar::operator=(const CVarChar64 &r)
{
	set(r.c_str(),r.length());
	return *this;

}

CFixedChar &CFixedChar::operator=(const CVarChar128 &r)
{
	set(r.c_str(),r.length());
	return *this;
}

CFixedChar &CFixedChar::operator=(const CVarChar256 &r)
{
	set(r.c_str(),r.length());
	return *this;
}

CFixedChar &CFixedChar::operator=(const CVarChar &r)
{
	set(r.c_str(),r.length());
	return *this;

}

CFixedChar::~CFixedChar()
{
	if(m_content)
		delete m_content;

}

//重新分配空间
BOOL CFixedChar::setSize(INT size)
{
	if(size<=0) return FALSE;
	if(size>MAX_FCHAR_LENGTH)
	{
		UniERROR("WARNING in CFixedChar::setSize(INT size): size(%d) exceeds limit: %d",
			size,MAX_FCHAR_LENGTH);

		size = MAX_FCHAR_LENGTH;
	}

	if(m_content) delete m_content;
	m_size = size;
	m_content = new CHAR[m_size+1];
	memset(m_content,0,m_size+1);
	m_length = 0;

	return TRUE;
}

INT CFixedChar::length() const
{
	return m_length;
}

INT CFixedChar::size() const
{
	return m_size;
}


CHAR* CFixedChar::c_str() const
{
	return m_content;
}


void CFixedChar::set(const CHAR* buf, INT len)
{
	if(buf && len >0)
	{
		INT tmp = len;
		if(tmp>m_size)
		{
			UniERROR("WARNING in CFixedChar::set(): strlen(r)(%d) > m_size (%d). We cut it.",
				len,m_size);
			tmp = m_size;
		}
		memset(m_content,0,m_size+1);
		memcpy_safec(m_content, buf, tmp);
		m_length = tmp;

	}

}

void CFixedChar::set(const CHAR* buf)
{
	if(buf)
		set(buf,strlen(buf));
}

INT CFixedChar::decode(CHAR* &buf)
{
	memcpy( m_content, buf, m_size );
	m_content[m_size]=0;
	m_length=strlen(m_content);
	buf +=m_size;

	return size();
}

INT CFixedChar::encode(CHAR* &buf) const
{
	memcpy(buf,m_content,m_size);
	buf +=m_size;

	return size();
}


BOOL CFixedChar::operator == (const CFixedChar &fchar2)
{
	BOOL result=FALSE;
	if (m_length==fchar2.m_length)
	{
		result=TRUE;
		for (INT i=0;i<m_length ;i++ )
		{
			if(m_content[i]!=fchar2.m_content[i])
			{
				result=FALSE;
				break;
			}
		}
	}
	return result;
}

void CFixedChar::print(ostrstream &st)
{

  st << "{CFixedChar:";
  st << "m_size  =" << (INT)m_size << ";";
  st << "m_length=" << (INT)m_length << ";";
  if ( m_length>0 )
  {
  	st << "m_content" << (CHAR*)m_content << ";";
  }
  st << "}";
}


/* ---------------------------- 变长字符数组------------------------------*/


///////////////////////////////////////////
//////////////////////////////////////////
CVarChar8::CVarChar8()
{
   m_cVarCharContent = NULL;
   m_cVarCharContent = new CHAR[9];
   m_ucVarCharLen = 0;
   m_cVarCharContent[0] = '\0';
   m_cVarCharContent[8] = '\0';
}

CVarChar8::CVarChar8(const CVarChar8 &r)
{
   m_cVarCharContent = NULL;
   m_cVarCharContent = new CHAR[9];
   m_ucVarCharLen = r.m_ucVarCharLen;
   memcpy_safec(m_cVarCharContent, r.m_cVarCharContent, 9);
}

 CVarChar8 &CVarChar8::operator=(const CVarChar8 &r)
{
   m_ucVarCharLen = r.m_ucVarCharLen;
   memcpy_safe(m_cVarCharContent, r.m_cVarCharContent, 9);
   return *this;
}

//added by lxm.2009-02-26。 可以直接赋值为字符串
CVarChar8 &CVarChar8::operator=(const CHAR* r)
{
   if(r)
   {
      set(r);
   }
   return *this;

}

CVarChar8 &CVarChar8::operator=(const CFixedChar &r)
{
   set(r.c_str(),r.length());
   return *this;
}

CVarChar8 &CVarChar8::operator=(const CVarChar &r)
{
   set(r.c_str(),r.length());
   return *this;
}

/////////////////////////////////////////////////////////////////////
// Function：      GetVarCharLen
// Description:
//   取字符串中有效字符的个数
// Args：
//   无
// Return Values:
//      BYTE
//      返回字符串中有效字符的个数
//////////////////////////////////////////////////////////////////////

 INT CVarChar8::GetVarCharLen() const
{
   return m_ucVarCharLen;
}

/////////////////////////////////////////////////////////////////////
// Function：      SetVarCharLen
// Description:
//   设置字符串中有效字符的个数
// Args：
//   len BYTE  字符串中有效字符的个数
// Return Values:
//      无
//////////////////////////////////////////////////////////////////////


 void CVarChar8::SetVarCharLen(INT len)
{
   m_ucVarCharLen=len;
}

/////////////////////////////////////////////////////////////////////
// Function：      GetVarCharContent
// Description:
//   取成员变量m_cVarCharContent中的字符
// Args：
//   num   INT  所取字符的下标
// Return Values:
//      CHAR
//      返回下标所对应字符
//////////////////////////////////////////////////////////////////////

 CHAR CVarChar8::GetVarCharContent(INT num)
{
   if ((num>m_ucVarCharLen) || (num<0))
      return 0;
   else
      return m_cVarCharContent[num];
}


/////////////////////////////////////////////////////////////////////
// Function：      SetVarCharContent
// Description:
//   设置成员变量m_cVarCharContent中的字符
// Args：
//   num      INT        所取字符的下标
//   character   CHAR   欲写入的字符
// Return Values:
//      无
//////////////////////////////////////////////////////////////////////

 void CVarChar8::SetVarCharContent(INT num,CHAR character)
{
   if ((num<=m_ucVarCharLen) && (num>=0))
      m_cVarCharContent[num]=character;
}


/////////////////////////////////////////////////////////////////////
// Function：      GetVarCharContent
// Description:
//   取成员变量m_cVarCharContent中的指针
// Args：
//      无
// Return Values:
//         CHAR*
//         返回成员变量m_cVarCharContent的指针
//////////////////////////////////////////////////////////////////////

 CHAR* CVarChar8::GetVarCharContentPoint() const
{
   return (CHAR*)m_cVarCharContent;
}



/////////////////////////////////////////////////////////////////////
// Function：      SetVarCharContent
// Description:
//   设置成员变量m_cVarCharContent中的内容
// Args：
//   buf   CHAR*     待拷入成员变量m_cVarCharContent中的字符串
//   len   INT      拷入的有效长度
// Return Values:
//      无
//////////////////////////////////////////////////////////////////////

void CVarChar8::SetVarCharContent(const CHAR* buf, INT len)
{
   if (buf && len >0)
   {
      if(len>8) len = 8;  // if( len<=8 )
      memcpy(GetVarCharContentPoint(),buf,len);
      m_ucVarCharLen=len;
      m_cVarCharContent[len]='\0';
   }
}

void CVarChar8::SetVarCharContent(const CHAR* buf)
{
   if(buf)
      SetVarCharContent(buf,strlen(buf));
}


/////////////////////////////////////////////////////////////////////
// Function：      size
// Description:
//   返回所有成员变量有效大小
// Args：
//   无
// Return Values:
//    INT
//    返回所有成员变量有效大小
//////////////////////////////////////////////////////////////////////

 INT CVarChar8::size() const
{
   //modified by lxm. 2008.12.09. 使用实际长度进行有效编解码。避免编码过长，减少网络传输开销。
   return ( sizeof(BYTE) +  m_ucVarCharLen );

   //return ( sizeof(INT) +  9 );
}

//
//added by LXM. 2009-02-26
//
CHAR*  CVarChar8::c_str() const
{
   //相当于GetVarCharContentPoint
   return m_cVarCharContent;
};

INT   CVarChar8::length() const
{
   //相当于GetVarCharLen
   return m_ucVarCharLen;
}

void CVarChar8::set(const CHAR* buf,INT len)
{
   SetVarCharContent(buf,len);
}

void CVarChar8::set(const CHAR* buf)
{
   if(buf)
      SetVarCharContent(buf,strlen(buf));

}

BOOL CVarChar8::operator == ( const CVarChar8 &varchar2)
{
   BOOL result=FALSE;
   if (m_ucVarCharLen==varchar2.m_ucVarCharLen)
   {
      result=TRUE;
      for (INT i=0;i<m_ucVarCharLen ;i++ )
      {
         if(m_cVarCharContent[i]!=varchar2.m_cVarCharContent[i])
         {
            result=FALSE;
            break;
         }
      }
   }
   return result;
}

INT CVarChar8::decode(CHAR* &buf)
{
   //DECODE_INT(m_ucVarCharLen,buf);
   DECODE(m_ucVarCharLen,buf);
   CHECK_DECODE_VCHAR_SIZE_U(m_ucVarCharLen,8)
   DECODE_ARRAY_U(m_cVarCharContent,buf,m_ucVarCharLen);
   m_cVarCharContent[m_ucVarCharLen]=0;

   return size();
}

INT CVarChar8::encode(CHAR* &buf) const
{
   //ENCODE_INT(buf,m_ucVarCharLen);
   ENCODE(buf,m_ucVarCharLen);
   ENCODE_ARRAY(buf,m_cVarCharContent,m_ucVarCharLen);

   return size();
}

void CVarChar8::print(ostrstream &st)
{

  st << "{CVarChar8:";
  st << "m_ucVarCharLen=" << (INT)m_ucVarCharLen << ";";

  /*for(int i = 0 ; i < m_ucVarCharLen ; i++)
  {
    st << "m_cVarCharContent[" << i << "]=" << (INT)m_cVarCharContent[i] << ";";
  }
  st << "}";*/
  st << "m_cVarCharContent=" << (CHAR*)m_cVarCharContent << ";";
  st << "}";
}

/*************************************************************/

 CVarChar16::CVarChar16()
{
   m_cVarCharContent = NULL;
   m_cVarCharContent = new CHAR[17];
   m_ucVarCharLen = 0;
   m_cVarCharContent[0] = '\0';
   m_cVarCharContent[16] = '\0';
}

 CVarChar16::CVarChar16(const CVarChar16 &r)
{
   m_cVarCharContent = NULL;
   m_cVarCharContent = new CHAR[17];
   m_ucVarCharLen = r.m_ucVarCharLen;
   memcpy_safec(m_cVarCharContent, r.m_cVarCharContent, 17);
}

 CVarChar16 &CVarChar16::operator=(const CVarChar16 &r)
{
   m_ucVarCharLen = r.m_ucVarCharLen;
   memcpy_safe(m_cVarCharContent, r.m_cVarCharContent, 17);
   return *this;
}

//added by lxm.2009-02-27。 可以直接赋值为字符串
CVarChar16 &CVarChar16::operator=(const CHAR* r)
{
   if(r)
   {
      set(r);
   }
   return *this;

}

CVarChar16 &CVarChar16::operator=(const CFixedChar &r)
{
   set(r.c_str(),r.length());
   return *this;
}

CVarChar16 &CVarChar16::operator=(const CVarChar8 &r)
{
   set(r.c_str(),r.length());
   return *this;
}

CVarChar16 &CVarChar16::operator=(const CVarChar &r)
{
   set(r.c_str(),r.length());
   return *this;
}

INT CVarChar16::GetVarCharLen() const
{
   return m_ucVarCharLen;
}

 void CVarChar16::SetVarCharLen(INT len)
{
   m_ucVarCharLen=len;
}


 CHAR CVarChar16::GetVarCharContent(INT num)
{
   if ((num>m_ucVarCharLen) || (num<0))
      return 0;
   else
      return m_cVarCharContent[num];
}

 void CVarChar16::SetVarCharContent(INT num,CHAR character)
{
   if ((num<=m_ucVarCharLen) && (num>=0))
      m_cVarCharContent[num]=character;
}

 CHAR* CVarChar16::GetVarCharContentPoint() const
{
   return (CHAR*)m_cVarCharContent;
}

 void CVarChar16::SetVarCharContent(const CHAR* buf, INT len)
{
   if (buf && len >0 ) // && len<=16)
   {
      if(len > 16) len = 16;
      memcpy(m_cVarCharContent,buf,len);
      m_ucVarCharLen=len;
      m_cVarCharContent[len]='\0';
   }
}

void CVarChar16::SetVarCharContent(const CHAR* buf)
{
   if(buf)
      SetVarCharContent(buf,strlen(buf));
}

 INT CVarChar16::size() const
{
   //modified by lxm. 2008.12.09. 使用实际长度进行有效编解码。避免编码过长，减少网络传输开销。
   return ( sizeof(BYTE) +  m_ucVarCharLen ); //等价于 ( sizeof(INT) + sizeof(CHAR) * m_ucVarCharLen );
   //注意，对于CVarChar16，不论m_ucVarCharLen是多少，字符串空间都是17.其他类似。
   //return ( sizeof(INT) + 17);
}

//
//added by LXM. 2009-02-27
//
CHAR*  CVarChar16::c_str() const
{
   //相当于GetVarCharContentPoint
   return m_cVarCharContent;
};

INT   CVarChar16::length() const
{
   //相当于GetVarCharLen
   return m_ucVarCharLen;
}

void CVarChar16::set(const CHAR* buf,INT len)
{
   SetVarCharContent(buf,len);
}

void CVarChar16::set(const CHAR* buf)
{
   if(buf)
      SetVarCharContent(buf,strlen(buf));

}



BOOL CVarChar16::operator == (const CVarChar16 &varchar2)
{
   BOOL result=FALSE;
   if (m_ucVarCharLen==varchar2.m_ucVarCharLen)
   {
      result=TRUE;
      for (INT i=0;i<m_ucVarCharLen ;i++ )
      {
         if(m_cVarCharContent[i]!=varchar2.m_cVarCharContent[i])
         {
            result=FALSE;
            break;
         }
      }
   }
   return result;
}

INT CVarChar16::decode(CHAR* &buf)
{
   DECODE(m_ucVarCharLen,buf);
   CHECK_DECODE_VCHAR_SIZE_U(m_ucVarCharLen,16)
   DECODE_ARRAY_U(m_cVarCharContent,buf,m_ucVarCharLen);
   m_cVarCharContent[m_ucVarCharLen]=0;

   return size();
}

INT CVarChar16::encode(CHAR* &buf) const
{
   ENCODE(buf,m_ucVarCharLen);
   ENCODE_ARRAY(buf,m_cVarCharContent,m_ucVarCharLen);

   return size();
}

void CVarChar16::print(ostrstream &st)
{

  st << "{CVarChar16:";
  st << "m_ucVarCharLen=" << (INT)m_ucVarCharLen << ";";
  st << "m_cVarCharContent=" << (CHAR*)m_cVarCharContent << ";";
  st << "}";
}

/*************************************************************/

 CVarChar32::CVarChar32()
{
   m_cVarCharContent = NULL;
   m_cVarCharContent = new CHAR[33];
   m_ucVarCharLen = 0;
   m_cVarCharContent[0] = '\0';
   m_cVarCharContent[32] = '\0';
}

 CVarChar32::CVarChar32(const CVarChar32 &r)
{
   m_cVarCharContent = NULL;
   m_cVarCharContent = new CHAR[33];

   m_ucVarCharLen = r.m_ucVarCharLen;
   memcpy_safec(m_cVarCharContent, r.m_cVarCharContent, 33);
}

 CVarChar32 &CVarChar32::operator=(const CVarChar32 &r)
{
   m_ucVarCharLen = r.m_ucVarCharLen;
   memcpy_safe(m_cVarCharContent, r.m_cVarCharContent, 33);
   return *this;
}

//added by lxm.2009-02-27。 可以直接赋值为字符串
CVarChar32 &CVarChar32::operator=(const CHAR* r)
{
   if(r)
   {
      set(r);
   }
   return *this;

}

CVarChar32 &CVarChar32::operator=(const CFixedChar &r)
{
   set(r.c_str(),r.length());
   return *this;
}

CVarChar32 &CVarChar32::operator=(const CVarChar8 &r)
{
   set(r.c_str(),r.length());
   return *this;
}

CVarChar32 &CVarChar32::operator=(const CVarChar16 &r)
{
   set(r.c_str(),r.length());
   return *this;
}

CVarChar32 &CVarChar32::operator=(const CVarChar &r)
{
   set(r.c_str(),r.length());
   return *this;
}


INT CVarChar32::GetVarCharLen() const
{
   return m_ucVarCharLen;
}

 void CVarChar32::SetVarCharLen(INT len)
{
   m_ucVarCharLen=len;
}

 CHAR CVarChar32::GetVarCharContent(INT num)
{
   if ((num>m_ucVarCharLen) || (num<0))
      return 0;
   else
      return m_cVarCharContent[num];
}

 void CVarChar32::SetVarCharContent(INT num,CHAR character)
{
   if ((num<=m_ucVarCharLen) && (num>=0))
      m_cVarCharContent[num]=character;
}

 CHAR* CVarChar32::GetVarCharContentPoint() const
{
   return (CHAR*)m_cVarCharContent;
}

 void CVarChar32::SetVarCharContent(const CHAR* buf, INT len)
{
   if (buf && len > 0) //len<=32)
   {
      if(len>32) len = 32;
      memcpy(GetVarCharContentPoint(),buf,len);
      m_ucVarCharLen=len;
      m_cVarCharContent[len]='\0';
   }
}

 void CVarChar32::SetVarCharContent(const CHAR* buf)
{
   if(buf)
      SetVarCharContent(buf,strlen(buf));
}


 INT CVarChar32::size() const
{
   //modified by lxm. 2008.12.09. 使用实际长度进行有效编解码。避免编码过长，减少网络传输开销。
   return ( sizeof(BYTE) +  m_ucVarCharLen );
   //return ( sizeof(INT) + 33);
}


//
//added by LXM. 2009-02-27
//
CHAR*  CVarChar32::c_str() const
{
   //相当于GetVarCharContentPoint
   return m_cVarCharContent;
};

INT   CVarChar32::length() const
{
   //相当于GetVarCharLen
   return m_ucVarCharLen;
}

void CVarChar32::set(const CHAR* buf,INT len)
{
   SetVarCharContent(buf,len);
}

void CVarChar32::set(const CHAR* buf)
{
   if(buf)
      SetVarCharContent(buf,strlen(buf));

}


BOOL CVarChar32::operator == ( const CVarChar32 &varchar2)
{
   BOOL result=FALSE;
   if (m_ucVarCharLen==varchar2.m_ucVarCharLen)
   {
      result=TRUE;
      for (INT i=0;i<m_ucVarCharLen ;i++ )
      {
         if(m_cVarCharContent[i]!=varchar2.m_cVarCharContent[i])
         {
            result=FALSE;
            break;
         }
      }
   }
   return result;
}

INT CVarChar32::decode(CHAR* &buf)
{
   DECODE(m_ucVarCharLen,buf);
   CHECK_DECODE_VCHAR_SIZE_U(m_ucVarCharLen,32)
   DECODE_ARRAY_U(m_cVarCharContent,buf,m_ucVarCharLen);
   m_cVarCharContent[m_ucVarCharLen]=0;

   return size();
}

INT CVarChar32::encode(CHAR* &buf) const
{
   ENCODE(buf,m_ucVarCharLen);
   ENCODE_ARRAY(buf,m_cVarCharContent,m_ucVarCharLen);

   return size();
}

void CVarChar32::print(ostrstream &st)
{

  st << "{CVarChar32:";
  st << "m_ucVarCharLen=" << (INT)m_ucVarCharLen << ";";
  st << "m_cVarCharContent=" << (CHAR*)m_cVarCharContent << ";";
  st << "}";
}

/*************************************************************/

 CVarChar64::CVarChar64()
{
   m_cVarCharContent = NULL;
   m_cVarCharContent = new CHAR[65];
   m_ucVarCharLen = 0;
   m_cVarCharContent[0] = '\0';
   m_cVarCharContent[64] = '\0';
}

 CVarChar64::CVarChar64(const CVarChar64 &r)
{
   m_cVarCharContent = NULL;
   m_cVarCharContent = new CHAR[65];
   m_ucVarCharLen = r.m_ucVarCharLen;
   memcpy_safec(m_cVarCharContent, r.m_cVarCharContent, 65);
}

 CVarChar64 &CVarChar64::operator=(const CVarChar64 &r)
{
   m_ucVarCharLen = r.m_ucVarCharLen;
   memcpy_safe(m_cVarCharContent, r.m_cVarCharContent, 65);
   return *this;
}

//added by lxm.2009-02-27。 可以直接赋值为字符串
CVarChar64 &CVarChar64::operator=(const CHAR* r)
{
   if(r)
   {
      set(r);
   }
   return *this;

}

CVarChar64 &CVarChar64::operator=(const CFixedChar &r)
{
   set(r.c_str(),r.length());
   return *this;
}

CVarChar64 &CVarChar64::operator=(const CVarChar8 &r)
{
   set(r.c_str(),r.length());
   return *this;
}

CVarChar64 &CVarChar64::operator=(const CVarChar16 &r)
{
   set(r.c_str(),r.length());
   return *this;
}

CVarChar64 &CVarChar64::operator=(const CVarChar32 &r)
{
   set(r.c_str(),r.length());
   return *this;
}

CVarChar64 &CVarChar64::operator=(const CVarChar &r)
{
   set(r.c_str(),r.length());
   return *this;
}

INT CVarChar64::GetVarCharLen() const
{
   return m_ucVarCharLen;
}

 void CVarChar64::SetVarCharLen(INT len)
{
   m_ucVarCharLen=len;
}

 CHAR CVarChar64::GetVarCharContent(INT num)
{
   if ((num>m_ucVarCharLen) || (num<0))
      return 0;
   else
      return m_cVarCharContent[num];
}

 void CVarChar64::SetVarCharContent(INT num,CHAR character)
{
   if ((num<=m_ucVarCharLen) && (num>=0))
      m_cVarCharContent[num]=character;
}

 CHAR* CVarChar64::GetVarCharContentPoint() const
{
   return (CHAR*)m_cVarCharContent;
}

 void CVarChar64::SetVarCharContent(const CHAR* buf, INT len)
{
   if (buf && len> 0)//len<=64)
   {
      if(len>64) len = 64;
      memcpy(m_cVarCharContent,buf,len);
      m_ucVarCharLen=len;
      m_cVarCharContent[len]='\0';
   }
}

void CVarChar64::SetVarCharContent(const CHAR* buf)
{
   if(buf)
      SetVarCharContent(buf,strlen(buf));
}

INT CVarChar64::size() const
{
   //modified by lxm. 2008.12.09. 使用实际长度进行有效编解码。避免编码过长，减少网络传输开销。
   return ( sizeof(BYTE) +  m_ucVarCharLen );
   //return ( sizeof(INT) + 65 );
}


//
//added by LXM. 2009-02-27
//
CHAR*  CVarChar64::c_str() const
{
   //相当于GetVarCharContentPoint
   return m_cVarCharContent;
};

INT   CVarChar64::length() const
{
   //相当于GetVarCharLen
   return m_ucVarCharLen;
}

void CVarChar64::set(const CHAR* buf,INT len)
{
   SetVarCharContent(buf,len);
}

void CVarChar64::set(const CHAR* buf)
{
   if(buf)
      SetVarCharContent(buf,strlen(buf));

}


BOOL CVarChar64::operator == (const CVarChar64 &varchar2)
{
   BOOL result=FALSE;
   if (m_ucVarCharLen==varchar2.m_ucVarCharLen)
   {
      result=TRUE;
      for (INT i=0;i<m_ucVarCharLen ;i++ )
      {
         if(m_cVarCharContent[i]!=varchar2.m_cVarCharContent[i])
         {
            result=FALSE;
            break;
         }
      }
   }
   return result;
}

INT CVarChar64::decode(CHAR* &buf)
{
   DECODE(m_ucVarCharLen,buf);
   CHECK_DECODE_VCHAR_SIZE_U(m_ucVarCharLen,64)
   DECODE_ARRAY_U(m_cVarCharContent,buf,m_ucVarCharLen);
   m_cVarCharContent[m_ucVarCharLen]=0;

   return size();
}

INT CVarChar64::encode(CHAR* &buf) const
{
   ENCODE(buf,m_ucVarCharLen);
   ENCODE_ARRAY(buf,m_cVarCharContent,m_ucVarCharLen);

   return size();
}

void CVarChar64::print(ostrstream &st)
{

  st << "{CVarChar64:";
  st << "m_ucVarCharLen=" << (INT)m_ucVarCharLen << ";";
  st << "m_cVarCharContent=" << (CHAR*)m_cVarCharContent << ";";
  st << "}";
}

/*************************************************************/

 CVarChar128::CVarChar128()
{
   m_cVarCharContent = NULL;
   m_cVarCharContent = new CHAR[129];
   m_ucVarCharLen = 0;
   m_cVarCharContent[0] = '\0';
   m_cVarCharContent[128] = '\0';
}

 CVarChar128::CVarChar128(const CVarChar128 &r)
{
   m_cVarCharContent = NULL;
   m_cVarCharContent = new CHAR[129];
   m_ucVarCharLen = r.m_ucVarCharLen;
   memcpy_safec(m_cVarCharContent, r.m_cVarCharContent, 129);
}

 CVarChar128 &CVarChar128::operator=(const CVarChar128 &r)
{
   m_ucVarCharLen = r.m_ucVarCharLen;
   memcpy_safe(m_cVarCharContent, r.m_cVarCharContent, 129);
   return *this;
}

//added by lxm.2009-02-27。 可以直接赋值为字符串
CVarChar128 &CVarChar128::operator=(const CHAR* r)
{
   if(r)
   {
      set(r);
   }
   return *this;

}

CVarChar128 &CVarChar128::operator=(const CFixedChar &r)
{
   set(r.c_str(),r.length());
   return *this;
}

CVarChar128 &CVarChar128::operator=(const CVarChar8 &r)
{
   set(r.c_str(),r.length());
   return *this;
}

CVarChar128 &CVarChar128::operator=(const CVarChar16 &r)
{
   set(r.c_str(),r.length());
   return *this;
}

CVarChar128 &CVarChar128::operator=(const CVarChar32 &r)
{
   set(r.c_str(),r.length());
   return *this;
}

CVarChar128 &CVarChar128::operator=(const CVarChar64 &r)
{
   set(r.c_str(),r.length());
   return *this;
}

CVarChar128 &CVarChar128::operator=(const CVarChar &r)
{
   set(r.c_str(),r.length());
   return *this;
}

INT CVarChar128::GetVarCharLen() const
{
   return m_ucVarCharLen;
}

 void CVarChar128::SetVarCharLen(INT len)
{
   m_ucVarCharLen=len;
}

 CHAR CVarChar128::GetVarCharContent(INT num)
{
   if ((num>m_ucVarCharLen) || (num<0))
      return 0;
   else
      return m_cVarCharContent[num];
}

 void CVarChar128::SetVarCharContent(INT num,CHAR character)
{
   if ((num<=m_ucVarCharLen) && (num>=0))
      m_cVarCharContent[num]=character;
}

 CHAR* CVarChar128::GetVarCharContentPoint() const
{
   return (CHAR*)m_cVarCharContent;
}

void CVarChar128::SetVarCharContent(const CHAR* buf, INT len)
{
   if (buf && len>0) //&& len<=128)
   {
      if(len>128) len = 128;
      memcpy(m_cVarCharContent,buf,len);
      m_ucVarCharLen=len;
      m_cVarCharContent[len]='\0';
   }

}

void CVarChar128::SetVarCharContent(const CHAR* buf)
{
   if(buf)
      SetVarCharContent(buf,strlen(buf));
}


INT CVarChar128::size() const
{
   //modified by lxm. 2008.12.09. 使用实际长度进行有效编解码。避免编码过长，减少网络传输开销。
   return ( sizeof(BYTE) + m_ucVarCharLen );
   //return ( sizeof(INT) + 129 );
}


//
//added by LXM. 2009-02-27
//
CHAR*  CVarChar128::c_str() const
{
   //相当于GetVarCharContentPoint
   return m_cVarCharContent;
};

INT   CVarChar128::length() const
{
   //相当于GetVarCharLen
   return m_ucVarCharLen;
}

void CVarChar128::set(const CHAR* buf,INT len)
{
   SetVarCharContent(buf,len);
}

void CVarChar128::set(const CHAR* buf)
{
   if(buf)
      SetVarCharContent(buf,strlen(buf));

}


BOOL CVarChar128::operator == (const CVarChar128 &varchar2)
{
   BOOL result=FALSE;
   if (m_ucVarCharLen==varchar2.m_ucVarCharLen)
   {
      result=TRUE;
      for (INT i=0;i<m_ucVarCharLen ;i++ )
      {
         if(m_cVarCharContent[i]!=varchar2.m_cVarCharContent[i])
         {
            result=FALSE;
            break;
         }
      }
   }
   return result;
}

INT CVarChar128::decode(CHAR* &buf)
{
   DECODE(m_ucVarCharLen,buf);
   CHECK_DECODE_VCHAR_SIZE_U(m_ucVarCharLen,128)
   DECODE_ARRAY_U(m_cVarCharContent,buf,m_ucVarCharLen);
   m_cVarCharContent[m_ucVarCharLen]=0;

   return size();
}

INT CVarChar128::encode(CHAR* &buf) const
{
   ENCODE(buf,m_ucVarCharLen);
   ENCODE_ARRAY(buf,m_cVarCharContent,m_ucVarCharLen);

   return size();
}

void CVarChar128::print(ostrstream &st)
{

  st << "{CVarChar128:";
  st << "m_ucVarCharLen=" << (INT)m_ucVarCharLen << ";";
  st << "m_cVarCharContent=" << (CHAR*)m_cVarCharContent << ";";
  st << "}";
}

/*************************************************************/

 CVarChar256::CVarChar256()
{
   m_cVarCharContent = NULL;
   m_cVarCharContent = new CHAR[257];
   m_ucVarCharLen = 0;
   m_cVarCharContent[0] = '\0';
   m_cVarCharContent[256] = '\0';
}

 CVarChar256::CVarChar256(const CVarChar256 &r)
{
   m_cVarCharContent = NULL;
   m_cVarCharContent = new CHAR[257];
   m_ucVarCharLen = r.m_ucVarCharLen;
   memcpy_safec(m_cVarCharContent, r.m_cVarCharContent, 257);
}

 CVarChar256 &CVarChar256::operator=(const CVarChar256 &r)
{
   m_ucVarCharLen = r.m_ucVarCharLen;
   memcpy_safe(m_cVarCharContent, r.m_cVarCharContent, 257);
   return *this;
}


//added by lxm.2009-02-27。 可以直接赋值为字符串
CVarChar256 &CVarChar256::operator=(const CHAR* r)
{
   if(r)
   {
      set(r);
   }
   return *this;

}

CVarChar256 &CVarChar256::operator=(const CFixedChar &r)
{
   set(r.c_str(),r.length());
   return *this;
}

CVarChar256 &CVarChar256::operator=(const CVarChar8 &r)
{
   set(r.c_str(),r.length());
   return *this;
}

CVarChar256 &CVarChar256::operator=(const CVarChar16 &r)
{
   set(r.c_str(),r.length());
   return *this;
}

CVarChar256 &CVarChar256::operator=(const CVarChar32 &r)
{
   set(r.c_str(),r.length());
   return *this;
}

CVarChar256 &CVarChar256::operator=(const CVarChar64 &r)
{
   set(r.c_str(),r.length());
   return *this;
}

CVarChar256 &CVarChar256::operator=(const CVarChar128 &r)
{
   set(r.c_str(),r.length());
   return *this;
}

CVarChar256 &CVarChar256::operator=(const CVarChar &r)
{
   set(r.c_str(),r.length());
   return *this;
}

INT CVarChar256::GetVarCharLen() const
{
   return m_ucVarCharLen;
}

 void CVarChar256::SetVarCharLen(INT len)
{
   if(len>255) len = 255;
   m_ucVarCharLen=len;
}

 CHAR CVarChar256::GetVarCharContent(INT num)
{
   if ((num>m_ucVarCharLen) || (num<0))
      return 0;
   else
      return m_cVarCharContent[num];
}

 void CVarChar256::SetVarCharContent(INT num,CHAR character)
{
   if ((num<=m_ucVarCharLen) && (num>=0))
      m_cVarCharContent[num]=character;
}

 CHAR* CVarChar256::GetVarCharContentPoint() const
{
   return (CHAR*)m_cVarCharContent;
}

 void CVarChar256::SetVarCharContent(const CHAR* buf, INT len)
{
   //modified by lxm. 2009-02-26. 由于m_ucVarCharLen为BYTE类型，故最多255字节。
   //if (len<=256)
   if(buf && len >0) //len <= 255)
   {
      if(len>255) len = 255;
      memcpy(m_cVarCharContent,buf,len);
      m_ucVarCharLen=len;
      m_cVarCharContent[len]='\0';
   }

}

void CVarChar256::SetVarCharContent(const CHAR* buf)
{
   if(buf)
      SetVarCharContent(buf,strlen(buf));
}

 INT CVarChar256::size() const
{
   //modified by lxm. 2008.12.09. 使用实际长度进行有效编解码。避免编码过长，减少网络传输开销。
   //return ( sizeof(INT) +  m_ucVarCharLen );
    return ( sizeof(BYTE) +  m_ucVarCharLen );//modified by lxm. 2009-02-26.
   //return ( sizeof(INT) + 257 );
}


//
//added by LXM. 2009-02-26
//
CHAR*  CVarChar256::c_str() const
{
   //相当于GetVarCharContentPoint
   return m_cVarCharContent;
};

INT   CVarChar256::length() const
{
   //相当于GetVarCharLen
   return m_ucVarCharLen;
}

void CVarChar256::set(const CHAR* buf,INT len)
{
   SetVarCharContent(buf,len);
}

void CVarChar256::set(const CHAR* buf)
{
   if(buf)
      SetVarCharContent(buf,strlen(buf));

}


BOOL CVarChar256::operator == (const CVarChar256 &varchar2)
{
   BOOL result=FALSE;
   if (m_ucVarCharLen==varchar2.m_ucVarCharLen)
   {
      result=TRUE;
      for (INT i=0;i<m_ucVarCharLen ;i++ )
      {
         if(m_cVarCharContent[i]!=varchar2.m_cVarCharContent[i])
         {
            result=FALSE;
            break;
         }
      }
   }
   return result;
}

INT CVarChar256::decode(CHAR* &buf)
{
   //modified by lxm. 2009-02-26
   //DECODE_INT(m_ucVarCharLen,buf);
   DECODE(m_ucVarCharLen,buf);
   // CHECK_DECODE_VCHAR_SIZE_U(m_ucVarCharLen,256) // unsigned char always 0-255
   DECODE_ARRAY_U(m_cVarCharContent,buf,m_ucVarCharLen);
   m_cVarCharContent[m_ucVarCharLen]=0;

   return size();
}

INT CVarChar256::encode(CHAR* &buf) const
{
   //modified by lxm. 2009-02-26
   //ENCODE_INT(buf,m_ucVarCharLen);
   ENCODE(buf,m_ucVarCharLen);
   ENCODE_ARRAY(buf,m_cVarCharContent,m_ucVarCharLen);

   return size();
}

void CVarChar256::print(ostrstream &st)
{

  st << "{CVarChar256:";
  st << "m_ucVarCharLen=" << (INT)m_ucVarCharLen << ";";
  st << "m_cVarCharContent=" << (CHAR*)m_cVarCharContent << ";";
  st << "}";
}


/*************************************************************/

 CVarChar::CVarChar()
{
   m_cVarCharContent = NULL;
   m_cVarCharContent = new CHAR[1];//NULL;
   m_ucVarCharLen = 0;
   m_cVarCharContent[0]=0;

}

 CVarChar::CVarChar(const CVarChar &r)
{
   m_cVarCharContent = NULL;
   m_ucVarCharLen = r.m_ucVarCharLen;
   m_cVarCharContent = new CHAR[m_ucVarCharLen+1];
   memcpy_safec(m_cVarCharContent, r.m_cVarCharContent, m_ucVarCharLen+1);
}

 CVarChar &CVarChar::operator=(const CVarChar &r)
{
   if ( m_cVarCharContent != NULL ) delete[] m_cVarCharContent;
   m_ucVarCharLen = r.m_ucVarCharLen;
   m_cVarCharContent = new CHAR[m_ucVarCharLen+1];
   memcpy_safe(m_cVarCharContent, r.m_cVarCharContent, m_ucVarCharLen+1);
   return *this;
}

//added by lxm.2009-02-27。 可以直接赋值为字符串
CVarChar &CVarChar::operator=(const CHAR* r)
{
   if(r)
   {
      set(r);
   }
   return *this;

}

CVarChar &CVarChar::operator=(const CFixedChar &r)
{
   set(r.c_str(),r.length());
   return *this;
}

CVarChar &CVarChar::operator=(const CVarChar8 &r)
{
   set(r.c_str(),r.length());
   return *this;
}


CVarChar &CVarChar::operator=(const CVarChar16 &r)
{
   set(r.c_str(),r.length());
   return *this;
}

CVarChar &CVarChar::operator=(const CVarChar32 &r)
{
   set(r.c_str(),r.length());
   return *this;
}

CVarChar &CVarChar::operator=(const CVarChar64 &r)
{
   set(r.c_str(),r.length());
   return *this;
}

CVarChar &CVarChar::operator=(const CVarChar128 &r)
{
   set(r.c_str(),r.length());
   return *this;
}

CVarChar &CVarChar::operator=(const CVarChar256 &r)
{
   set(r.c_str(),r.length());
   return *this;
}

INT CVarChar::GetVarCharLen() const
{
   return m_ucVarCharLen;
}

 void CVarChar::SetVarCharLen(INT len)
{
   //modified by Long Xiangming.2008.03.13
   if (len == 0)
   {
      if ( m_cVarCharContent!=NULL ) delete[] m_cVarCharContent;
      //m_cVarCharContent=NULL;
      m_cVarCharContent = new CHAR[1];
      m_cVarCharContent[0]=0;
      m_ucVarCharLen=0;
   }
   else if ( len > 0 && len <= m_ucVarCharLen )
   {
      m_ucVarCharLen=len;
      m_cVarCharContent[len]='\0';
   }
   else if(len > m_ucVarCharLen && len <= MAX_VCHAR_LENGTH )
   {
      if ( m_cVarCharContent!=NULL ) delete[] m_cVarCharContent;
      m_cVarCharContent = new CHAR[len+1];
      m_cVarCharContent[0]=0;
      m_ucVarCharLen=len;

   }
   else
   {
      UniERROR("ERROR in CVarChar::SetVarCharLen(): The length exeeds limit: %d(%d).",MAX_VCHAR_LENGTH,len);
      //do nothing.
   }
}

 CHAR CVarChar::GetVarCharContent(INT num)
{
   if ((num>m_ucVarCharLen) || (num<0))
      return 0;
   else
      return m_cVarCharContent[num];
}

 void CVarChar::SetVarCharContent(INT num,CHAR character)
{
   if ((num<=m_ucVarCharLen) && (num>=0))
      m_cVarCharContent[num]=character;
}

 CHAR* CVarChar::GetVarCharContentPoint() const
{
   return (CHAR*)m_cVarCharContent;
}

void CVarChar::SetVarCharContent(const CHAR* buf, INT len)
{
   if(buf==NULL) return;
   if ( len >0 && len <= MAX_VCHAR_LENGTH)
   {
      if (m_cVarCharContent!=NULL) delete[] m_cVarCharContent;
      m_cVarCharContent = new CHAR[len+1];
      memcpy(m_cVarCharContent,buf,len);
      m_ucVarCharLen=len;
      m_cVarCharContent[len]='\0';
   }
   else if(len > MAX_VCHAR_LENGTH)
   {
      //截取前面MAX_VCHAR_LENGTH个
      if (m_cVarCharContent!=NULL) delete[] m_cVarCharContent;
      m_cVarCharContent = new CHAR[MAX_VCHAR_LENGTH+1];
      memcpy(m_cVarCharContent,buf,MAX_VCHAR_LENGTH);
      m_ucVarCharLen=MAX_VCHAR_LENGTH;
      m_cVarCharContent[MAX_VCHAR_LENGTH]='\0';
      UniERROR("WARNING: The CVarChar content length exceeds limit: %d(%d).",MAX_VCHAR_LENGTH,len);
   }
   else
      m_cVarCharContent[0]=0;

}

void CVarChar::SetVarCharContent(const CHAR* buf)
{
   if(buf)
      SetVarCharContent(buf,strlen(buf));
}


//
//added by LXM. 2009-02-27
//
CHAR*  CVarChar::c_str() const
{
   //相当于GetVarCharContentPoint
   return m_cVarCharContent;
};

INT   CVarChar::length() const
{
   //相当于GetVarCharLen
   return m_ucVarCharLen;
}

void CVarChar::set(const CHAR* buf,INT len)
{
   SetVarCharContent(buf,len);
}

void CVarChar::set(const CHAR* buf)
{
   if(buf)
      SetVarCharContent(buf,strlen(buf));

}



INT CVarChar::size() const
{
   if (m_cVarCharContent==NULL) return sizeof(INT);
   //modified by lxm. 2008.12.09. 最后一个空字符'\0' 不会编码进去。
   return ( sizeof(INT) + m_ucVarCharLen  ); //return ( sizeof(INT) + m_ucVarCharLen + 1 );
}



BOOL CVarChar::operator == (const CVarChar &varchar2)
{
   BOOL result=false;
   if (m_ucVarCharLen==varchar2.m_ucVarCharLen)
   {
      if (m_cVarCharContent==NULL && varchar2.m_cVarCharContent==NULL) return TRUE;

      result=TRUE;
      for (INT i=0;i<m_ucVarCharLen ;i++ )
      {
         if(m_cVarCharContent[i]!=varchar2.m_cVarCharContent[i])
         {
            result=FALSE;
            break;
         }
      }
   }
   return result;
}

INT CVarChar::decode(CHAR* &buf)
{
   m_ucVarCharLen=0;

   DECODE_INT(m_ucVarCharLen,buf);
   if(m_ucVarCharLen>0 && m_ucVarCharLen<=MAX_VCHAR_LENGTH)
   {
      if (m_cVarCharContent!=NULL)
      {
         delete[] m_cVarCharContent;
         m_cVarCharContent=NULL;
      }
      m_cVarCharContent=new CHAR[m_ucVarCharLen+1];
      if (m_cVarCharContent == NULL)
      {
         m_cVarCharContent = new CHAR[1];
         m_cVarCharContent[0]=0;
         m_ucVarCharLen=0;
         UniERROR("ERROR in CVarChar::decode(): new CHAR[%d] failed.",m_ucVarCharLen+1);
         return -1;
      }
      //DECODE_ARRAY会用MAX_ARRAY_SIZE作限制
      //modified by Long Xiangming. 2008.04.02
      //DECODE_ARRAY(m_cVarCharContent,buf,m_ucVarCharLen);
      //CHECK_DECODE_ARRAY_SIZE(m_ucVarCharLen,MAX_VCHAR_LENGTH,CVarChar,CVarChar)
      memcpy( m_cVarCharContent, buf, m_ucVarCharLen );
      buf += m_ucVarCharLen;

      m_cVarCharContent[m_ucVarCharLen]=0;
   }
   else if(m_ucVarCharLen<0 || m_ucVarCharLen>MAX_VCHAR_LENGTH)
   {
      UniERROR("ERROR in CVarChar::decode(): length is invalid: %d. MAX_VCHAR_LENGTH: %d", m_ucVarCharLen,MAX_VCHAR_LENGTH);
      m_ucVarCharLen=0;
      m_cVarCharContent[0]=0;
   }
   else
   {
      m_ucVarCharLen=0;
      m_cVarCharContent[0]=0;
   }

   return size();
}

INT CVarChar::encode(CHAR* &buf) const
{
   ENCODE_INT(buf,m_ucVarCharLen);
   if ( m_ucVarCharLen>0 )
   {
      ENCODE_ARRAY(buf,m_cVarCharContent,m_ucVarCharLen);
   }
   return size();
}

void CVarChar::print(ostrstream &st)
{

  st << "{CVarChar:";
  st << "m_ucVarCharLen=" << (INT)m_ucVarCharLen << ";";
  if ( m_ucVarCharLen>0 )
  {
     st << "m_cVarCharContent=" << (CHAR*)m_cVarCharContent << ";";
  }
  st << "}";
}

CStr::CStr()
{
   m_buffer =&_m_buffer[0];
   m_buffer[0]= 0;
   m_length=0;
   m_capacity=MaxCStrBufferLen-1;
}

CStr::CStr(const char* str)
{
   if(str==NULL)
   {
      m_buffer =&_m_buffer[0];
      m_buffer[0]= 0;
      m_length=0;
      m_capacity=MaxCStrBufferLen-1;
   }
   else
   {
      m_length = strlen(str);
      if(m_length>MaxCStrBufferLen-1)
      {
         m_buffer=new char[m_length+1];
         m_capacity=m_length;
      }
      else
      {
         m_buffer = &_m_buffer[0];
         m_capacity=MaxCStrBufferLen-1;
      }
      memcpy(m_buffer, str, m_length+1);
   }
}

CStr::CStr(const CStr& str)
{
   m_buffer =&_m_buffer[0];
   m_buffer[0]= 0;
   m_length=0;
   m_capacity=MaxCStrBufferLen-1;
   m_length = str.length();
   if(m_length>m_capacity)
   {
         m_buffer=new char[m_length+1];
         m_capacity=m_length;
   }
   memcpy(m_buffer, str.c_str(), str.length()+1);
}

CStr::CStr(int val)
{
   m_buffer=&_m_buffer[0];
   m_buffer[0]=0;

   m_capacity=MaxCStrBufferLen-1;
   sprintf(m_buffer,"%d",val);
   m_length=strlen(m_buffer);
}

CStr::~CStr()
{
   if(m_buffer!=&_m_buffer[0])
   {
      delete []m_buffer;
   }
}

char* CStr::steal(int& len)
{
   len = m_length;
   char* rStr;
   if(m_buffer!=&_m_buffer[0])
   {
      rStr = m_buffer;
      m_buffer = &_m_buffer[0];
      m_capacity = MaxCStrBufferLen-1;
   }
   else
   {
      rStr = new char[m_length+1];
      memcpy(rStr, m_buffer, m_length);
      rStr[m_length]=0;
   }
   m_buffer[0]=0;
   m_length=0;
   return rStr;
}

char CStr::getByte(int pos)
{
   if(pos>=m_length || pos < -m_length)
      return 0;
   if(pos>=0)
      return m_buffer[pos];
   return m_buffer[m_length+pos];
}

void CStr::SetVarCharContent(INT num,CHAR character)
{
   if ((num<=m_length) && (num>=0))
      m_buffer[num]=character;
}

void CStr::setByte(char c, int pos)
{
   if(pos>=m_length || pos < -m_length)
      return;
   if(pos>=0)
      m_buffer[pos]=c;
   else
      m_buffer[m_length+pos]=c;
}

const char* CStr::trim()
{
   char* p = m_buffer;
   while(isspace(*p))
      p++;

   if(p != m_buffer);
   {
      strcpy(m_buffer, p);
      m_length -= (p - m_buffer);
   }

   p = m_buffer + m_length - 1;
   while(p >= m_buffer && isspace(*p))
      p--;
   *(p+1)=0;
   m_length = p - m_buffer + 1;
   return m_buffer;
}

const char* CStr::trimAll()
{
   char* p = m_buffer;
   char* q = m_buffer;
   while(*p!=0)
   {
      if(isalnum(*p))
      {
         *q = *p;
         q++;
      }
      p++;
   }
   *q = 0;
   m_length = q - m_buffer;
   return m_buffer;
}

void CStr::cut(const char* sep, CStr& result)
{
   char* p = strstr(m_buffer, sep);
   if(p==NULL)
   {
      result = *this;
      clear();
      return;
   }
   result.clear();
   result.nCat(m_buffer, p-m_buffer);
   *this = p+strlen(sep);
}

const char* CStr::cut(int length)
{
   if (length >= m_length || length < -m_length)
      return m_buffer;
   if(length>=0)
   {
      m_length = length;
      m_buffer[m_length]=0;
   }
   else
   {
      m_length += length;
      m_buffer[m_length]=0;
   }
   return m_buffer;
}

void CStr::clear()
{
   m_length=0;
   m_buffer[0]=0;
}

void CStr::operator=(int value)
{
   sprintf(m_buffer,"%d",value);
   m_length=strlen(m_buffer);
}

void CStr::operator=(const char* str)
{
   if(str==NULL ||*str==0)
   {
      m_buffer[0]=0;
      m_length=0;
      return;
   }
   int len = strlen(str);
   if (len > m_capacity)
   {
      resize(len);
   }
   m_length = len;
   memcpy(m_buffer, str, m_length+1);
   m_buffer[m_length]=0;
}
int CStr::operator==(const char* str) const
{
   if(str == NULL)
      return m_length==0;
   return strcmp(c_str(), str) == 0;
}
int CStr::operator!=(const char* str) const
{
   return !(*this == str);
}
int CStr::operator<(const char* str) const
{
   if(str == NULL)
      return 0;
   return strcmp(m_buffer, str) < 0;
}
int CStr::operator>(const char* str) const
{
   if(str == NULL)
      return m_length>0;
   return strcmp(m_buffer, str) > 0;
}
int CStr::operator>=(const char* str) const
{
   return *this > str || *this == str;
}
int CStr::operator<=(const char* str) const
{
   return *this < str || *this == str;
}
int CStr::operator==(const CStr& str) const
{
   if(m_length != str.m_length)
      return 0;
   return memcmp(m_buffer, str.m_buffer, m_length) == 0;
}
int CStr::operator!=(const CStr& str) const
{
   return !(*this == str);
}
int CStr::operator>=(const CStr& str) const
{
   return *this >= str.c_str();
}
int CStr::operator>(const CStr& str) const
{
   return *this > str.c_str();
}
int CStr::operator<=(const CStr& str) const
{
   return *this <= str.c_str();
}
int CStr::operator<(const CStr& str) const
{
   return *this < str.c_str();
}
void CStr::operator=( const CStr& str )
{
   if (&str != this)
   {
      if (str.length() > m_capacity)
      {
         resize(str.length());
      }
      m_length = str.length();
      memcpy(m_buffer, str.m_buffer, str.m_length+1);
   }
}

void CStr::resize(int newCapacity)
{
   if (newCapacity <= m_capacity)
   {
      return;
   }

   int newStr=0;
   if(m_buffer!=&_m_buffer[0])
      newStr=1;

   char* oldBuf = m_buffer;
   int realCap=(newCapacity/MaxCStrBufferLen+1)*MaxCStrBufferLen;
   m_buffer = new char[realCap];
   memcpy(m_buffer,oldBuf,m_length+1);

   m_capacity = realCap-1;

   if(newStr)
      delete[] oldBuf;
}

CStr& CStr::operator+=(const CStr& str)
{
   if (m_capacity < m_length + str.m_length)
   {
      resize(m_length + str.m_length);
   }

   memcpy(m_buffer+m_length, str.m_buffer, str.m_length+1);
   m_length += str.m_length;
   m_buffer[m_length]=0;
   return *this;
}

CStr& CStr::operator+=(const char* str)
{
   int l = strlen(str);
   if (m_capacity < m_length + l)
   {
      resize(m_length + l);
   }

   memcpy(m_buffer+m_length, str, l+1);
   m_length += l;
   m_buffer[m_length]=0;
   return *this;
}

CStr& CStr::operator<<(CStr& str)
{
   *this += str;
   return *this;
}

CStr& CStr::operator<<(const char* str)
{
   *this += str;
   return *this;
}

CStr& CStr::operator<<(int i)
{
   char temp[16];
   sprintf(temp,"%d",i);
   const char* str = temp;
   *this += str;
   return *this;
}

CStr& CStr::operator+=(const char c)
{
   if (m_capacity < m_length + 1)
   {
      resize(m_length + 1);
   }

   m_buffer[m_length] = c;
   m_length += 1;
   m_buffer[m_length] = 0;
   return *this;
}

HASH_CODE CStr::hash()
{
   char temp[4];
   memset(temp, 0, 4);
   strncpy(temp, m_buffer, 4);
   HASH_CODE code = 0;
   memcpy((char*)(&code), temp, 4);
   return code;
}

CStr& CStr::toLower()
{
   char* p = m_buffer;
   while ((*p = tolower(*p)) != 0)
   {
      p++;
   }
   return *this;
}

CStr& CStr::toUpper()
{
   char* p = m_buffer;
   while ((*p = toupper(*p)) != 0)
   {
      p++;
   }
   return *this;
}

CStr& CStr::iconv(const char* from, const char* to)
{
   iconv_t cd = iconv_open(to, from);
   if(cd < 0)
   {
      UniERROR("iconv_open from '%s' to '%s' failed, errno = %d", from, to, errno);
      return *this;
   }
   char* r = m_buffer;
   size_t rLen = length();
   size_t wLen = rLen*3;
   char* buffer = new char[wLen];
   char* w = buffer;

   size_t saveWLen = wLen;
   int re = ::iconv(cd, &r, &rLen, &w, &wLen);
   if(re<0)
   {
      UniERROR("iconv failed, errno = %d", errno);
      delete buffer;
      return *this;
   }
   *w=0;
   int newLength = saveWLen - wLen;
   if (newLength > m_capacity)
   {
      resize(newLength);
   }
   m_length = newLength;
   memcpy(m_buffer, buffer, newLength+1);
   delete buffer;
   return *this;
}

CStr& CStr::uricEncode()
{
   const char* r = c_str();
   char* buffer = new char[length()*3+1];
   char* w = buffer;
   while(*r!=0)
   {
      if(*r=='&' || *r=='=' || *r=='+' || *r=='%')
      {
         sprintf(w,"%%%02X", *r);
         w+=3;
         r++;
         continue;
      }
      if(*r==' ')
         *w = '+';
      else
         *w = *r;
      w++;
      r++;
   }
   *w=0;
   int newLength = strlen(buffer);
   if (newLength > m_capacity)
   {
      resize(newLength);
   }
   m_length = newLength;
   memcpy(m_buffer, buffer, newLength+1);
   delete buffer;
}

CStr& CStr::uricDecode()
{
   char* r = m_buffer;
   char* w = m_buffer;
   while(*r!=0)
   {
      if(*r=='%')
      {
         r++;
         if(*r == 'u')
         {
            r++;
            int unicode = 0;
            sscanf(r, "%04x", &unicode);
            r+=4;
            CStr utf8Str;
            utf8Str.intCatUtf8(unicode);
            memcpy(w, utf8Str.c_str(), utf8Str.length());
            w += utf8Str.length();
            continue;
         }
         int temp;
         sscanf(r,"%02x", &temp);
         *w = temp;
         r+=2;
         w++;
         continue;
      }
      if(*r=='+')
         *w = ' ';
      else
         *w = *r;
      w++;
      r++;
   }
   *w=0;
   m_length = w - m_buffer;
}

CStr& CStr::htmlEncode()
{
   const char* r = c_str();
   char* buffer = new char[length()*6+1];
   char* w = buffer;
   while(*r!=0)
   {
      if(*r=='&')
      {
         memcpy(w,"&amp;",5);
         w+=5;
         r++;
         continue;
      }
      if(*r=='<')
      {
         memcpy(w,"&lt;",4);
         w+=4;
         r++;
         continue;
      }
      if(*r=='>')
      {
         memcpy(w,"&gt;",4);
         w+=4;
         r++;
         continue;
      }
      if(*r=='\'')
      {
         memcpy(w,"&apos;",6);
         w+=6;
         r++;
         continue;
      }
      if(*r=='"')
      {
         memcpy(w,"&quot;",6);
         w+=6;
         r++;
         continue;
      }
      if(*r==' ' && *(r+1)==' ')
      {
         memcpy(w,"&nbsp;",6);
         w+=6;
         r++;
         continue;
      }
      if(*r=='\n')
      {
         memcpy(w,"<br>\n",5);
         w+=5;
         r++;
         continue;
      }
      if(*r=='\r')
      {
         r++;
         continue;
      }
      *w=*r;
      *w=*r;
      w++;
      r++;
   }
   *w=0;
   int newLength = strlen(buffer);
   if (newLength > m_capacity)
   {
      resize(newLength);
   }
   m_length = newLength;
   memcpy(m_buffer, buffer, newLength+1);
   delete buffer;
   return *this;
}

CStr& CStr::fileCat(const char* fileName)
{
   int l = getFileLength(fileName);
   if(l <= 0)
      return *this;

   FILE* f = fopen(fileName, "rb");
   if(f==NULL)
      return *this;

   if (m_capacity < m_length + l)
      resize(m_length + l);

   fread(m_buffer+m_length, l, 1, f);
   m_length += l;
   m_buffer[m_length]=0;
   fclose(f);
   return *this;
}
void CStr::getFieldName(const char** p, int& arrayIndex)
{
   arrayIndex = -1;
   if(*p == NULL)
   {
      clear();
      return;
   }
   const char* pp = strstr(*p, ".");
   if(pp==NULL)
   {
      *this = *p;
      *p = NULL;
   }
   else
   {
      set(*p, pp - *p);
      *p = pp + 1;
   }
   char* p1 = strstr(m_buffer, "[");
   if(p1 != NULL)
   {
      *p1 = 0;
      m_length = strlen(m_buffer);
      p1++;
      arrayIndex = atoi(p1);
   }
}
CStr& CStr::intCatShort(int i)
{
   i = htonl(i);
   unsigned char* p = (unsigned char*)(&i);

   int len = 4;
   while(len>1 && (p[0]==0 && (p[1]&0x80)==0) || (p[0]==0xff && (p[1]&0x80)!=0))
   {
      p++;
      len--;
   }
   nCat((char*)p, len);
   return *this;
}

CStr& CStr::intCat4(int i)
{
   i = htonl(i);
   char* p = (char*)(&i);
   nCat(p, 4);
   return *this;
}

CStr& CStr::intCatUtf8(int i)
{
   unsigned char highByte = i/256;
   unsigned char lowerByte = i%256;
   if( (highByte==0)&&(lowerByte<0x80) ) //0000-007F, 1 byte 0xxxxxxx
   {
      char c = lowerByte;
      *this += c;
      return *this;
   }
   if(highByte<0x08) //0080-07FF, 2 bytes 110xxxxx 10xxxxxx
   {
      char c[2];
      c[0] = 0xC0 | ((highByte << 2) & 0x1C) | ((lowerByte >> 6) & 0x03);
      c[1] = 0x80 | (lowerByte & 0x3F);
      nCat(c, 2);
      return *this;
   }
   //0800-FFFF, 3 bytes 1110xxxx 10xxxxxx 10xxxxxx
   char c[3];
   c[0] = 0xE0 | (highByte >> 4);
   c[1] = 0x80 | ((highByte << 2) & 0x3C) | ((lowerByte >> 6) & 0x03);
   c[2] = 0x80 | (lowerByte & 0x3F);
   nCat(c, 3);
   return *this;
}

CStr& CStr::intCat7(int id)
{
static int mask[] = { 0xF0000000, 0x0FE00000, 0x001FC000, 0x00003F80, 0x0000007F};
static int bitPos[] = { 28, 21, 14, 7, 0 };
   char c;
   int begin = 0;
   for(int j=0; j<5; j++)
   {
      c = (id & mask[j]) >> bitPos[j];
      if(c!=0 || begin || j==4)
      {
         begin = 1;
         if(j!=4)
            c |= 0x80;
         *this += c;
      }
   }
   return *this;
}

CStr& CStr::nCat(const char* str, int l)
{
   if (m_capacity < m_length + l)
   {
      resize(m_length + l);
   }
   memcpy(m_buffer+m_length, str, l);
   m_length += l;
   m_buffer[m_length]=0;
   return *this;
}

CStr& CStr::fCat(const char* fmt, ... )
{
   va_list args;
   va_start(args, fmt);
   return fCatV(fmt, args);
}

CStr& CStr::fCatV(const char* fmt, va_list args)
{
   char temp[81920];
   vsprintf(temp, fmt, args);
   va_end(args);
   *this+=temp;
   return *this;
}

CStr& CStr::replace(const char* pattern, const char* replace)
{
   int patternLength = strlen(pattern);
   char* p = m_buffer;
   char* q;
   CStr destStr;
   while((q = strstr(p, pattern)) != NULL)
   {
      destStr.nCat(p, q-p);
      destStr += replace;
      p = q + patternLength;
   }
   destStr += p;
   *this = destStr;
   return *this;
}

CStr& CStr::fCatChars(char fillChar, int lineLen)
{
   int i;
   for(i = 0; i<lineLen; i++)
      *this += fillChar;
}

CStr& CStr::fCatTitle(const char* title, char fillChar, int lineLen)
{
   int titleLen = strlen(title);
   if(titleLen > lineLen - 4)
   {
      *this += title;
      return *this;
   }
   int leftLen = (lineLen - titleLen - 2) / 2;
   int rightLen = lineLen - titleLen - 2 - leftLen;
   fCatChars(fillChar, leftLen);
   *this += ' ';
   *this += title;
   *this += ' ';
   fCatChars(fillChar, rightLen);
   return *this;
}

CStr& CStr::fCatShortBin(const char* content, int length)
{
   int i;
   for(i=0; i<length; i++)
      fCat("%s%02X", i==0 ? "" : " ", (unsigned char)(content[i]));
}

CStr& CStr::fCatBin2(const char* content, int length, int indent)
{
   int i;
   for(i=0; i<length; i++)
   {
      if(i != 0 && (i%16) == 0)
      {
         *this += "\n";
         fCatChars(' ', indent);
      }
      fCat("%02X ", (unsigned char)(content[i]));
   }
}

CStr& CStr::fCatBin(const char* content, int length)
{
   int i;
   for(i=0; i<length; i++)
   {
      if((i%16) == 0)
      {
         if(i!=0)
         {
            *this += "  ";
            for(int j = i-16; j<i; j++)
               *this += isgraph(content[j])?content[j]:'.';
         }
         fCat("\n[%3d-%-3d]", i, i+16<=length-1 ? i+15 : length-1);
      }
      fCat("%s%02X ", i%8==0 ? " " : "", (unsigned char)(content[i]));
   }
   for(i=length; i<((length-1)/16+1)*16; i++)
   {
      if(i%8==0 && i%16!=0)
         *this += " ";
      *this += "   ";
   }
   *this += "  ";
   for(i=(length-1)/16*16 ; i<length; i++)
      *this += isgraph(content[i]) ? content[i] : '.';
   return *this;
}

void CStr::print(ostrstream &st)
{
   st << "{CStr:";
   st << "length=" << m_length << ";";
   if ( m_length>0 )
   {
      st << "content=" << m_buffer << ";";
   }
   st << "}";
}

CHAR* CStr::GetVarCharContentPoint() const
{
   return (CHAR*)m_buffer;
}

int CStr::size() const
{
   return sizeof(int) + m_length;
}

int CStr::decode(char* &buf)
{
   int length;
   DECODE_INT(length, buf);
   if (length > m_capacity)
   {
      resize(length);
   }
   m_length = length;
   memcpy(m_buffer, buf, m_length);
   buf += m_length;
   m_buffer[m_length]=0;
   return size();
}

int CStr::encode(char* &buf) const
{
   ENCODE_INT(buf, m_length);
   if ( m_length>0 )
   {
      ENCODE_ARRAY(buf, m_buffer, m_length);
   }
   return size();
}

const char* CStr::c_strList() const
{
static CStr strList;
   strList.clear();
   for(int i = 0; i<m_length; i++)
   {
      if(isgraph(m_buffer[i]))
         strList += m_buffer[i];
      else if(m_buffer[i] == ' ')
         strList += " ";
      else if(m_buffer[i] == '\t')
         strList += "^I";
      else if(m_buffer[i] == '\n')
         strList += "$\n";
      else if(m_buffer[i] == '\r')
         strList += "^M";
      else
         strList.fCat("\\%02X", (unsigned char)m_buffer[i]);
   }
   return strList.c_str();
}

int CStrShort::size() const
{
   return 1 + m_length;
}

int CStrShort::decode(char* &buf)
{
   int length = (unsigned char)(buf[0]);
   if (length > m_capacity)
   {
      resize(length);
   }
   m_length = length;
   memcpy(m_buffer, buf, m_length);
   buf += m_length;
   m_buffer[m_length]=0;
   return size();
}

int CStrShort::encode(char* &buf) const
{
   unsigned char shortLen;
   if(m_length>255)
   {
      shortLen = 255;
      UniERROR("CStrShort::encode: length = %d, 255 max", m_length);
   }
   else
      shortLen = m_length;
   ENCODE(buf,shortLen);
   ENCODE_ARRAY(buf,m_buffer,shortLen);
   return size();
}

void CStr::SetVarCharContent(const char* str, int length)
{
   set(str, length);
}

void CStr::set(const char* str, int length)
{
   if(length > m_capacity)
   {
      resize(length);
   }
   if(length==0)
   {
      m_buffer[0]=0;
      m_length=0;
   }
   memcpy(m_buffer, str, length);
   m_buffer[length] = 0;
   m_length = length;
}

int CStr::merge(CStr* argvArray, int argc)
{
   clear();
   for(int i=0; i<argc; i++)
   {
      if(i != 0)
         *this += " ";
      *this += argvArray[i];
   }
}

int CStr::split(CStr* argvArray, int maxArgc, const char* sep) const
{
   const char* p = c_str();
   const char* end = p + length();
   int argc=0;
   int sepLen = strlen(sep);
   while(p < end && argc < maxArgc)
   {
      const char* q = strstr(p, sep);
      if(q == NULL)
      {
         argvArray[argc] = p;
         argc++;
         break;
      }
      argvArray[argc].set(p, q-p);
      argc++;
      p = q + sepLen;
   }
   return argc;
}

int CStr::split(CStr* argvArray, int maxArgc) const
{
   const char* buffer = c_str();
   int i=0;
   int argc=0;
   int inQuota = 0;
   while(buffer[i]!=0)
   {
      while(buffer[i]==' ' || buffer[i]=='\t')
         i++;
      if(buffer[i]!=0)
      {
         while((inQuota || buffer[i]!=' ' && buffer[i]!='\t') &&buffer[i]!=0)
         {
            if(buffer[i] == '\'')
            {
               if(inQuota)
                  inQuota = 0;
               else
                  inQuota = 1;
               i++;
            }
            if(buffer[i] == '\\')
               i++;
            argvArray[argc] += buffer[i];
            i++;
         }
         argc++;
         if(argc>=MaxArgC)
            break;
      }
   }
   return argc;
}

HASH_CODE _GetHash(CStr str)
{
   str.hash();
}
int CStrInt::operator<(const char* str) const
{
   if(str == NULL)
      return 0;
   int a = atoi(m_buffer);
   int b = atoi(str);
   if(a == b)
      return strcmp(m_buffer, str) < 0;
   return a < b;
}
int CStrInt::operator>(const char* str) const
{
   if(str == NULL)
      return m_length>0;
   int a = atoi(m_buffer);
   int b = atoi(str);
   if(a == b)
      return strcmp(m_buffer, str) > 0;
   return a > b;
}
int CStrInt::operator>=(const char* str) const
{
   return *this > str || *this == str;
}
int CStrInt::operator<=(const char* str) const
{
   return *this < str || *this == str;
}
int CStrInt::operator>=(const CStr& str) const
{
   return *this >= str.c_str();
}
int CStrInt::operator>(const CStr& str) const
{
   return *this > str.c_str();
}
int CStrInt::operator<=(const CStr& str) const
{
   return *this <= str.c_str();
}
int CStrInt::operator<(const CStr& str) const
{
   return *this < str.c_str();
}
