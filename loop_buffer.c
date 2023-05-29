/**
  ******************************************************************************
  * 版权所有： 
  * 文件名:   loop_buffer.c 
  * 作者：    zwy
  * 创建日期: 2023.05.13
  * 描述： 	此文档提供了对数据帧环形缓冲区的队列操作。适用于数据帧长度不固定，每次入列的
  *      数据，在出列操作能完整读出来，一般用于通信数据缓存。
  * 
  * 注意：数据帧的长度信息和数据信息都存放在同一个缓冲区，数据帧长度信息占用2个字节
  ******************************************************************************
  */

#include "loop_buffer.h"
#include <string.h>
#include <stdbool.h>


/******************************************************************************
 * 函数名:	GetNextPos 
 * 描述: 
 *            --队列索引值往后移一位
 * 输入参数: 
 * 				p_loop_buf-队列指针
 * 				now_pos-当前索引
 * 输出参数: 
 *				
 * 返回值: 
 * 				下个索引值
 *				 
 * 				
 * 作者: 
 * 创建日期:
 * 
 *------------------------
 * 修改人:
 * 修改日期:
 ******************************************************************************/
u16 GetNextPos(LOOP_BUFFER_DEF *p_loop_buf, u16 now_pos)
{
    return now_pos == (p_loop_buf->BufSize - 1) ? 0 : (now_pos + 1);
}

/******************************************************************************
 * 函数名:	LoopBuffer_IsFull 
 * 描述: 
 *            -判断队列满
 * 输入参数: 
 * 				p_loop_buf-队列指针
 * 				
 * 输出参数: 
 *				
 * 返回值: 
 * 				true-队列满,false-未满
 *				
 * 				
 * 作者: 
 * 创建日期:
 * 
 *------------------------
 * 修改人:
 * 修改日期:
 ******************************************************************************/
bool LoopBuffer_IsFull(LOOP_BUFFER_DEF *p_loop_buf)
{
    if(GetNextPos(p_loop_buf, p_loop_buf->BlankStartPos) == p_loop_buf->DatStartPos)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/******************************************************************************
 * 函数名:	LoopBuffer_IsEmpty 
 * 描述: 
 *            -判断队列空
 * 输入参数: 
 * 				p_loop_buf-队列指针
 * 				
 * 输出参数: 
 *				buf-出列数据
 * 返回值: 
 * 				
 *				true-队列空,false-未空
 * 				
 * 作者: 
 * 创建日期:
 * 
 *------------------------
 * 修改人:
 * 修改日期:
 ******************************************************************************/
bool LoopBuffer_IsEmpty(LOOP_BUFFER_DEF *p_loop_buf)
{
    if(p_loop_buf->BlankStartPos == p_loop_buf->DatStartPos)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/******************************************************************************
 * 函数名:	GetBlankSize 
 * 描述: 
 *            -队列出列操作
 * 输入参数: 
 * 				p_loop_buf-队列指针
 * 				
 * 输出参数: 
 *				
 * 返回值: 
 * 				返回队列空间剩余字节数
 *				
 * 				
 * 作者: 
 * 创建日期:
 * 
 *------------------------
 * 修改人:
 * 修改日期:
 ******************************************************************************/
u16 GetBlankSize(LOOP_BUFFER_DEF *p_loop_buf)
{
    if(p_loop_buf->BlankStartPos < p_loop_buf->DatStartPos)
    {
        return (p_loop_buf->DatStartPos - p_loop_buf->BlankStartPos - 1);
    }
    else
    {
        return (p_loop_buf->DatStartPos  +  p_loop_buf->BufSize - p_loop_buf->BlankStartPos - 1);
    }
}

/******************************************************************************
 * 函数名:	LoopBuffer_Init 
 * 描述: 
 *            -队列初始化
 * 输入参数: 
 * 				p_loop_buf-队列指针
 * 				p_buf-缓冲区指针
 *              buf_len-缓冲区大小
 * 输出参数: 
 *				
 * 返回值: 
 * 				QUEUE_OP_SUCCESS-操作成功
 *				QUEUE_OP_FAIL-操作失败，参数有误 
 * 				
 * 作者: 
 * 创建日期:
 * 
 *------------------------
 * 修改人:
 * 修改日期:
 ******************************************************************************/
s32 LoopBuffer_Init(LOOP_BUFFER_DEF *p_loop_buf, u8 *p_buf, u16 buf_len)
{
    if(p_loop_buf == NULL || p_buf == NULL || buf_len == 0)
    {
        return QUEUE_OP_FAIL;
    }

    p_loop_buf->pBuf = p_buf;
    p_loop_buf->BufSize = buf_len;
    p_loop_buf->BlankStartPos = 0;
    p_loop_buf->DatStartPos = 0;
    return QUEUE_OP_SUCCESS;
}

/******************************************************************************
 * 函数名:	LoopBuffer_Enqueue 
 * 描述: 
 *            -队列出列操作
 * 输入参数: 
 * 				p_loop_buf-队列指针
 * 				p_buf-入列数据
 *              buf_len-入列数据字节数
 * 输出参数: 
 *				
 * 返回值: 
 * 				操作成功，返回入列的字节数
 *				QUEUE_OP_FAIL-操作失败，参数有误 
 * 				QUEUE_OP_ERR_NO_MEM-操作失败，队列存不下需要存放的数据
 * 作者: 
 * 创建日期:
 * 
 *------------------------
 * 修改人:
 * 修改日期:
 ******************************************************************************/
s32 LoopBuffer_Enqueue(LOOP_BUFFER_DEF *p_loop_buf,const u8 *p_buf, u16 buf_len)
{
    if(p_loop_buf == NULL || p_buf == NULL || buf_len == 0)
    {
        return QUEUE_OP_FAIL;
    }

    if(GetBlankSize(p_loop_buf) < (buf_len + 2))
    {
        return QUEUE_OP_ERR_NO_MEM;
    }

    u16 wr_pos = p_loop_buf->BlankStartPos;

    p_loop_buf->pBuf[wr_pos] =  buf_len >> 8;
    wr_pos = GetNextPos(p_loop_buf, wr_pos);
    p_loop_buf->pBuf[wr_pos] =  buf_len & 0xff;
    wr_pos = GetNextPos(p_loop_buf, wr_pos);

    u16 back_left_len = p_loop_buf->BufSize - wr_pos;
    if(back_left_len > buf_len)
    {
        memcpy(&p_loop_buf->pBuf[wr_pos], p_buf, buf_len);
        wr_pos += buf_len;
    }
    else
    {
        memcpy(&p_loop_buf->pBuf[wr_pos], p_buf, back_left_len);
        memcpy(&p_loop_buf->pBuf[0], p_buf, buf_len - back_left_len);
        wr_pos = buf_len - back_left_len; 
    }
    p_loop_buf->BlankStartPos = wr_pos;

    return buf_len;
}

/******************************************************************************
 * 函数名:	LoopBuffer_Dequeue 
 * 描述: 
 *            -队列出列操作
 * 输入参数: 
 * 				p_loop_buf-队列指针
 * 				buf_len-出列数据存放区大小，如果出列的数据长度大于存放区大小，则只返回
 *                  存放区大小长度的数据。
 * 输出参数: 
 *				buf-出列数据
 * 返回值: 
 * 				操作成功，返回出列的字节数
 *				QUEUE_OP_FAIL-操作失败，参数有误 
 * 				QUEUE_OP_ERR_EMPTY-操作失败，队列已空
 * 作者: 
 * 创建日期:
 * 
 *------------------------
 * 修改人:
 * 修改日期:
 ******************************************************************************/
s32 LoopBuffer_Dequeue(LOOP_BUFFER_DEF *p_loop_buf,u8 *p_buf, u16 buf_len)
{
    if(p_loop_buf == NULL || p_buf == NULL || buf_len == 0)
    {
        return QUEUE_OP_FAIL;
    }

    if(LoopBuffer_IsEmpty(p_loop_buf) == true)
    {
        return QUEUE_OP_ERR_EMPTY;
    }

    u16 rd_pos = p_loop_buf->DatStartPos;
    u16 dat_len = (p_loop_buf->pBuf[rd_pos]) << 8;
    rd_pos = GetNextPos(p_loop_buf, rd_pos);
    dat_len += p_loop_buf->pBuf[rd_pos];
    rd_pos = GetNextPos(p_loop_buf, rd_pos);

    u16 rd_len = dat_len;
    if(rd_len > buf_len)
    {
        rd_len = buf_len;
    }

    u16 back_left_len = p_loop_buf->BufSize - rd_pos;

    if(back_left_len >= rd_len)
    {
        memcpy(p_buf, &p_loop_buf->pBuf[rd_pos], rd_len);
    }
    else
    {
        memcpy(p_buf, &p_loop_buf->pBuf[rd_pos], back_left_len);
        memcpy(p_buf, &p_loop_buf->pBuf[0], rd_len - back_left_len);
    }  

    //更新datstartpos
    back_left_len = p_loop_buf->BufSize - rd_pos;
    if(back_left_len > dat_len)
    {
        rd_pos += dat_len;
    }
    else
    {
        rd_pos = dat_len - back_left_len; 
    }  
    p_loop_buf->DatStartPos = rd_pos;

    return rd_len; 
}