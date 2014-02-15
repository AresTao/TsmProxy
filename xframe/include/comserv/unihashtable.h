/*********************************************************************
 * Copyright (c)2002, by Beijing TeleStar Network Technology Company Ltd.(MT2)
 * All rights reserved.

 * FileName：				hashtable.h
 * System：           		SoftSwitch
 * SubSystem：	        	Common
 * Author：					Ye Li
 * Description：
		哈西表
 *
 * Last Modified:
		2002-5-12
			完成初始版本	By Ye Li
		2002-5-16
			完善对CHAR*作为键类型的支持	By Ye Li
		2002-5-27
			修正从hash值映射哈西表索引的算法。
			（感谢林松涛的意见） By Ye Li
		2002-11-25
			增加了遍历功能，用两个函数实现：reset()、getNext()。
			使用方法：
				1.调用reset()开始；
				2.反复调用getNext()，逐个获取所有的值；
				3.直到getNext()返回FALSE，表示没有后续的值了。

			注意：
				1.在遍历期间不能往表中添加项，否则遍历过程自动中断，需
				重新开始。
				2.遍历的顺序按照内部实现顺序，与用户操作（如添加顺序）
				无关。
				3.遍历操作不改变哈西表中的任何项。  
        2005-11-11
			按照新的命名规则，重命名所有方法（首字母小写）
			

	2006-3-23 李静林 修改了文件头的宏定义，因为原来的宏定义 _HASHTABLE_H 与
			resiprocate的宏意义重叠，导致编译错误 

 *********************************************************************/

#ifndef _UNIHASHTABLE_H_
#define _UNIHASHTABLE_H_

#include <memory.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "comtypedef.h"


///////////////////////////////////////////////////////////////////////
// 定义常量值
///////////////////////////////////////////////////////////////////////

#define DEFAULT_HASHTABLE_LENGTH	16		// 哈西表缺省长度
#define DEFAULT_HASHTABLE_LOADFACTOR	0.75f	// 缺省的装填因子

#define HASHTABLE_MIN_LENGTH		8		// 哈西表最小长度
#define	HASHTABLE_MAX_LENGTH		131072	// 哈西表最大长度，
											// 2的17次方，已经大于10万

#define HASHTABLE_MIN_LOADFACTOR	0.4f	// 装填因子的最小值
#define HASHTABLE_MAX_LOADFACTOR	1.5f	// 装填因子的最大值

//  modified by shuangkai 2004-3-4 
// moved to "comtypedef.h"
//typedef UINT HASH_CODE;		// 定义hash值的类型
//typedef ULONG HASH_CODE;		// 定义hash值的类型
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// 定义 CHashTable< TKey, TValue > 类
///////////////////////////////////////////////////////////////////////

template < class TKey, class TValue >
class CHashTable	// 以TKey为键类型，TValue为值类型的哈西表
{
private:
	class _Entry	// 定义哈西表中的节点类型
	{
	public:
		HASH_CODE	m_hash;			// 此节点的哈西值
		TKey		m_key;			// 此节点的键值
		TValue		m_value;		// 此节点的内容
		_Entry*		m_next;			// 指向下一个节点

		// the following two pointers are for record the put sequence
		_Entry*		m_s_prev;
		_Entry*		m_s_next;

		// 定义设置节点键值的函数类型
		typedef void ( *PF_SETKEY )( TKey& dest, TKey src );

		// 定义清除节点键值的函数类型
		typedef void ( *PF_REMOVEKEY )( TKey& key );

		// 静态的函数指针，指向设置节点键值的函数
		static PF_SETKEY	m_spfSetKey;

		// 静态的函数指针，指向清除节点键值的函数
		static PF_REMOVEKEY	m_spfRemoveKey;

	public:
		// 节点的构造函数，其中会调用键值设置函数来设置这个节点的键
		_Entry( HASH_CODE hash, TKey& key, TValue& value, _Entry* next ) :
		  m_hash( hash ), m_value( value ), m_next( next )
		{
			( *m_spfSetKey )( m_key, key );
		}

		// 节点的析构函数，其中调用键值清除函数来清除此节点的键
		~_Entry( void )
		{
			( *m_spfRemoveKey )( m_key );
		}

	};	// 节点类型定义结束
	typedef _Entry* _PEntry;

private:
	_PEntry*	m_table;			// 定义哈西表
	UINT		m_entryCount;		// 记录目前表中的节点总数
	UINT		m_tableLength;		// 目前的哈西表长度
	UINT		m_modCount;			// 哈西表被更新（修改）的次数
	DOUBLE		m_loadFactor;		// 哈西表装填因子
	UINT		m_threshold;		// 哈西表再哈西算法的触发门限值

	_PEntry		m_pcurEntry;		// 用于记录遍历时的当前节点
	UINT		m_uicurIndex;		// 遍历时，记载当前节点所在的表的索引值

	// 定义用于比较两个键的函数的类型
	typedef BOOL ( *PF_CMP )( TKey& keyA, TKey& keyB );

	// 定义用于计算哈西值的函数的类型
	typedef HASH_CODE ( *PF_HASH )( TKey& key );

	PF_CMP		m_pfCompareKey;		// 函数指针，指向键值比较函数
	PF_HASH		m_pfGetHash;		// 函数指针，指向哈西值计算函数

	// 为了保证二进制兼容（即升版uniframe的时候无需重新编译老的.so）
	// 所以没有定义三个成员，而是使用m_table追加在后面的三个多余位置
	_PEntry seqHead;
	_PEntry seqTail;
	_PEntry seqCur;

public:
	/*	分别用于不同目的的五个构造函数，其中：

		length			用于指定哈西表的初始长度，这个参数受限于
						已定义的哈西表最小最大长度值。并且在使用中，
						由于再哈西算法，哈西表的长度可能会增长。

		loadFactor		指定装填因子，这个参数受限于已定义的
						哈西表最小最大装填因子。

		pfCompareKey	指定键值比较函数。

		pfGetHash		指定哈西值计算函数。

		对于没有指定的参数，将会使用已定义的缺省值。*/
	CHashTable();
	CHashTable( UINT length );
	CHashTable( UINT length, DOUBLE loadFactor );
	CHashTable( PF_HASH pfGetHash );
	CHashTable( PF_CMP pfCompareKey, PF_HASH pfGetHash );
	CHashTable( UINT length, DOUBLE loadFactor,
				PF_CMP pfCompareKey, PF_HASH pfGetHash );

	// 哈西表的析构函数，主要是释放内存空间
	~CHashTable();

public:
	// 清空哈西表
	void	clear( void );

	// 检索哈西表中是否已包含这个键
	BOOL	containsKey( TKey key );

	// 取出从哈西表中的内容（不会清除这个表项）
	BOOL	get( TKey key, TValue& value );
	BOOL	get_r( TKey& key, TValue& value );

	// 加入一项内容，可能只是更新原来的内容
	TValue	put( TKey key, TValue value );
	TValue	put_r( TKey& key, TValue& value );
	void	put_v( TKey& key, TValue& value );

	// 从哈西表中清除一项内容
	BOOL	remove( TKey key );

	// 获取目前哈西表中的表项数目
	UINT	size( void );

	// 判断哈西表是否为空
	BOOL	isEmpty( void );

	// 重置遍历指针，一次遍历前必须进行的动作
	BOOL	reset( void );
	void	resetSeq( void ); // for sequencial

	// 获取下一个项。反复调用这个方法来实现遍历。
	BOOL	getNext( TValue & value );
		// 获取下一个项。不但反馈取值，也反馈Key值。//Added by LJL 2008.08.5
	BOOL	getNext( TKey &key, TValue & value );
	BOOL	getNextSeq( TKey &key, TValue & value ); // for sequencial

private:
	// 由构造函数调用的初始化函数，用于统一处理各种构造函数
	void	initTable( UINT length, DOUBLE loadFactor,
						PF_CMP pfCompareKey, PF_HASH pfGetHash );

	// 再哈西函数
	void	rehash( void );

	// 通过哈西值映射哈西表索引号
	#define GET_HASH_INDEX(hash) ((hash) % m_tableLength)

	// 搜索下一个遍历节点
	BOOL	searchNextEntry( void );

	void addSequence(_Entry*);
	void delSequence(_Entry*);
	void printSequence();
};	// 哈西表定义结束

///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// 定义缺省的函数：键值设置函数、键值清除函数、键值比较函数、哈西函数
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// Function		: _SetKey< TKey >
//
// Description	: 缺省的键值设置函数（赋给哈西表节点中的静态函数指针），
//				  用于除字符串外的其他所有键类型。
//
// Args			:
//				  1. dest	TKey&	被设置的键
//				  2. src	TKey	键值的源
//
// Return values: None
///////////////////////////////////////////////////////////////////////
template < class TKey >
void _SetKey( TKey& dest, TKey src )
{
	dest = src;
}

///////////////////////////////////////////////////////////////////////
// Function		: _SetKey< CHAR* >
//
// Description	: 用于字符串的键值设置函数。在内部分配内存空间，
//				  复制整个字符串。
//
// Args			:
//				  1. dest	CHAR*&	被设置的键
//				  2. src	CHAR*	键值的源
//
// Return values: None
///////////////////////////////////////////////////////////////////////

/*template <>
void _SetKey( CHAR*& dest, CHAR* src )
{
	int len = strlen( src );
	if ( dest!= NULL ) delete[] dest;
	dest = new CHAR[len + 1];
	strcpy( dest, src );
}
*/
void _SetKey( CHAR*& dest, CHAR* src );

///////////////////////////////////////////////////////////////////////
// Function		: _RemoveKey< TKey >
//
// Description	: 缺省的键值清除函数（赋给哈西表节点中的静态函数指针），
//				  用于除字符串外的其他所有键类型。
//
// Args			:
//				  1. key	TKey&	被清除的键
//
// Return values: None
///////////////////////////////////////////////////////////////////////
template < class TKey >
void _RemoveKey( TKey& key )
{
	return;
}

///////////////////////////////////////////////////////////////////////
// Function		: _RemoveKey< CHAR* >
//
// Description	: 用于字符串的键值清除函数。释放先前分配的内存空间。
//
// Args			:
//				  1. key	CHAR*&	被清除的键
//
// Return values: None
///////////////////////////////////////////////////////////////////////
void _RemoveKey( CHAR*& key );

///////////////////////////////////////////////////////////////////////
// Function		: _CompareKey< TKey >
//
// Description	: 缺省的键值比较函数（赋给哈西表中的函数指针），
//				  用于除字符串外的其他所有键类型
//
// Args			:
//				  1. keyA	TKey	用于比较的键A
//				  2. keyB	TKey	用于比较的键B
//
// Return values:
//				  BOOL	键A与键B是否相等
///////////////////////////////////////////////////////////////////////
template < class TKey >
BOOL _CompareKey( TKey& keyA, TKey& keyB )
{
	return ( keyA == keyB );
}

///////////////////////////////////////////////////////////////////////
// Function		: _CompareKey< CHAR* >
//
// Description	: 用于字符串的键值比较函数
//
// Args			:
//				  1. keyA	CHAR*	用于比较的键A
//				  2. keyB	CHAR*	用于比较的键B
//
// Return values:
//				  BOOL	键A与键B是否相等
///////////////////////////////////////////////////////////////////////
BOOL _CompareKey( CHAR*& keyA, CHAR*& keyB );

///////////////////////////////////////////////////////////////////////
// Function		: _GetHash< TKey >
//
// Description	: 缺省的哈西函数（赋给哈西表中的函数指针），
//				  用于除字符串外的其他所有键类型
//
// Args			:
//				  1. key	TKey	用于计算哈西值的键
//
// Return values:
//				  HASH_CODE		键所对应的哈西值
///////////////////////////////////////////////////////////////////////
template < class TKey >
HASH_CODE _GetHash( TKey& key )
{
	// 直接使用类型转换。自定义类型的键需要重载此类型的转换操作符。
	return ( HASH_CODE )key;
}

///////////////////////////////////////////////////////////////////////
// Function		: _GetHash< CHAR* >
//
// Description	: 用于字符串的哈西函数。这是一种简单的实现，
//				  只是根据字符串的第一位来计算哈西值。对于不适用的情况，
//				  应该指定并使用自定义的哈西函数。
//
// Args			:
//				  1. key	CHAR*	用于计算哈西值的键
//
// Return values:
//				  HASH_CODE		键所对应的哈西值
///////////////////////////////////////////////////////////////////////
HASH_CODE _GetHash( CHAR*& key );

///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// CHashTable< TKey, TValue > 的方法实现
///////////////////////////////////////////////////////////////////////

// 哈西表的构造函数，其中调用了私有方法initTable()来初始化哈西表
template < class TKey, class TValue >
inline CHashTable< TKey, TValue >::CHashTable()
{
	initTable( DEFAULT_HASHTABLE_LENGTH, DEFAULT_HASHTABLE_LOADFACTOR,
				_CompareKey, _GetHash );
}

template < class TKey, class TValue >
inline CHashTable< TKey, TValue >::CHashTable( UINT length )
{
	initTable( length, DEFAULT_HASHTABLE_LOADFACTOR,
				_CompareKey, _GetHash );
}

template < class TKey, class TValue >
inline CHashTable<TKey, TValue>::CHashTable(PF_HASH pfGetHash )
{
	initTable( DEFAULT_HASHTABLE_LENGTH, DEFAULT_HASHTABLE_LOADFACTOR, _CompareKey, pfGetHash );
}

template < class TKey, class TValue >
inline CHashTable< TKey, TValue >::CHashTable( UINT length,
												DOUBLE loadFactor )
{
	initTable( length, loadFactor, _CompareKey, _GetHash );
}

template < class TKey, class TValue >
inline CHashTable< TKey, TValue >::CHashTable( PF_CMP pfCompareKey,
												PF_HASH pfGetHash )
{
	initTable( DEFAULT_HASHTABLE_LENGTH,
				DEFAULT_HASHTABLE_LOADFACTOR,
				pfCompareKey, pfGetHash );
}

template < class TKey, class TValue >
inline CHashTable< TKey, TValue >::CHashTable( UINT length,
												DOUBLE loadFactor,
												PF_CMP pfCompareKey,
												PF_HASH pfGetHash )
{
	initTable( length, loadFactor, pfCompareKey, pfGetHash );
}

// 用于初始化哈西表的私有方法
template < class TKey, class TValue >
void CHashTable< TKey, TValue >::initTable( UINT length,
											DOUBLE loadFactor,
											PF_CMP pfCompareKey,
											PF_HASH pfGetHash )
{
	// 根据预定义的最大最小值，对表长进行限制
	if ( length < HASHTABLE_MIN_LENGTH )
		m_tableLength = HASHTABLE_MIN_LENGTH;
	else if ( length > HASHTABLE_MAX_LENGTH )
		m_tableLength = HASHTABLE_MAX_LENGTH;
	else
		m_tableLength = length;
	// 分配表空间，并清空
	m_table = new _PEntry[m_tableLength];
	memset( m_table, 0, sizeof( _PEntry ) * m_tableLength );
	seqHead = NULL;
	seqTail = NULL;
	seqCur = NULL;

	// 根据预定义的最大最小值，对装填因子进行限制
	if ( loadFactor < HASHTABLE_MIN_LOADFACTOR )
		m_loadFactor = HASHTABLE_MIN_LOADFACTOR;
	else if ( loadFactor > HASHTABLE_MAX_LOADFACTOR )
		m_loadFactor = HASHTABLE_MAX_LOADFACTOR;
	else
		m_loadFactor = loadFactor;

	// 由表长和装填因子，计算出哈西表再哈西算法的触发门限值
	m_threshold = ( UINT )( m_tableLength * m_loadFactor );

	// 初始化键值比较函数和哈西函数的函数指针
	if ( !pfCompareKey ) m_pfCompareKey = _CompareKey;
	else m_pfCompareKey = pfCompareKey;

	if ( !pfGetHash )
		m_pfGetHash = _GetHash;
	else
		m_pfGetHash = pfGetHash;

	// 初始化节点计数和更新计数
	m_entryCount = 0;
	m_modCount = 0;

	// 初始化用于遍历的当前节点指针，以及索引值
	m_pcurEntry = NULL;
	m_uicurIndex = 0;
}

template < class TKey, class TValue >
CHashTable< TKey, TValue >::~CHashTable()
{
	clear();			// 清空哈西表
	delete[] m_table;	// 释放表空间
	m_table = NULL;
	m_tableLength = 0;
}

///////////////////////////////////////////////////////////////////////
// Function		: CHashTable< TKey, TValue >::Clear
//
// Description	: 清空哈西表
//
// Args			: None
//
// Return values: None
///////////////////////////////////////////////////////////////////////
template < class TKey, class TValue >
void CHashTable< TKey, TValue >::clear( void )
{
	// 扫描整个哈西表，释放每一个节点
	for ( UINT i = 0; i < m_tableLength; i ++ )
	{
		while ( m_table[i] )
		{
			_PEntry p = m_table[i];
			m_table[i] = p->m_next;
			p->m_next = NULL;
			delSequence(p);
			delete p;
		}
	}
	m_entryCount = 0;	// 节点计数重置为0
	m_modCount ++;		// 更新（修改）计数加1

	m_pcurEntry = NULL;
	m_uicurIndex = 0;
}

///////////////////////////////////////////////////////////////////////
// Function		: CHashTable< TKey, TValue >::containsKey
//
// Description	: 检索哈西表中是否已包含这个键
//
// Args			:
//				  1. Key	TKey	用于检索的键
//
// Return values:
//				  BOOL	是否包含这个键
///////////////////////////////////////////////////////////////////////
template < class TKey, class TValue >
BOOL CHashTable< TKey, TValue >::containsKey( TKey key )
{
	HASH_CODE hash = ( *m_pfGetHash )( key );	// 计算哈西值
	UINT index = GET_HASH_INDEX( hash );				// 映射哈西表索引号

	// 搜索此索引号下的所有节点
	_PEntry p = m_table[index];
	while ( p )
	{
		// 调用键值比较函数，判断是否是所搜索的节点
		if ( ( *m_pfCompareKey )( key, p->m_key ) )
		{
			return TRUE;	// 找到，返回TRUE
		}
		p = p->m_next;
	}
	return FALSE;	// 没有找到，返回FALSE
}

///////////////////////////////////////////////////////////////////////
// Function		: CHashTable< TKey, TValue >::get
//
// Description	: 检索出哈西表中的一个项
//
// Args			:
//				  1. key	TKey	用于检索的键
//				  2. value	TValue&	存放检索到的内容
//
// Return values:
//				  BOOL	是否成功检索到相应的项
///////////////////////////////////////////////////////////////////////
template < class TKey, class TValue >
BOOL CHashTable< TKey, TValue >::get_r( TKey& key, TValue& value )
{
	_PEntry p = m_table[GET_HASH_INDEX((*m_pfGetHash)(key))];
	while ( p )
	{
		// 调用键值比较函数，判断是否是所搜索的节点
		if ( ( *m_pfCompareKey )( key, p->m_key ) )
		{
			// 找到，给用于返回内容的value参数赋值，然后返回TRUE。
			// 这里使用直接‘=‘赋值，对于复杂类型，应另定义拷贝构造函数
			value = p->m_value;
			return TRUE;
		}
		p = p->m_next;
	}
	return FALSE;	// 没找到相应的项，返回FALSE
}
template < class TKey, class TValue >
BOOL CHashTable< TKey, TValue >::get( TKey key, TValue& value )
{
	return get_r(key, value);
}

///////////////////////////////////////////////////////////////////////
// Function		: CHashTable< TKey, TValue >::Put
//
// Description	: 在哈西表中存放一个项，如果此键已有对应的项，则覆盖原
//				  有的内容，否则新建一个条目
//
// Args			:
//				  1. key	TKey	此项所对应的键
//				  2. value	TValue	所存放的内容
//
// Return values:
//				  TValue	如果这个键在哈西表中已有对应的内容，则返回
//							这个内容，否则返回新加入的内容
///////////////////////////////////////////////////////////////////////
template < class TKey, class TValue >
TValue CHashTable< TKey, TValue >::put_r( TKey& key, TValue& value )
{
	HASH_CODE hash = ( *m_pfGetHash )( key );	// 计算哈西值
	UINT index = GET_HASH_INDEX( hash );				// 映射哈西表索引号
	_PEntry p = m_table[index];

	m_modCount ++;		// 更新计数加1

	// 察看是否已有对应的节点
	while ( p )
	{
		if ( ( *m_pfCompareKey )( key, p->m_key ) )
		{
			// 找到旧的节点
			TValue oldValue = p->m_value;
			p->m_value = value;				// 更新节点内容
			return oldValue;				// 返回旧的内容
		}
		p = p->m_next;
	}

	// 新加入一个节点
	m_entryCount ++;	// 节点计数加1

	// 中断已有的遍历
	m_pcurEntry = NULL;
	m_uicurIndex = 0;

	// 如果节点数达到或超过再哈西门限值，则触发再哈西算法
	if ( m_entryCount >= m_threshold )
	{
		rehash();					// 触发再哈西算法
		index = GET_HASH_INDEX( hash );	// 重新映射哈西表索引号
	}

	// 分配新的节点，并加入到哈西表中
	_PEntry newEntry = new _Entry( hash, key, value, m_table[index] );
	m_table[index] = newEntry;
	addSequence(newEntry);

	return value;	// 返回新加入的内容
}
template < class TKey, class TValue >
TValue CHashTable< TKey, TValue >::put( TKey key, TValue value )
{
	return put_r(key, value);
}

template < class TKey, class TValue >
void CHashTable< TKey, TValue >::put_v( TKey& key, TValue& value )
{
	HASH_CODE hash = ( *m_pfGetHash )( key );// 计算哈西值
	UINT index = GET_HASH_INDEX( hash );		// 映射哈西表索引号
	_PEntry p = m_table[index];

	m_modCount ++;	// 更新计数加1

	// 察看是否已有对应的节点
	while ( p )
	{
		if ( ( *m_pfCompareKey )( key, p->m_key ) )
		{
			// 找到旧的节点
			p->m_value = value;	// 更新节点内容
			return;
		}
		p = p->m_next;
	}

	// 新加入一个节点
	m_entryCount ++;	// 节点计数加1

	// 中断已有的遍历
	m_pcurEntry = NULL;
	m_uicurIndex = 0;

	// 如果节点数达到或超过再哈西门限值，则触发再哈西算法
	if ( m_entryCount >= m_threshold )
	{
		rehash();					// 触发再哈西算法
		index = GET_HASH_INDEX( hash );	// 重新映射哈西表索引号
	}

	// 分配新的节点，并加入到哈西表中
	_PEntry newEntry = new _Entry( hash, key, value, m_table[index] );
	m_table[index] = newEntry;
	addSequence(newEntry);
}

///////////////////////////////////////////////////////////////////////
// Function		: CHashTable< TKey, TValue >::Remove
//
// Description	: 从哈西表中移除一个项
//
// Args			:
//				  1. key	TKey	此项对应的键
//
// Return values:
//				  BOOL	是否成功移除
///////////////////////////////////////////////////////////////////////
template < class TKey, class TValue >
BOOL CHashTable< TKey, TValue >::remove( TKey key )
{
	HASH_CODE hash = ( *m_pfGetHash )( key );	// 计算哈西值
	UINT index = GET_HASH_INDEX( hash );				// 映射哈西表索引号
	_PEntry p = m_table[index];

	if ( !p ) return FALSE;	// 此索引号下没有节点，即没找到指定的项

	// 判断此索引号下第一个节点是否所搜索的项
	if ( ( *m_pfCompareKey )( key, p->m_key ) )
	{
		// 所要删除的项是此索引号下的第一个节点
		// 移除这个节点
		if ( p == m_pcurEntry )	// 所移除的节点正好是遍历中的当前节点
			searchNextEntry();

		m_table[index] = p->m_next;
		delSequence(p);
		delete p;
		m_entryCount --;	// 节点计数减1
		m_modCount ++;		// 更新计数加1

		return TRUE;
	}

	// 搜索此索引号下其他的节点
	_PEntry prev = p;
	p = p->m_next;
	while ( p )
	{
		if ( ( *m_pfCompareKey )( key, p->m_key ) )
		{
			// 找到所要删除的项
			// 移除这个节点

			//Modified by Draon.(Check by LiJinglin), 2002.12.25
			// 按照新的编程规范，变量应该在后面
			//if ( p = m_pcurEntry )		// 所移除的节点正好是遍历中的当前节点
			if (  m_pcurEntry == p )		// 所移除的节点正好是遍历中的当前节点
				searchNextEntry();

			prev->m_next = p->m_next;
			delSequence(p);
			delete p;
			m_entryCount --;	// 节点计数减1
			m_modCount ++;		// 更新计数加1
			return TRUE;
		}
		prev = p;
		p = p->m_next;
	}

	return FALSE;	// 没有找到指定的项
}

///////////////////////////////////////////////////////////////////////
// Function		: CHashTable< TKey, TValue >::size
//
// Description	: 获取当前哈西表中项的总数
//
// Args			: None
//
// Return values:
//				  UINT	当前项的总数
///////////////////////////////////////////////////////////////////////
template < class TKey, class TValue >
inline UINT CHashTable< TKey, TValue >::size( void )
{
	return m_entryCount;
}

///////////////////////////////////////////////////////////////////////
// Function		: CHashTable< TKey, TValue >::isEmpty
//
// Description	: 判断哈西表是否为空
//
// Args			: None
//
// Return values:
//				  BOOL	哈西表是否为空
///////////////////////////////////////////////////////////////////////
template < class TKey, class TValue >
inline BOOL CHashTable< TKey, TValue >::isEmpty( void )
{
	if ( m_entryCount == 0 )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

///////////////////////////////////////////////////////////////////////
// Function		: CHashTable< TKey, TValue >::reset
//
// Description	: 重置遍历指针，一次遍历前必须进行的动作。将遍历指针
//					指向表中的第一个节点。
//
// Args			: None
//
// Return values:
//				  BOOL	如果哈西表为空则返回FALSE，否则返回TRUE。
///////////////////////////////////////////////////////////////////////

template < class TKey, class TValue >
void CHashTable< TKey, TValue >::resetSeq( void )
{
	seqCur = seqHead;
}

template < class TKey, class TValue >
BOOL CHashTable< TKey, TValue >::reset( void )
{
	for ( UINT i = 0; i < m_tableLength; i ++ )
	{
		if ( m_table[i] )
		{
			m_pcurEntry = m_table[i];
			m_uicurIndex = i;
			return TRUE;
		}
	}
	m_pcurEntry = NULL; //added by lxm. 2008.09.28。if is empty, should set it to NULL.
	m_uicurIndex =0;
	return FALSE;
}

///////////////////////////////////////////////////////////////////////
// Function		: CHashTable< TKey, TValue >::getNext
//
// Description	: 获取下一个项。反复调用这个方法来实现遍历。
//
// Args			: 1. value	TValue&	用于存放所取得的项。
//
// Return values:
//				  BOOL	返回FALSE表示没有取到项，也就是说没有后续项了。
///////////////////////////////////////////////////////////////////////

template < class TKey, class TValue >
BOOL CHashTable< TKey, TValue >::getNext( TValue & value )
{
	if ( !m_pcurEntry )
		return FALSE;

	//　给参数赋值
	value = m_pcurEntry->m_value;

	// 搜索下一个节点

	searchNextEntry();

	return TRUE;
}

//Added by LJL 2008.08.5
template < class TKey, class TValue >
BOOL CHashTable< TKey, TValue >::getNext( TKey &key, TValue &value )
{
	if ( !m_pcurEntry )
		return FALSE;

	//　给参数赋值
	key = m_pcurEntry->m_key;
	value = m_pcurEntry->m_value;

	// 搜索下一个节点
	searchNextEntry();

	return TRUE;
}

template < class TKey, class TValue >
BOOL CHashTable< TKey, TValue >::getNextSeq( TKey &key, TValue &value )
{
	if(seqCur == NULL)
		return FALSE;
	key = seqCur->m_key;
	value = seqCur->m_value;
	seqCur = seqCur->m_s_next;
	return  TRUE;
}

template < class TKey, class TValue >
BOOL CHashTable< TKey, TValue >::searchNextEntry( void )
{
	if ( m_pcurEntry->m_next )
	{
		m_pcurEntry = m_pcurEntry->m_next;
		return TRUE;
	}
	else
	{
		for ( UINT i = m_uicurIndex + 1; i < m_tableLength; i ++ )
		{
			if ( m_table[i] )
			{
				m_pcurEntry = m_table[i];
				m_uicurIndex = i;
				return TRUE;
			}
		}

		// 再也没有后续节点了
		m_pcurEntry = NULL;
		m_uicurIndex = 0;
		return FALSE;
	}
}

template < class TKey, class TValue >
void CHashTable< TKey, TValue >::addSequence(_Entry* entry)
{
	if(seqHead == NULL || seqTail == NULL)
	{
		entry->m_s_next = NULL;
		entry->m_s_prev = NULL;
		seqHead = seqTail = entry;
		return;
	}
	entry->m_s_next = NULL;
	entry->m_s_prev = seqTail;
	seqTail->m_s_next = entry;
	seqTail = entry;
}
template < class TKey, class TValue >
void CHashTable< TKey, TValue >::delSequence(_Entry* entry)
{
	if(entry->m_s_next == NULL && entry->m_s_prev == NULL) // only one
	{
		seqHead = seqTail = NULL;
		return;
	}
	if(entry->m_s_next == NULL) // is tail
	{
		seqTail = entry->m_s_prev;
		seqTail->m_s_next = NULL;
		return;
	}
	if(entry->m_s_prev == NULL) // is head
	{
		seqHead = entry->m_s_next;
		seqHead->m_s_prev = NULL;
		return;
	}
	entry->m_s_next->m_s_prev = entry->m_s_prev;
	entry->m_s_prev->m_s_next = entry->m_s_next;
}
template < class TKey, class TValue >
void CHashTable< TKey, TValue >::printSequence()
{
	int i = 0;
	_Entry* p = seqHead;
	while(p!=NULL)
	{
		printf("[%d] %p <- %p -> %p\n", i, p->m_s_prev, p, p->m_s_next);
		p = p->m_s_next;
		i++;
	}
	printf("%d entries\n\n", i);
}

///////////////////////////////////////////////////////////////////////
// Function		: CHashTable< TKey, TValue >::rehash
//
// Description	: 再哈西函数。当哈西表中项的数目突破再哈西门限值，且
//				  哈西表总长度不超过长度最大值时，这个函数会被调用。
//				  其效果是增大哈西表长度，重新分配各个项的位置。
//
// Args			: None
//
// Return values: None
///////////////////////////////////////////////////////////////////////
template < class TKey, class TValue >
void CHashTable< TKey, TValue >::rehash( void )
{
	// 哈西表长的增长达到上限，不再增大
	if ( m_tableLength >= HASHTABLE_MAX_LENGTH ) return;

	UINT oldLength = m_tableLength;
	m_tableLength = m_tableLength * 2;	// 哈西表长增大为原来的两倍
	// 新的表长不能大于表长最大值
	if ( m_tableLength >= HASHTABLE_MAX_LENGTH )
		m_tableLength = HASHTABLE_MAX_LENGTH;

	// 分配新的表空间，并初始化
	_PEntry* oldTable = m_table;
	m_table = new _PEntry[m_tableLength + 3];
	memset( m_table, 0, sizeof( _PEntry ) * m_tableLength );

	// 计算新的再哈西门限值
	m_threshold = ( UINT )( m_tableLength * m_loadFactor );

	m_modCount ++;	// 更新计数加1

	// 转移旧表中的节点至新表
	_PEntry pentry;
	UINT index;
	for ( UINT i = 0; i < oldLength; i ++ )
	{
		while ( oldTable[i] )
		{

			pentry = oldTable[i];
			oldTable[i] = oldTable[i]->m_next;
			// 根据节点中记录的哈西值，重新映射哈西表索引号
			index = GET_HASH_INDEX( pentry->m_hash );
			pentry->m_next = m_table[index];
			m_table[index] = pentry;
		}
	}
	delete[] oldTable;	// 删除旧的哈西表，释放内存空间
}


///////////////////////////////////////////////////////////////////////
// CHashTable< TKey, TValue >::_Entry 类的静态属性（函数指针）初始化
///////////////////////////////////////////////////////////////////////

template < class TKey, class TValue >
void ( *CHashTable< TKey, TValue >::_Entry::m_spfSetKey )( TKey&, TKey )
	= _SetKey;

template < class TKey, class TValue >
void ( *CHashTable< TKey, TValue >::_Entry::m_spfRemoveKey )( TKey& )
	= _RemoveKey;

///////////////////////////////////////////////////////////////////////

#endif	// _CHASHTABLE_H
