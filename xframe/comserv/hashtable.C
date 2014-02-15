#include "unihashtable.h"

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
void _SetKey( CHAR*& dest, CHAR* src )
{
	int len = strlen( src );
	dest = new CHAR[len + 1];
	strcpy( dest, src );
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
void _RemoveKey( CHAR*& key )
{
	if ( key ) delete[] key;
	key = 0;
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
BOOL _CompareKey( CHAR*& keyA, CHAR*& keyB )
{
	return ( strcmp(keyA, keyB) == 0 );
}

///////////////////////////////////////////////////////////////////////
// Function		: _GetHash< CHAR* >
//
// Description	: 用于字符串的哈西函数。这是一种简单的实现，
//				  只是根据字符串的第一位来计算哈西值。对于不适用的情况，
//				  应该指定并使用自定义的哈西函数。
//
//		  使用新的键值生成算法。
//  哈希键值＝ 字符串长度×10 ＋ 首字母ASCII值＋末字母ASCII值＋中间字母ASCII值
// Args			:
//				  1. key	CHAR*	用于计算哈西值的字符串
//
// Return values:
//				  HASH_CODE		键所对应的哈西值
///////////////////////////////////////////////////////////////////////
HASH_CODE _GetHash( CHAR*& key )
{
	HASH_CODE temp_node;
	UINT i = strlen (key);	
	if (i <= 0) 
	{
		temp_node =0;
	}
	else
	{
		// 计算键值
		temp_node = i *10;
		temp_node += (HASH_CODE)key[0];
		temp_node += (HASH_CODE)key[i-1];
		temp_node += (HASH_CODE)key[i/2];		
	}
	return temp_node;
//	return ( HASH_CODE )key[0];
}
