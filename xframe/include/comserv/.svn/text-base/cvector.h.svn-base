/*********************************************************************
* Copyright (c)2005, by MT2
* All rights reserved.
* FileName：				cvector.h
* System：           		SoftSwitch
* SubSystem：	        	Common
* Author：					李静林
* Description：
        可变长度数组 
    可变数组需要初始化数组的长度，并可以使用resize改变数组的长度 。
    需要注意的是，如果用户数据类型为结构体、类等内容，且这些 结构体、类里面
    有指针，则用户得自行释放这些指针的内存，vector的析构函数只能释放自己的内存
    
    在存储与提取的时候，vector直接使用了 “=”进行赋值  

	
	  数组长度和数组有效项目数：
	  数组长度是数组初始化和resize之后的长度，或者是push的时候超出了原来数组的长度，
	    而造成的数组自动延长。
	  数组有效值长度是数组使用push方法，及赋初值方式添加的有效项目的数量，一般情况
	    下会小于数组实际大小

	  在初始化数组的时候：
	  CVector();								数组初始大小=数组有效值大小=0
	  CVector(UINT initsize);					数组初始大小=initsize；数组有效值大小=0，没有赋值
	  CVector(UINT initsize, TValue initvalue);	数组初始大小=数组有效值大小=initsize，有初值
	
	  resize(UINT newSize);					修订后的数组大小=newSize，数组有效值大小不变，没有赋值
	  resize(UINT newSize, TValue value);   修订后的数组大小=数组有效值大小=newSize，有初值

	  
	  需要注意的是，采用下标的方式进行数组的存取，并不会影响数组实际有效长度。
	  这意味着，如果采用以下方式设置数组：
	  CVector<int> v(6);
	  v[5]=10;
	  则数组实际有效值并不是6，而是0。

		remove和resize的区别在于：
		remove是实实在在的将数组的空间减小为0，释放了所有占用的内存
		resize实际上并不会影响数组占用空间，只是影响push和pop的位置

*
* Last Modified:
	2005-11-20 v1.1 李静林 建立基本文件，提供了数组初始化方法和resize方法
                         提供使用下标设置数组内容，和使用下标获得数组内容的方法 
	
    2005-11-23 v1.2 李静林 提供push_back和pop_back方法，以添加数组内容和控制数组实际
							项目数量

    2005-12-1 v1.3 李静林 添加remove方法，用以释放数组空间。调用该方法之后，数组长度变为0

	2005-12-2 v1.4 李静林 修订析构函数和remove方法，弥补忘记删除哈希表记录的BUG
	2005-12-29 v1.5 李静林  修订remove中的一个内存泄露隐患，添加了多处错误判断 
	2006-2-10 v1.6 李静林 push_back 填写位置错误
					

*********************************************************************/

#ifndef _VECTOR_H
#define _VECTOR_H

#include "comtypedef.h"
#include "unihashtable.h"
//#include "clist.h"

///////////////////////////////////////////////////////////////////////
// 定义 CVector< TValue > 类
///////////////////////////////////////////////////////////////////////
template <class TValue>
class CVector	// TValue为数组要存储的值
{
private:

	class _Entry	// 定义链表中的节点类型
	{
	public:
		TValue*		m_value;		// 此节点的内容
		_Entry*		m_prev;			// 指向前一个节点
		_Entry*		m_next;			// 指向下一个节点
    
       _Entry( TValue* pvalue, _Entry* prev, _Entry* next )
	   {
	     m_value=pvalue;
		 m_prev=prev;
		 m_next=next;
	   }
	   ~_Entry( void )
	   {

	   }
    };
    
    CHashTable<UINT, _Entry*> m_ht;
    
	_Entry* m_pHead;
	_Entry* m_pTail;
	_Entry* m_pEnd;
	_Entry* m_pCurrent;
    UINT m_count;
	UINT m_size;
	TValue* m_pValue;
	
public:
	CVector();	//建立空数组，以后可以使用resize改变
	CVector(UINT initsize);		//建立一定长度的数组
	CVector(UINT initsize, TValue initvalue);	//使用初值初始化数组
	~CVector();

	void resize(UINT newSize);		//重置数组长度，需要注意的是，如果 newSize < initSize，则数组长度不会改变
	void resize(UINT newSize, TValue value);	//重置数组长度且赋初值
	
	TValue& operator[](const UINT i);
	
	//整个数组长度
	//	整个数组长度是初始化或resize之后的数值
	UINT size(){return m_size;}	
	
	//数组有效值长度. 
	//	当前数组实际填写的项目数，超过这一项目数的数组内容实际上是无意义的
	UINT count(){return m_count;}	

    //添加一项
	//	在数组最后的有效值后面添加一项
	//	输入：添加的内容
	//	返回：数组有效项目数
	UINT push_back (const TValue& value);	

    //删除一项
	//	删除数组最后一个有效值
	//	返回：数组有效项目数
	UINT pop_back();	

	//删除一项
	//	删除数组最后一个有效值
	//	返回：数组有效项目数，数组最后一个有效值
	UINT pop_back(TValue& value);

	void remove();	//删除所有节点 

	//判断数组有效值是否为空
	BOOL empty()	
	{
		if(m_count) return FALSE;
		else return TRUE;
	}

};


template <class TValue>
CVector<TValue>::CVector()
{
	m_count=0;
	m_size=0;
	m_pValue=new TValue;
}


template <class TValue>
CVector<TValue>::CVector(UINT initsize)
{
	m_count=0;
	m_size=0;
	m_pValue=new TValue;
//del by LJL 2005-12-29 将resize修改为内联函数，构造函数的内容和resize是一致的 
/*	if(initsize>0) m_size=initsize;
	m_pValue=new TValue;
	_Entry* pEntry;
	TValue* pValue;
	pEntry=NULL;
	pValue=NULL;
	for(int i=0; i<m_size; i++)
    {
        pValue=new TValue;
        pEntry=new _Entry(pValue,NULL,NULL);
        m_ht.put( i, pEntry );
    }
    */
    resize(initsize); 
}


template <class TValue>
CVector<TValue>::CVector(UINT initsize, TValue initvalue)
{
	m_count=0;
	m_size=0;
	m_pValue=new TValue;
	*m_pValue=initvalue; 
//del by LJL 2005-12-29 将resize修改为内联函数，构造函数的内容和resize是一致的 
/*	if(initsize>0)
    {
       m_count=initsize;
       m_size=initsize;
    }
	m_pValue=new TValue;
	_Entry* pEntry;
	TValue* pValue;
	pEntry=NULL;
	pValue=NULL;
	for(int i=0; i<m_size; i++)
    {
        pValue=new TValue;
        *pValue=initvalue;
        pEntry=new _Entry(pValue,NULL,NULL);
        m_ht.put( i, pEntry );
    }
    */
    resize(initsize, initvalue); 
}


template <class TValue>
CVector<TValue>::~CVector()
{
//del by LJL 2005-12-29 将remove修改为内联函数，析构函数的内容和remove是一致的 
/*	_Entry* pEntry;
	pEntry=NULL;

	if(m_pValue!=NULL) delete m_pValue;
	for(int i=0;i<m_size;i++)
	{
       if(m_ht.get( i , pEntry))
	   {
          if(pEntry!=NULL)
		  {
             if(pEntry->m_value!=NULL) delete pEntry->m_value;

             delete pEntry;
             pEntry=NULL;
          }
		  m_ht.remove(i);
	   }
    }
*/
   if(m_pValue != NULL)
   {
      delete m_pValue;
      m_pValue = NULL;
   }
  remove(); 
}


template <class TValue>
inline void CVector<TValue>::resize(UINT newSize)
{
	_Entry* pEntry;
	TValue* pValue;
	pEntry=NULL;
	pValue=NULL;
	
	if(newSize>m_size)
	{
		for(int i=m_size; i<newSize; i++)
		{
            pValue=new TValue;
            *pValue=*m_pValue;
            pEntry=new _Entry(pValue,NULL,NULL);
            m_ht.put( i, pEntry );
		}
		m_size=newSize;
	}

	if(newSize<m_count)	//如果新的表格大小小于原始有效数量，则有效值减小
	{
		m_count=newSize;
	}
}


template <class TValue>
inline void CVector<TValue>::resize(UINT newSize, TValue value)
{
	_Entry* pEntry;
	TValue* pValue;
	pEntry=NULL;
	pValue=NULL;
    if(newSize>m_size)
	{
		for(int i=m_size; i<newSize; i++)
		{
            pValue=new TValue;
            *pValue=value;
            pEntry=new _Entry(pValue,NULL,NULL);
            m_ht.put( i, pEntry );
		}
		m_size=newSize;
		m_count=newSize;	
	}

	if(newSize<m_count)	//如果新的表格大小小于原始有效数量，则有效值减小
	{
		m_count=newSize;
	}
}



template <class TValue>
TValue& CVector<TValue>::operator[](const UINT i)
{
	_Entry* pEntry;
	TValue* pValue;
	pEntry=NULL;
	pValue=NULL;

	if(i<m_size)
	{
		if(m_ht.get(i, pEntry))
        {
           if(pEntry!=NULL)
           {
              if(pEntry->m_value!=NULL)
              {
                 pValue=pEntry->m_value;
                 return *pValue;
              }
           }
        }
	}
	return *m_pValue;
}


template <class TValue>
UINT CVector<TValue>::push_back(const TValue& value)
{
	_Entry* pEntry;
	TValue* pValue;
	pEntry=NULL;
	pValue=NULL;
	
	if(m_count<m_size)	//如果有效数量小于数组大小，则意味着有实际数组空间可用
	{
		//if(!m_ht.get( m_count-1, pEntry )) return 0; Delete By LJL 2006-2-10
		if(!m_ht.get( m_count, pEntry )) return 0;	//应该是写到有效数量之后的那个空间位置中
        if(pEntry==NULL) return 0;
        if(pEntry->m_value==NULL) return 0;

        *(pEntry->m_value)=value;
        m_count++;
	}
	else
	{
		pValue=new TValue;
		if(pValue==NULL) return 0;
		
		*pValue=value;
		pEntry=new _Entry(pValue,NULL,NULL);
		if(pEntry==NULL)
		{
            delete pValue;
            return 0;
        }
		
        m_ht.put( m_count, pEntry );
		m_size++;
		m_count++;
	}
	return m_count;	
}


template <class TValue>
UINT CVector<TValue>::pop_back()
{
	_Entry* pEntry;
	pEntry=NULL;
	
	if(m_count>0 && m_ht.get( m_count-1, pEntry ))
	{
		m_count-=1;
		return m_count;
	}

	return 0;
}


template <class TValue>
UINT CVector<TValue>::pop_back(TValue& value)
{
	_Entry* pEntry;
	pEntry=NULL;
	
	if(m_count>0 && m_ht.get( m_count-1, pEntry ))
	{
		if(pEntry==NULL) return 0;
		if(pEntry->m_value==NULL) return 0;
		
		value=*(pEntry->m_value);
		m_count-=1;
		return m_count;
	}

	return 0;
}


template <class TValue>
inline void CVector<TValue>::remove()
{
	_Entry* pEntry;
	pEntry=NULL;
	
	for(int i=0;i<m_size;i++)
	{
		if(m_ht.get( i , pEntry))
		{
            if(pEntry!=NULL)
            {
			    if(pEntry->m_value!=NULL) delete pEntry->m_value;
			    delete pEntry;
                pEntry=NULL; 
            }
			m_ht.remove(i);
		}
	}
	m_size=0;
	m_count=0;
}


#endif
