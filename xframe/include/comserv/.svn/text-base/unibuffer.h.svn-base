//============================================================================
// Name        : unibuffer.h
// Author      : Yubo Chow, lxm.
// Version     :
// Copyright   : 
// Description : a general buffer.
//============================================================================


#ifndef __UNIBUFFER_H_
#define __UNIBUFFER_H_

template <class type> class TBuffer
{
public:
	TBuffer();
	TBuffer(int bufferSize);
	typedef int(type::*pfFunc)(void *dst, const void *src, unsigned int length);

	//TBuffer(int bufferSize, int type::popFunction(void *dst, const void *src, unsigned int length), int type::pushFunction(void *dst, const void *src, unsigned int length));
	TBuffer(int bufferSize, type *master, pfFunc popFunction, pfFunc pushFunction);
	~TBuffer();

	int writeIntoBuffer(const char *src, int length);
	int readFromBuffer(char *dst, int length);
	void reset();
	void rearrange();
	int size()
	{	return m_size;}
	int freeLength()
	{	return m_freeLength;}
	int filledLength()
	{	return m_filledLength;}

private:
	type *m_master;
	char *msgBufEnd;
	char *msgBuf;
	char *pBufHead;
	char *pBufTail;

	int m_size;
	int m_filledLength;
	int m_freeLength;

	//int(type::*popFromBuff)(void *dst, const void *src, unsigned int length);
	//int(type::*pushIntoBuff)(void *dst, const void *src, unsigned int length);
	pfFunc popFromBuff;
	pfFunc pushIntoBuff;

	int isBasicMaster;
};

class TBufferMaster
{
public:
	int _pop(void *dst, const void *src, unsigned int length)
	{
		int ret = length;
		memcpy(dst, src, length);
		return ret;
	}

	int _push(void *dst, const void *src, unsigned int length)
	{
		int ret = length;
		memcpy(dst, src, length);
		return ret;
	}

};

template <class type> TBuffer<type>::TBuffer()
{
	m_freeLength = m_size = 2048000;// 2 M
	m_filledLength = 0;
	msgBuf = pBufHead = pBufTail = new char[m_size];
	msgBufEnd = msgBuf + m_size;

	m_master = new TBufferMaster;
	popFromBuff =((TBufferMaster*)m_master)->_pop;
	pushIntoBuff= ((TBufferMaster*)m_master)->_push;
	isBasicMaster=1;

}

template <class type> TBuffer<type>::TBuffer(int bufferSize)
{
	m_freeLength = m_size = bufferSize;
	m_filledLength = 0;
	msgBuf = pBufHead = pBufTail = new char[m_size];
	msgBufEnd = msgBuf + m_size;

	m_master = new TBufferMaster;
	popFromBuff = ((TBufferMaster*)m_master)->_pop;
	pushIntoBuff= ((TBufferMaster*)m_master)->_push;
	isBasicMaster=1;

}

template <class type>
//TBuffer<type>::TBuffer(int bufferSize, int type::popFunction(void *dst, const void *src, unsigned int length), int type::pushFunction(void *dst, const void *src, unsigned int length))
TBuffer<type>::TBuffer(int bufferSize, type *master, pfFunc popFunction,
		pfFunc pushFunction)
{
	m_freeLength = m_size = bufferSize;
	m_filledLength = 0;
	msgBuf = pBufHead = pBufTail = new char[m_size];
	msgBufEnd = msgBuf + m_size;
	m_master = master;
	popFromBuff = popFunction;
	pushIntoBuff = pushFunction;
	isBasicMaster=0;

}

template <class type> TBuffer<type>::~TBuffer()
{
	if (m_size>0)
		delete[] msgBuf;
	if (isBasicMaster)
		delete (TBufferMaster*)m_master;

	msgBuf=NULL;
	m_size=0;
}

template <class type> void TBuffer<type>::reset()
{
	m_freeLength = m_size;
	m_filledLength = 0;
	pBufHead = pBufTail = msgBuf;
}

template <class type> void TBuffer<type>::rearrange()
{
	if (pBufHead == msgBuf)
	{
		return;
	}
	if (pBufHead < pBufTail)
	{
		memcpy(msgBuf, pBufHead, m_filledLength);
	}
	else
	{
		int frontLength = msgBufEnd - pBufHead;
		int endLength = pBufTail - msgBuf;

		if (frontLength <= m_freeLength)
		{
			// rearrange a incompact buffer
			memmove(frontLength + msgBuf, msgBuf, endLength);
			memcpy(msgBuf, pBufHead, frontLength);
		}
		else
		{
			// rearrange a compact buffer
			char *temp = new char[endLength];
			memcpy(temp, msgBuf, endLength);
			memcpy(msgBuf, pBufHead, frontLength);
			memcpy(msgBuf + frontLength, temp, endLength);
			delete temp;
		}
	}
	pBufHead = msgBuf;
	pBufTail = msgBuf + m_filledLength;
}

template <class type> int TBuffer<type>::readFromBuffer(char *dst, int length)
{
	if (length <= 0)
	{
		return length;
	}
	if (m_filledLength <= 0)
	{
		return m_filledLength;
	}

	int ret = 0;
	length=length <= m_filledLength ? length : m_filledLength;

	if (pBufHead < pBufTail)
	{
		ret = (m_master->*popFromBuff)(dst, pBufHead, length);
		if (ret > 0 && ret <= length)
		{
			pBufHead += ret;
			m_filledLength -= ret;
			m_freeLength += ret;
		}
	}
	else
	{
		int lengthTemp;
		lengthTemp = msgBufEnd - pBufHead;
		if (length <= lengthTemp)
		{
			ret = (m_master->*popFromBuff)(dst, pBufHead, length);
			if (ret > 0 && ret <= length)
			{
				pBufHead += ret;
				m_filledLength -= ret;
				m_freeLength += ret;
			}
		}
		else
		{
			ret = (m_master->*popFromBuff)(dst, pBufHead, lengthTemp);
			if (ret == lengthTemp)
			{
				lengthTemp = length - lengthTemp;
				int retOfSecondRead = (m_master->*popFromBuff)(dst + ret,
						msgBuf, lengthTemp);
				if (retOfSecondRead> 0 && retOfSecondRead <= lengthTemp)
				{
					pBufHead = msgBuf + retOfSecondRead;
					ret += retOfSecondRead;
					m_filledLength -= ret;
					m_freeLength += ret;
				}
			}
			else
			{
				if (ret > 0 && ret <= lengthTemp)
				{
					pBufHead += ret;
					m_filledLength -= ret;
					m_freeLength += ret;
				}
			}
		}
	}

	if (m_filledLength == 0)
	{
		pBufHead = pBufTail = msgBuf;
	}

	return ret;
}

template <class type> int TBuffer<type>::writeIntoBuffer(const char *src,
		int length)
{
	if (length <= 0)
	{
		return length;
	}
	if (m_freeLength <= 0)
	{
		return m_freeLength;
	}
	int ret = 0;
	length=length <= m_freeLength ? length : m_freeLength;

	if (pBufHead > pBufTail)
	{
		ret = (m_master->*pushIntoBuff)(pBufTail, src, length);
		if (ret> 0 && ret <= length)
		{
			pBufTail += ret;
			m_filledLength += ret;
			m_freeLength -= ret;
		}
	}
	else
	{
		int lengthTemp;
		lengthTemp = msgBufEnd - pBufTail;
		if (length <= lengthTemp)
		{
			ret = (m_master->*pushIntoBuff)(pBufTail, src, length);
			if (ret > 0 && ret <= length)
			{
				pBufTail += ret;
				m_filledLength += ret;
				m_freeLength -= ret;
			}
		}
		else
		{
			ret = (m_master->*pushIntoBuff)(pBufTail, src, lengthTemp);
			if (ret == lengthTemp)
			{
				lengthTemp = length - lengthTemp;
				int retOfSecondWrite = (m_master->*pushIntoBuff)(msgBuf, src
						+ ret, lengthTemp);
				if (retOfSecondWrite> 0 && retOfSecondWrite <= lengthTemp)
				{
					pBufTail = msgBuf + retOfSecondWrite;
					ret += retOfSecondWrite;
					m_filledLength += ret;
					m_freeLength -= ret;
				}
			}
			else
			{
				if (ret > 0 && ret <= lengthTemp)
				{
					pBufTail += ret;
					m_filledLength += ret;
					m_freeLength -= ret;
				}
			}
		}
	}

	return ret;
}

#endif
