/*********************************************************************
 * Copyright (c)2005, by MT2
 * All rights reserved.
 * FileName：            clist.h
 * System：                 SoftSwitch
 * SubSystem：              Common
 * Author：               李静林
 * Description：
      双向链表，可以用于做消息缓冲等

      需要注意的是，getNext/getPrev 方法，必须在getHead或getTail之后，
      这样才好准确处理，否则返回的是当前节点的 getHead或getTail ，除非
      使用者记得当前应该是哪一个节点，否则就不知道定位到哪里去了。

      另外，作为链表内容的class TValue，在使用中是直接进行赋值操作的，因此
      如果class TValue比较复杂，请自行解决重载=的问题，最好不发生此问题。

      如果class TValue中保存了指针等内容，在释放list之前，请自行释放class TValue
      中保存的内存指针，list只能将自己的所有内存释放，解决不了节点内的问题
      
      例如：进行链表中的搜索匹配操作
      list.reset();      //首先要重置指针，将当前指针指向链表的起始。
      while(list.contain(value))
      {
         //匹配到了符合检索条件的值，可以继续执行相应操作
         
         //如果执行了pop_current()，删除了当前节点，则可以继续执行
         //因为删除当前节点之后，当前节点指针自动指向了被删除节点的下一个节点（如果删除的是尾节点，则指向上一个节点）
         
         //如果没有删除当前节点，仅仅是获得了当前节点的值，则应该执行以下操作
         list.next();      
         
      }
      
         

 *
 * Last Modified:
      2005-11-16 v1.1 李静林 建立基本文件
      2005-11-17 v1.2 李静林 提供removeCurrent方法，用于释放当前的节点
      2005-11-19 v1.3 李静林 提供remove方法，用于释放指定的节点
                        提供contain和containNext用于查找指定的节点
                        提供Insert方法，用于在任意位置插入节点
                        
      2005-11-20 v1.4 李静林 按照模板类的风格修订了接口函数，注意有些参数的使用是不一致的。
      2005-12-05 v1.5 李静林 由于有些模块使用了模板类的const_iterator概念，特添加上这些接口。
      2005-12-05 v1.9 李静林 添加iterator。
      2005-12-06 v1.10 李静林 将List修改成为双向循环链表，其中迭代器支持循环操作，普通的访问不支持循环
                        contain_next不再被支持
                        
      2005-12-09 v1.11 李静林 将reset方法的内容恢复为：将当前指针指向头指针。同时，contain方法
                                          将从当前节点开始从头至尾进行匹配，并返回的是第一个包含指定内容的节点。
                                          搜索操作的例子见以上说明。
                                          添加了front(),back(),next(),prev()方法，用于操作当前指针
                                          添加了current(TValue& value)用于获得当前指针的值
      2006-3-23 李静林 修改了文件头的宏定义，因为原来的宏定义 _HASHTABLE_H 与
                                 resiprocate的宏意义重叠，导致编译错误
 *********************************************************************/

#ifndef _UNILIST_H_
#define _UNILIST_H_

#include "comtypedef.h"

typedef void* CListNodeHandle;

///////////////////////////////////////////////////////////////////////
// 定义 CList< TValue > 类
///////////////////////////////////////////////////////////////////////
template <class TValue>
class CList   // TValue为链表要存储的值
{
private:
   
   class _Entry   // 定义链表中的节点类型
   {
   public:
      TValue      m_value;      // 此节点的内容
      _Entry*      m_prev;         // 指向前一个节点
      _Entry*      m_next;         // 指向下一个节点
      // 节点的构造函数，其中会调用键值设置函数来设置这个节点的键
      _Entry(TValue& value, _Entry* prev, _Entry* next )
      {
         m_value=value;
         m_prev=prev;
         m_next=next;
      }
      
      _Entry(_Entry* prev, _Entry* next )
      {
         m_prev=prev;
         m_next=next;
      }
      // 节点的析构函数，其中调用键值清除函数来清除此节点的键
      ~_Entry( void )
      {

      }
   };
   
   _Entry* m_pHead;
   _Entry* m_pTail;
   _Entry* m_pCurrent;
   
   _Entry* m_pTag;
   
   INT m_count;
   BOOL _addr(int pos);

public:
   CList();
   ~CList();

   class iterator
   {
   protected:
      _Entry* _Ptr;
   public:
         
      iterator() {}
      iterator(_Entry* _P){ _Ptr=_P;}   //有初始化的构造，初始化_Entry

      TValue& operator*() const
      {
         return (_Ptr->m_value); 
      }
      iterator& operator++()
      {
         if(_Ptr!=NULL) _Ptr = _Ptr->m_next;   //指向下一个节点地址
         return (*this);                //返回本iterator
      }
      
      iterator operator++(int)
      {
         iterator _Tmp = *this;
         ++*this;
         return (_Tmp); 
      }
      
      iterator& operator--()
      {
         if(_Ptr!=NULL) _Ptr = _Ptr->m_prev;   //指向前一个节点地址
         return (*this);                //返回本iterator
      }

      iterator operator--(int)
      {
         iterator _Tmp = *this;
         --*this;
         return (_Tmp); 
      }
      
      BOOL operator==(const iterator& _X) const
      {
         return (_Ptr == _X._Ptr); 
      }
      
      BOOL operator!=(const iterator& _X) const
      {
         return (!(*this == _X)); 
      }
      int pointTo(_Entry* p)
      {
         return _Ptr == p;
      }
   };
         

   iterator begin()                     //构造一个指向头节点的iterator
      {return (iterator(m_pHead)); }
   iterator end()                        //构造一个指向尾节点的iterator
      {return (iterator(m_pTail->m_next)); }
   iterator pos(int index)
   {
      _Entry* p = m_pHead;
      while(index > 0 && p!= m_pTail->m_next)
      {
         p = p->m_next;
         index --;
      }
      return (iterator(p)); 
   }
   BOOL front(TValue& value);   //获得头节点，获得后当前节点为头节点 FALSE：没有头节点，空
   TValue& front_r();   //获得头节点的引用
   BOOL back(TValue& value);      //获得尾节点，获得后当前节点为尾节点FALSE：没有尾节点，空
   TValue& back_r();      //获得尾节点的引用
   BOOL next(TValue& value);   //获得下一个节点，FALSE没有下一个节点了. 
   BOOL prev(TValue& value);   //获得下一个节点，FALSE没有上一个节点了.
   BOOL current(TValue& value);   //获得当前节点，FALSE当前节点没有值.
   BOOL get(int pos, TValue& value);
   BOOL set(int pos, TValue& value);
    
   void reset();      //当前指针指向链表的第一个节点，一般在调用contain之前使用。
   BOOL front();      //当前指针指向头节点，FALSE：没有头节点，空
   BOOL back();      //当前指针指向尾节点，FALSE：没有尾节点，空
   BOOL next();      //当前指针指向下一个节点，FALSE没有下一个节点了. 
   BOOL prev();      //当前指针指向上一个节点，FALSE没有上一个节点了. 

   UINT push_front(TValue value);   //添加头节点，返回表长度，0，添加失败，添加后当前节点为头节点 
   UINT push_front_r(TValue& value);
   UINT push_back(TValue value); //添加尾节点，返回表长度，0，添加失败，添加后当前节点为尾节点 
   UINT push_back_r(TValue& value);
   UINT insert(TValue value);      //在当前位置之后插入节点，返回表长度，0失败，当前节点为插入后的节点
   UINT insert_r(TValue& value);

   // insert sortly
   // mode = 0, from min to max
   // mode = 1, from max to min
   UINT insertSort(TValue& value, int mode=0);

   void print();

   BOOL contain(TValue& value);    //从当前节点开始查找相同节点，TRUE，找到相同的节点；FALSE，没找到相同的节点，当前节点为匹配的节点
   int index(TValue& value); // find a node, return the index, -1 for not found
   int index(iterator&);

   UINT pop_front();   //删除头节点，返回表长度，0，删除失败；删除后当前节点为头节点
   UINT pop_back();   //删除尾节点，返回表长度，0，删除失败；删除后当前节点为尾节点
   UINT pop_current();   //删除当前节点，返回表长度，0，删除失败；删除之后当前节点转移到下一个（缺省是从头到尾的下一个节点）
   UINT pop_front(TValue& value);   //删除头节点，返回删除的内容及表长度,0 is empty). 
   UINT pop_back(TValue& value);   //删除尾节点，返回删除的内容及表长度,0 is empty).  
   UINT pop_current(TValue& value);   //删除当前节点，返回被删除的节点，当前节点转移到下一个（缺省是从头到尾的下一个节点） add by 李静林 2005-11-18
   UINT remove(TValue& value); //删除所有匹配的节点，返回删除的数量，0，删除失败（没有匹配项）；

   CListNodeHandle getCurrentHandle();     
   void removeByHandle(CListNodeHandle handle);

   void remove();   //删除所有节点 
   
   BOOL empty();   //链表是否为空 
   BOOL ishead();   //当前位置是链表的头
   BOOL istail();   //当前位置是链表的尾
   INT count();   //链表长度. 

   void clear();
   INT  size();
   UINT put(TValue& value);
   void operator=(CList<TValue>& v);
 
};

template <class TValue>
inline CList<TValue>::CList()
{
   m_count=0;
   m_pTag=new _Entry( NULL, NULL );
   m_pHead=m_pTag;
   m_pTail=m_pTag;
   m_pCurrent=m_pTag;
   m_pTag->m_next=m_pHead;
   m_pTag->m_prev=m_pTail;
}


template <class TValue>
inline CList<TValue>::~CList()
{
   while(m_pHead!=m_pTag)
   {
      m_pCurrent=m_pHead->m_next;
      delete m_pHead;
      m_pHead=m_pCurrent;
   }
   if(m_pTag!=NULL) delete m_pTag;
}


template <class TValue >
inline BOOL CList<TValue>::front()
{
   if ( m_count>0 && m_pHead!=m_pTag )
   {
      m_pCurrent=m_pHead;
      return TRUE;
   }
   return FALSE;
}


template <class TValue >
inline BOOL CList<TValue>::front(TValue& value)
{
   if ( front() )
   {
      value=m_pHead->m_value;
      return TRUE;
   }
   return FALSE;
}

template <class TValue >
inline TValue& CList<TValue>::front_r()
{
   return m_pHead->m_value;
}

template <class TValue >
inline BOOL CList<TValue>::back()
{
   if( m_count>0 && m_pTail!=m_pTag )
   {
      m_pCurrent=m_pTail;
      return TRUE;
   }
   return FALSE;
}

template <class TValue >
inline TValue& CList<TValue>::back_r()
{
   return m_pTail->m_value;
}

template <class TValue >
inline BOOL CList<TValue>::back(TValue& value)
{
   if( back() )
   {
      value=m_pTail->m_value;
      return TRUE;
   }
   return FALSE;
}

template <class TValue >
UINT CList<TValue>::pop_front()
{
   if (m_count>0)
   {
      m_pCurrent=m_pHead->m_next;   
      m_pCurrent->m_prev=m_pTag;
      m_pTag->m_next=m_pCurrent;
      delete m_pHead;
      m_pHead=m_pCurrent;
      m_count-=1;
      
      if(m_count==0) m_pTail=m_pTag;
   }
   return m_count;
}


template <class TValue >
UINT CList<TValue>::pop_back()
{
   if (m_count>0)
   {
      m_pCurrent=m_pTail->m_prev;
      m_pCurrent->m_next=m_pTag;
      m_pTag->m_prev=m_pCurrent;
      delete m_pTail;
      m_pTail=m_pCurrent;
      m_count-=1;
      
      if(m_count==0) m_pHead=m_pTag;
   }
   return m_count;
}


template <class TValue >
UINT CList<TValue>::pop_current()
{
   _Entry* pPrev;
   _Entry* pNext;
   
   if (m_count==0 || m_pCurrent==m_pTag)
   {
      return 0;
   }

   pPrev=m_pCurrent->m_prev;
   pNext=m_pCurrent->m_next;

   if(pNext==m_pTag)   //当前节点是尾节点
   {
      return pop_back();
   }

   if (pPrev==m_pTag)   //当前节点是头节点
   {
      return pop_front();
   }

   //是中间的节点
   delete m_pCurrent;
   m_pCurrent=pNext;
   m_pCurrent->m_prev=pPrev;
   pPrev->m_next=m_pCurrent;
   m_count-=1;
   
   return m_count;
}


template <class TValue >
UINT CList<TValue>::pop_front(TValue& value)
{
   if (m_count>0)
   {
      value=m_pHead->m_value;
   }
   return pop_front();
}
   
template <class TValue >
UINT CList<TValue>::pop_back(TValue& value)
{
   if (m_count>0)
   {
      value=m_pTail->m_value;
   }
   return pop_back();
}

template <class TValue >
UINT CList<TValue>::pop_current(TValue& value)
{
   if (m_count>0 && m_pCurrent!=m_pTag)
   {
      value=m_pCurrent->m_value;
   }
   return pop_current();
}

template <class TValue >
UINT CList<TValue>::push_front_r(TValue& value)
{
   m_pCurrent=new _Entry(value, m_pTag, m_pHead);
   if (0==m_count)
   {
      m_pHead=m_pCurrent;
      m_pTail=m_pCurrent;
      m_pTag->m_prev=m_pTail;
   }
   else
   {
      m_pHead->m_prev=m_pCurrent;
      m_pHead=m_pCurrent;
   }
   m_pTag->m_next=m_pHead;
   m_count+=1;
   return m_count;
}

template <class TValue >
UINT CList<TValue>::push_front(TValue value)
{
   return push_front_r(value);
}

template <class TValue >
UINT CList<TValue>::push_back_r(TValue& value)
{
   m_pCurrent=new _Entry(value, m_pTail, m_pTag);
   if (0==m_count)
   {
      m_pHead=m_pCurrent;
      m_pTail=m_pCurrent;
      m_pTag->m_next=m_pHead;
   }
   else
   {
      m_pTail->m_next=m_pCurrent;
      m_pTail=m_pCurrent;
   }
   m_pTag->m_prev=m_pTail;
   m_count+=1;
   return m_count;
}

template <class TValue >
UINT CList<TValue>::push_back(TValue value)
{
   return push_back_r(value);
}

template <class TValue >
void CList<TValue>::remove()
{
   do 
   {
   } while(pop_front());    
}

template <class TValue >
inline BOOL CList<TValue>::next()
{
   if(m_pCurrent==m_pTag || istail()) return FALSE;
   
   m_pCurrent=m_pCurrent->m_next;
   return TRUE;
}

template <class TValue >
BOOL CList<TValue>::next(TValue& value)
{
   if(next())
   {
      value=m_pCurrent->m_value;
      return TRUE;
   }
   return FALSE;
}

template <class TValue >
inline BOOL CList<TValue>::prev()
{
   if(m_pCurrent==m_pTag || ishead()) return FALSE;
   
   m_pCurrent=m_pCurrent->m_prev;
   return TRUE;
}
   
   
template <class TValue >
BOOL CList<TValue>::prev(TValue& value)
{
   if(prev())
   {
      value=m_pCurrent->m_value;
      return TRUE;
   }
  return FALSE;
}


template <class TValue >
inline BOOL CList<TValue>::current(TValue& value)
{
   if(m_pCurrent==m_pTag || m_pCurrent==NULL) return FALSE;
   value=m_pCurrent->m_value;
   return TRUE;
}

template <class TValue >
inline BOOL CList<TValue>::_addr(int pos)
{
   if(m_count <= 0)
      return FALSE;
   if(pos < 0 || pos >= m_count)
      return FALSE;

   _Entry* p = m_pHead;
   while(pos>0 && p!=m_pTag && p!=NULL)
   {
      p = p->m_next;
      pos--;
   }
   m_pCurrent = p;
   return TRUE;
}
   
template <class TValue >
inline BOOL CList<TValue>::get(int pos, TValue& value)
{
   if(!_addr(pos))
      return FALSE;
   value = m_pCurrent->m_value;
   return TRUE;
}

template <class TValue >
inline BOOL CList<TValue>::set(int pos, TValue& value)
{
   if(!_addr(pos))
      return FALSE;
   m_pCurrent->m_value = value;
   return TRUE;
}

template <class TValue >
BOOL CList<TValue>::contain(TValue& value)
{
   while(m_pCurrent!=m_pTag && m_pCurrent!=NULL)
   {
      if(m_pCurrent->m_value == value) return TRUE;
      m_pCurrent=m_pCurrent->m_next;
   }
   return FALSE;
}

template <class TValue >
int CList<TValue>::index(TValue& value)
{
   int i=0;
   _Entry* p = m_pHead;
   while(p!=m_pTag && p!=NULL)
   {
      if(p->m_value == value)
         return i;
      p=p->m_next;
      i++;
   }
   return -1;
}

template <class TValue >
int CList<TValue>::index(iterator& j)
{
   int i=0;
   _Entry* p = m_pHead;
   while(p!=m_pTag && p!=NULL)
   {
      if(j.pointTo(p))
         return i;
      p=p->m_next;
      i++;
   }
   return -1;
}


template <class TValue >
inline void CList<TValue>::reset()
{
   m_pCurrent=m_pHead;
}

template <class TValue >
UINT CList<TValue>::remove(TValue& value)
{
   int i=0;
   reset();
   while(contain(value))
   {
      pop_current();
      reset();
      i++;
   }
   return i;
}

template <class TValue >
CListNodeHandle CList<TValue>::getCurrentHandle()     
{
   return m_pCurrent;
}

template <class TValue >
void CList<TValue>::removeByHandle(CListNodeHandle handle)
{
   if(handle == NULL)
      return;
   m_pCurrent = (_Entry*)handle;
   pop_current();
   reset();
}

template <class TValue >
UINT CList<TValue>::insert_r(TValue& value)
{
   _Entry* pPrev;
   _Entry* pNext;
   
   if(0==m_count || ishead())
   {
      return push_front_r(value);
   }
   
   if(m_pCurrent==m_pTag) return 0;

   if(istail())
   {
      return push_back_r(value);
   }
   
   pPrev=m_pCurrent;
   pNext=m_pCurrent->m_next;
   m_pCurrent=new _Entry(value,pPrev,pNext);
   pPrev->m_next=m_pCurrent;
   pNext->m_prev=m_pCurrent;
   m_count+=1;
   return m_count;      
}
template <class TValue >
UINT CList<TValue>::insert(TValue value)
{
   return insert_r(value);
}

template <class TValue >
void CList<TValue>::print()
{
   _Entry* p = m_pHead;
   while(p != m_pTag)
   {
      printf("%p <- %p -> %p\n"
         , p->m_prev
         , p
         , p->m_next
      );
      p = p->m_next;
   }
}

template <class TValue >
UINT CList<TValue>::insertSort(TValue& value, int mode)
{
   reset();
   while(m_pCurrent != m_pTag)
   {
      if(mode == 0 ? (value >= m_pCurrent->m_value) : (value <= m_pCurrent->m_value))
         m_pCurrent = m_pCurrent->m_next;
      else
         break;
   }
   if(m_pCurrent == m_pHead)
      return push_front_r(value);
   if(m_pCurrent == m_pTag)
      return push_back_r(value);

   _Entry* pPrev=m_pCurrent->m_prev;
   _Entry* pNext=m_pCurrent;
   m_pCurrent=new _Entry(value,pPrev,pNext);
   pPrev->m_next=m_pCurrent;
   pNext->m_prev=m_pCurrent;
   m_count+=1;
   return m_count;      
}

template <class TValue >
inline BOOL CList<TValue>::empty()
{
   if (m_count>0) return FALSE;
   return TRUE;
}

template <class TValue >
inline INT CList<TValue>::count()
{
   return m_count;
}


template <class TValue >
inline BOOL CList<TValue>::ishead()
{
   if(m_pCurrent==m_pTag) return FALSE;
   if(m_pCurrent->m_prev == m_pTag) return TRUE;
   return FALSE;
}


template <class TValue >
inline BOOL CList<TValue>::istail()
{
   if(m_pCurrent==m_pTag) return FALSE;
   if(m_pCurrent->m_next == m_pTag) return TRUE;
   return FALSE;
}

template < class TValue >
inline void CList<TValue>::operator=(CList<TValue>& v)
{
        if(&v != this )
        {
                remove();
		_Entry* p = v.m_pHead;
		while(p != v.m_pTag)
		{
			push_back_r(p->m_value);
			p = p->m_next;
		}
	}
}

template < class TValue >
inline void  CList<TValue>::clear() 
{ 
	remove();
}

template < class TValue >
inline INT  CList<TValue>::size()  
{ 
	return count();
};

template < class TValue >
inline UINT CList<TValue>::put(TValue& value) 
{ 
	return push_back_r(value); 
};

#endif



