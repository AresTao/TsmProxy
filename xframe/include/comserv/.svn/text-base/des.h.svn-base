#ifndef	__DES_h__
#define	__DES_h__

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef U8
#define U8 unsigned char
#endif

#ifndef LPBYTE
#define LPBYTE uchar *
#endif 
///////////////////////////////////////////
//以下部分为软件加密部分
//DES要求:
//key[8] text[8] mtext[8]
//singleDES加密算法
extern void DES(U8 * key,U8 * text,U8 * mtext);
//singleDES解密算法
extern void DES_1(U8 * key,U8 * text,U8 * mtext);

//3DES要求:
//key[16] text[8] mtext[8]
//TripleDES加密算法 
extern void TripleDES  (U8 * key16,U8 * text,U8 * mtext);
//TripleDES解密算法
extern void TripleDES_1(U8 * key16,U8 * text,U8 * mtext);

#endif
