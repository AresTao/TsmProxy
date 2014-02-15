/******************************************************************** 
 * Copyright (c)2002, by Beijing TeleStar Telecom Technology Institue 
 * All rights reserved.                                               
                                                                      
 * FileName：      cmd5.C                                       
 * System：            SoftSwitch                                    
 * SubSystem：      Call Service                         
 * Author：         Wu Jing                                
 * Date：         2003.11.24                                    
 * Version：         1.0                                           
 * Description：                                                 
        MD5加密算法                           
 *                                                                    
 * Last Modified:                                                     
        2003.11.28 完成初始版本
              by Wu Jing
        2004.05.30 使 GetDigest()的输入不限于C风格字符串那样的字符数组，而可以
            是各字节取值任意的字节数组。
              by Zhao Wenfeng
      2009.8.10 添加字符串风格的摘要输出。李静林
********************************************************************/
#include <memory.h>
#include <openssl/md5.h>

#include "cmd5.h"

CMD5::CMD5()
{
}

CMD5::~CMD5()
{}

UCHAR* CMD5::GetDigest(UCHAR *pucText, INT iLength, INT type)
{
   memset(m_ucDigest, 0, 17);
   MD5(pucText, iLength, m_ucDigest);
   
   //Modify by LJL,2009-8-10
   if(type)
   {
      CvtHex(m_ucDigest,m_hxDigest);      //Added by LJL,2009-8-10
      return m_hxDigest;
   }
   else
   {
      return m_ucDigest;
   }
}

BOOL CMD5::CheckDigest(UCHAR *pucText, INT iLength, UCHAR *pucDigest, INT type)
{
   assert(pucText != NULL);
   assert(pucDigest != NULL);

   UCHAR *pucOutput = GetDigest(pucText, iLength,type);

   //Modify by LJL,2009-8-10
   INT j=0;
   if(type)
   {
      j=32;
   }
   else
   {
      j=16;
   }
   
   for (INT i=0;i<j ;i++ )
   {
      if (pucOutput[i] != pucDigest[i])
      {
         return FALSE;
      }
   }
   return TRUE;
}

//Added by LJL,2009-8-10
void CMD5::CvtHex(const UCHAR *Bin, UCHAR *Hex)
{
   unsigned short i;
   unsigned char j;

   for (i = 0; i < 16; i++)
   {
      j = (Bin[i] >> 4) & 0xf;
      if (j <= 9)
         Hex[i*2] = (j + '0');
      else
         Hex[i*2] = (j + 'a' - 10);
      j = Bin[i] & 0xf;
      if (j <= 9)
         Hex[i*2+1] = (j + '0');
      else
         Hex[i*2+1] = (j + 'a' - 10);
    }
    Hex[32]='\0';
}

