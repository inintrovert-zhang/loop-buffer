/**
  ******************************************************************************
  * 版权所有： 
  * 文件名:   loop_buffer.h
  * 作者：    zwy
  * 创建日期: 2023.05.13
  * 描述： 	此文档提供了对数据帧环形缓冲区的队列操作。适用于数据帧长度不固定，每次入列的
  *      数据，在出列操作能完整读出来，一般用于通信数据缓存。
  * 
  * 注意：数据帧的长度信息和数据信息都存放在同一个缓冲区，数据帧长度信息占用2个字节
  ******************************************************************************
  */

#ifndef LOOP_BUFFER_H
#define LOOP_BUFFER_H

typedef unsigned char u8;
typedef char s8;
typedef unsigned short u16;
typedef short s16;
typedef unsigned int u32;
typedef int s32;



typedef struct 
{
    u8 *pBuf;           //指向缓存区
    u16 BufSize;        //缓冲区长度
    u16 DatStartPos;    //数据起始位置
    u16 BlankStartPos;  //空白区起始位置
}LOOP_BUFFER_DEF;


enum
{
	QUEUE_OP_SUCCESS = 0,          //操作成功
	QUEUE_OP_FAIL = (-1),          //操作失败
	QUEUE_OP_ERR_NO_MEM = (-2),    //队列已满
	QUEUE_OP_ERR_EMPTY = (-3),     //队列已空
};

u16 GetBlankSize(LOOP_BUFFER_DEF *p_loop_buf);
s32 LoopBuffer_Init(LOOP_BUFFER_DEF *p_loop_buf, u8 *p_buf, u16 buf_len);
s32 LoopBuffer_Enqueue(LOOP_BUFFER_DEF *p_loop_buf,const u8 *p_buf, u16 buf_len);
s32 LoopBuffer_Dequeue(LOOP_BUFFER_DEF *p_loop_buf,u8 *p_buf, u16 buf_len);


#endif