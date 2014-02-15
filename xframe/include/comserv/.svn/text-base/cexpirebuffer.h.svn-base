/*********************************************************************
 * Copyright (c)2010, by MT2
 * All rights reserved.
 * FileName：				CExpireBuffer.h
 * System：           		SoftSwitch
 * SubSystem：	        	Common
 * Author：					李静林
 * Description：
		带超时时间的缓冲，可以自动清除超期的数据
 *
 * Last Modified:
		2005-11-16 v1.1 李静林 建立基本文件
 *********************************************************************/

#ifndef _UNI_EXPIRE_BUFFER_H_
#define _UNI_EXPIRE_BUFFER_H_

#include <map>
#include <time.h>

///////////////////////////////////////////////////////////////////////
// 定义 CExpireBuffer< TKey, TValue > 类
///////////////////////////////////////////////////////////////////////
template <class TKey, class TValue>
class CExpireBuffer	// TValue........
{
private:
	//--------------------------
	//.......
	//--------------------------
	class _ExpireBuffer_Value	// ..........
	{
	public:
		TValue	m_value;		// 此节点的内容
		TKey	m_key;			// 此节点的内容
		time_t	m_inittime;		// 添加节点的时间
		time_t	m_refreshtime;	// 刷新节点的时间
		time_t	m_endtime;		// 删除节点的时间
		
		_ExpireBuffer_Value(const TKey &key, const TValue &value, time_t inittime=0)
		{
			m_key=key;
			m_value=value;
			if(inittime)
				m_inittime=inittime;
			else
				m_inittime=time(0);
			m_refreshtime=m_inittime;
			m_endtime=0;
		};

		_ExpireBuffer_Value()
		{
			m_inittime=time(0);
			m_refreshtime=m_inittime;
			m_endtime=0;
		};
		
		~_ExpireBuffer_Value( void )
		{

		};
		
		void setRefreshTime(time_t refreshtime=0)
		{
			if(refreshtime) m_refreshtime=refreshtime;
			else m_refreshtime=time(0);
		};
		
		void setEndTime(time_t endtime=0)
		{
			if(endtime) m_endtime=endtime;
			else m_endtime=time(0);
		};

	};

	//-----------------------------
	// 链表的节点类型
	//-----------------------------
	class _ExpireBuffer_Enty	// 定义链表中的节点类型
	{
	public:
		_ExpireBuffer_Value		m_value;		// 此节点的内容
		_ExpireBuffer_Enty*		m_prev;			// 指向前一个节点
		_ExpireBuffer_Enty*		m_next;			// 指向下一个节点
		// 节点的构造函数，其中会调用键值设置函数来设置这个节点的键
		_ExpireBuffer_Enty( _ExpireBuffer_Value value, _ExpireBuffer_Enty* prev, _ExpireBuffer_Enty* next )
		{
			m_value=value;
			m_prev=prev;
			m_next=next;
		};
		
		_ExpireBuffer_Enty( _ExpireBuffer_Enty* prev, _ExpireBuffer_Enty* next )
		{
			m_prev=prev;
			m_next=next;
		};
		// 节点的析构函数，其中调用键值清除函数来清除此节点的键
		~_ExpireBuffer_Enty( void )
		{

		};
	};
	
	_ExpireBuffer_Enty* m_pHead;
	_ExpireBuffer_Enty* m_pTail;
	_ExpireBuffer_Enty* m_pCurrent;
	_ExpireBuffer_Enty* m_pTag;

	 TValue tmpValue;
	
	//--------------------------
	// 哈希表，用于快速检索
	//--------------------------
	std::map<TKey, _ExpireBuffer_Enty*>  m_map;

	//--------------------------
	// 公共参数
	//--------------------------
	unsigned int m_count;			//记录数量
	unsigned int m_count_max;		//限制的最大记录数量 0：默认没有限制
	bool m_destroy_old;		//销毁记录的策略 默认0：不自动销毁过期记录；1：自动销毁过期记录，当到达最大记录数量的时候，从最老的开始销毁
	unsigned int m_living_time;		//生存时间，单位秒，默认172800秒；当最后一次刷新的时间超过这一时间长度（currenttime-refreshtime > m_living_time），则过期

public:
	CExpireBuffer(unsigned int max_count=0, bool destroy_old=false, unsigned int living_time=172800);
	~CExpireBuffer();

	//表长度. 
	void setMaxCount(unsigned int max_count=0) { m_count_max=max_count; };
	void setLivingTime(unsigned int living_time=172800) { m_living_time=living_time;};
	void setDestroyPolicy(bool destroy_old=false) { m_destroy_old=destroy_old;};
	//缓冲表能够保存的最大条目数量
	int getMaxSize() {return m_count_max;};
	bool getDestoryPolicy() {return m_destroy_old;};
	int getLivingTime() {return m_living_time;};
	//当前数量
	int count() { return m_count; };	
		
	// 清空缓冲表
	void clear( void );

	//遍历缓冲表，首先调用front或back或find，之后使用next或prev
	bool front(TKey& key, TValue& value);
	bool front(TValue& value);
	bool front();
	bool back(TKey& key, TValue& value);
	bool back(TValue& value);
	bool back();
	bool next(TKey& key, TValue& value);
	bool next(TValue& value);
	bool next();
	bool prev(TKey& key, TValue& value);
	bool prev(TValue& value);
	bool prev();
	//根据输入的key获得记录
	bool find(const TKey& key, TValue& value);
	bool find(const TKey& key);
	TValue& operator[](const TKey& key);
	
	//销毁头或尾节点
	bool pop_front(TKey&key, TValue& value);
	bool pop_front(TValue& value);
	bool pop_front();
	bool pop_back(TKey& key, TValue& value);
	bool pop_back(TValue& value);
	bool pop_back();
	//根据输入的TKey删除指定记录
	bool pop(const TKey& key, TValue& value);
	bool pop(const TKey& key);
	//销毁过期节点（如果其到期时间早于指定时间），默认输入为当前时间；用户可自行调用清理超期对象
	bool pop_front_old(TKey& key, TValue& value, const time_t currenttime=0);
	bool pop_front_old(TValue& value, const time_t currenttime=0);
	bool pop_front_old(const time_t currenttime=0);
	int  pop_old(const time_t currenttime=0);	//返回删除的条目数量
	
	//添加新记录，暂不支持设置初始化时间
	// 0: 添加失败
	// 1: 添加成功
	// 2: 添加成功，但替换掉了最旧的节点
	int push(const TKey& key, const TValue& value);
	int push(const TKey& key, const TValue& value, TValue& oldValue);
	
	//更新记录，更新记录的同时刷新了记录的刷新时间，默认输入为当前时间，暂不支持设置刷新时间
	bool update(const TKey& key, time_t refreshtime = 0);
	bool update(const TKey& key, const TValue& value, TValue& oldValue);
	
};


template <class TKey, class TValue>
inline CExpireBuffer<TKey, TValue>::CExpireBuffer(unsigned int max_count, bool destroy_old, unsigned int living_time)
{
	m_count=0;
	m_count_max=max_count;		
	m_destroy_old=destroy_old;
	m_living_time=living_time;	
	_ExpireBuffer_Enty* pEntry=NULL;
	m_pTag=new _ExpireBuffer_Enty( pEntry, pEntry );
	m_pHead=m_pTag;
	m_pTail=m_pTag;
	m_pCurrent=m_pTag;
	m_pTag->m_next=m_pHead;
	m_pTag->m_prev=m_pTail;
}


template <class TKey, class TValue>
inline CExpireBuffer<TKey, TValue>::~CExpireBuffer()
{
	while(m_pHead!=m_pTag)
	{
		m_pCurrent=m_pHead->m_next;
		delete m_pHead;
		m_pHead=m_pCurrent;
	}
	if(m_pTag!=NULL) delete m_pTag;
	m_map.clear();
}

template <class TKey, class TValue>
inline bool CExpireBuffer<TKey, TValue>::front(TKey& key, TValue& value)
{
	if ( m_count>0 )
	{
		m_pCurrent=m_pHead;
		value=m_pHead->m_value.m_value;
		key=m_pHead->m_value.m_key;
		return true;
	}
	return false;
}

template <class TKey, class TValue>
inline bool CExpireBuffer<TKey, TValue>::front(TValue& value)
{
	if ( m_count>0 )
	{
		m_pCurrent=m_pHead;
		value=m_pHead->m_value->m_value;
		return true;
	}
	return false;
}

template <class TKey, class TValue>
inline bool CExpireBuffer<TKey, TValue>::back(TKey& key, TValue& value)
{
	if( m_count>0 )
	{
		m_pCurrent=m_pTail;
		value=m_pTail->m_value.m_value;
		key=m_pTail->m_value.m_key;
		return true;
	}
	return false;
}

template <class TKey, class TValue>
inline bool CExpireBuffer<TKey, TValue>::back(TValue& value)
{
	if( m_count>0 )
	{
		m_pCurrent=m_pTail;
		value=m_pTail->m_value.m_value;
		return true;
	}
	return false;
}

template <class TKey, class TValue>
inline bool CExpireBuffer<TKey, TValue>::next()
{
	if ( m_count>0 )
	{
		if(!m_pCurrent) return false;
		m_pCurrent=m_pCurrent->m_next;
		if(m_pCurrent==m_pTag) return false;
		return true;
	}
	return false;	
}

template <class TKey, class TValue>
inline bool CExpireBuffer<TKey, TValue>::next(TKey& key, TValue& value)
{
	if ( next() )
	{
		value=m_pCurrent->m_value.m_value;
		key=m_pCurrent->m_value.m_key;
		return true;
	}
	return false;	
}

template <class TKey, class TValue>
inline bool CExpireBuffer<TKey, TValue>::next(TValue& value)
{
	if ( next() )
	{
		value=m_pCurrent->m_value.m_value;
		return true;
	}
	return false;	
}

template <class TKey, class TValue>
inline bool CExpireBuffer<TKey, TValue>::prev()
{
	if( m_count>0 )
	{
		if(!m_pCurrent) return false;
		m_pCurrent=m_pCurrent->m_prev;
		if(m_pCurrent==m_pTag) return false;
		return true;
	}
	return false;	
}

template <class TKey, class TValue>
inline bool CExpireBuffer<TKey, TValue>::prev(TKey& key, TValue& value)
{
	if(prev())
	{
		value=m_pCurrent->m_value.m_value;
		key=m_pCurrent->m_value.m_key;
		return true;
	}
	return false;	
}

template <class TKey, class TValue>
inline bool CExpireBuffer<TKey, TValue>::prev(TValue& value)
{
	if(prev())
	{
		value=m_pCurrent->m_value.m_value;
		return true;
	}
	return false;	
}



template <class TKey, class TValue>
inline bool CExpireBuffer<TKey, TValue>::find(const TKey& key, TValue& value)
{
	_ExpireBuffer_Enty* pValue=NULL;
	if (m_count==0) return false;
	
	if(m_map.find(key)!=m_map.end())
	{
		pValue=m_map[key];
		if(!pValue)
		{
			m_map.erase(key);
			return false;
		}
		m_pCurrent=pValue;
		value=pValue->m_value.m_value;
		return true;
	}
	else
		return false;
}

template <class TKey, class TValue>
inline TValue& CExpireBuffer<TKey, TValue>::operator[](const TKey& key)
{
	_ExpireBuffer_Enty* pValue=NULL;
	if (m_count==0) return tmpValue;
	
	if(m_map.find(key)!=m_map.end())
	{
		pValue=m_map[key];
		if(!pValue)
		{
			m_map.erase(key);
			return tmpValue;
		}
		m_pCurrent=pValue;
		
		return pValue->m_value.m_value;
	}
	else
		return tmpValue;
}


template <class TKey, class TValue>
inline bool CExpireBuffer<TKey, TValue>::find(const TKey& key)
{
	_ExpireBuffer_Enty* pValue=NULL;
	if (m_count==0) return false;
	
	if(m_map.find(key)!=m_map.end())
	{
		pValue=m_map[key];
		if(!pValue)
		{
			m_map.erase(key);
			return false;
		}
		m_pCurrent=pValue;
		return true;
	}
	else
		return false;
}

template <class TKey, class TValue>
inline bool CExpireBuffer<TKey, TValue>::pop_front()
{
	if (m_count>0)
	{
		m_pCurrent=m_pHead->m_next;	
		m_pCurrent->m_prev=m_pTag;
		m_pTag->m_next=m_pCurrent;
		
		m_map.erase(m_pHead->m_value.m_key);
		
		delete m_pHead;
		m_pHead=m_pCurrent;
		m_count-=1;
		
		if(m_count==0) m_pTail=m_pTag;
		return true;
	}
	return false;
}

template <class TKey, class TValue>
inline bool CExpireBuffer<TKey, TValue>::pop_front(TValue& value)
{
	if (m_count>0)
	{
		value=m_pHead->m_value.m_value;
		return pop_front();
	}
	return false;
}

template <class TKey, class TValue>
inline bool CExpireBuffer<TKey, TValue>::pop_front(TKey&key, TValue& value)
{
	if (m_count>0)
	{
		value=m_pHead->m_value.m_value;
		key=m_pHead->m_value.m_key;
		return pop_front();
	}
	return false;
}

template <class TKey, class TValue>
inline bool CExpireBuffer<TKey, TValue>::pop_front_old(const time_t currenttime)
{
	time_t oldtime=0;
	oldtime=currenttime;
	if(!oldtime) oldtime=time(0);
	
	if (m_count>0)
	{
		if((long)m_living_time<(oldtime - m_pHead->m_value.m_refreshtime))
		{
			return pop_front();
		}
	}	
	return false;
}

template <class TKey, class TValue>
inline bool CExpireBuffer<TKey, TValue>::pop_front_old(TKey& key, TValue& value, const time_t currenttime)
{
	if (m_count>0)
	{
		value=m_pHead->m_value.m_value;
		key=m_pHead->m_value.m_key;
		return pop_front_old(currenttime);
	}	
	return false;
}

template <class TKey, class TValue>
inline bool CExpireBuffer<TKey, TValue>::pop_front_old(TValue& value, const time_t currenttime)
{
	if (m_count>0)
	{
		value=m_pHead->m_value.m_value;
		return pop_front_old(currenttime);
	}	
	return false;
}

template <class TKey, class TValue>
inline int CExpireBuffer<TKey, TValue>::pop_old(const time_t currenttime)
{
	int count=0;
	if (m_count>0)
	{
		while(pop_front_old(currenttime))
		{
			count++;
		}
		return count;
	}	
	return 0;
}

template <class TKey, class TValue>
inline bool CExpireBuffer<TKey, TValue>::pop_back()
{
	if (m_count>0)
	{
		m_pCurrent=m_pTail->m_prev;
		m_pCurrent->m_next=m_pTag;
		m_pTag->m_prev=m_pCurrent;

		m_map.erase(m_pTail->m_value.m_key);

		delete m_pTail;
		m_pTail=m_pCurrent;
		m_count-=1;
		
		if(m_count==0) m_pHead=m_pTag;
		return true;
	}
	return false;
}


template <class TKey, class TValue>
inline bool CExpireBuffer<TKey, TValue>::pop_back(TKey& key, TValue& value)
{
	if (m_count>0)
	{
		value=m_pTail->m_value.m_value;
		key=m_pTail->m_value.m_key;
		return pop_back();
	}
	return false;
}

template <class TKey, class TValue>
inline bool CExpireBuffer<TKey, TValue>::pop_back(TValue& value)
{
	if (m_count>0)
	{
		value=m_pTail->m_value.m_value;
		return pop_back();
	}
	return false;
}

template <class TKey, class TValue>
inline bool CExpireBuffer<TKey, TValue>::pop(const TKey& key, TValue& value)
{
	_ExpireBuffer_Enty* pPrev;
	_ExpireBuffer_Enty* pNext;
	_ExpireBuffer_Enty* pValue=NULL;
	
	if (m_count==0) return false;
	
	if(m_map.find(key)!=m_map.end())
	{
		pValue=m_map[key];
		if(!pValue)
		{
			m_map.erase(key);
			return false;
		}

		value=pValue->m_value.m_value;
		pPrev=pValue->m_prev;
		pNext=pValue->m_next;
		m_count-=1;
		delete pValue;
		pValue=NULL;

		if(pNext==m_pTag)	//........
		{
			m_pTag->m_prev=pPrev;
			m_pTail=pPrev;
			m_pCurrent=pPrev;
			if(m_count==0) m_pHead=m_pTag;
		}
		if (pPrev==m_pTag)	//........
		{
			m_pTag->m_next=pNext;
			m_pHead=pNext;
			m_pCurrent=pNext;
			if(m_count==0) m_pTail=m_pTag;
		}
		//......
		pPrev->m_next=pNext;
		pNext->m_prev=pPrev;
		
		m_map.erase(key);
		
		return true;
	}
	else
		return false;
}

template <class TKey, class TValue>
inline bool CExpireBuffer<TKey, TValue>::pop(const TKey& key)
{
	TValue value;
	return pop(key, value);
}

template <class TKey, class TValue>
inline int CExpireBuffer<TKey, TValue>::push(const TKey& key, const TValue& value)
{
	if(m_map.find(key)!=m_map.end())
		return 0;
	
	if(!m_destroy_old && m_count_max>0 && m_count>=m_count_max) 
		return 0;
		
	_ExpireBuffer_Value theValue(key, value);
	m_pCurrent=NULL;
	m_pCurrent=new _ExpireBuffer_Enty(theValue, m_pTail, m_pTag);
	if(!m_pCurrent) return 0;

	m_map[key]=m_pCurrent;
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
	
	if(m_count_max>0 && m_count>m_count_max)
	{
		pop_front();
		return 2;
	}
	return 1;
}

template <class TKey, class TValue>
inline int CExpireBuffer<TKey, TValue>::push(const TKey& key, const TValue& value, TValue& oldValue)
{
	TValue tmpValue;
	if(m_count_max>0 && m_count>=m_count_max)
	{
		tmpValue=m_pHead->m_value.m_value;
	}
	int ret=0;
	ret=push(key, value);
	if(ret==2)
	{
		oldValue=tmpValue;	
	}
	return ret;
}

template <class TKey, class TValue>
inline bool CExpireBuffer<TKey, TValue>::update(const TKey& key, const TValue& value, TValue& oldValue)
{
	if(m_map.find(key)==m_map.end())
		return false;
	m_pCurrent=m_map[key];
	oldValue=m_pCurrent->m_value.m_value;
	m_pCurrent->m_value.m_value=value;
	m_pCurrent->m_value.m_refreshtime=time(0);

	if(m_pCurrent==m_pTail) return true; //已经是队尾，不用更新时间
	
	//不是队尾，且>1
	_ExpireBuffer_Enty* pPrev;
	_ExpireBuffer_Enty* pNext;
	//从表中取出当前指针
	pPrev=m_pCurrent->m_prev;
	pNext=m_pCurrent->m_next;
	pPrev->m_next=pNext;
	pNext->m_prev=pPrev;
	if(m_pCurrent==m_pHead) m_pHead=pNext;	//本来是队头，则向后移动一位
	//当前指针添加到队尾
	m_pTail->m_next=m_pCurrent;
	m_pTag->m_prev=m_pCurrent;
	m_pCurrent->m_next=m_pTag;
	m_pCurrent->m_prev=m_pTail;
	m_pTail=m_pCurrent;

	return true;
}

template <class TKey, class TValue>
inline bool CExpireBuffer<TKey, TValue>::update(const TKey& key, time_t refreshtime)
{
	if(m_map.find(key)==m_map.end())
		return false;
	m_pCurrent=m_map[key];
	if (refreshtime == 0)
		m_pCurrent->m_value.m_refreshtime=time(0);
	else
		m_pCurrent->m_value.m_refreshtime=refreshtime;
	
	if(m_pCurrent==m_pTail) return true; //已经是队尾，不用更新时间

	//不是队尾，且>1
	_ExpireBuffer_Enty* pPrev;
	_ExpireBuffer_Enty* pNext;
	//从表中取出当前指针
	pPrev=m_pCurrent->m_prev;
	pNext=m_pCurrent->m_next;
	pPrev->m_next=pNext;
	pNext->m_prev=pPrev;
	if(m_pCurrent==m_pHead) m_pHead=pNext;	//本来是队头，则向后移动一位
	//当前指针添加到队尾
	m_pTail->m_next=m_pCurrent;
	m_pTag->m_prev=m_pCurrent;
	m_pCurrent->m_next=m_pTag;
	m_pCurrent->m_prev=m_pTail;
	m_pTail=m_pCurrent;
	
	return true;
}

template <class TKey, class TValue>
inline void CExpireBuffer<TKey, TValue>::clear()
{
	if(m_count==0) return;
	m_map.clear();
	m_pCurrent=m_pHead;
	while(m_count) 
	{
		m_pCurrent=m_pHead->m_next;	
		m_pCurrent->m_prev=m_pTag;
		m_pTag->m_next=m_pCurrent;
		delete m_pHead;		
		m_pHead=m_pCurrent;
		m_count--;
	}	 
}

#endif




