/**
  ******************************************************************************
  * ��Ȩ���У� 
  * �ļ���:   loop_buffer.c 
  * ���ߣ�    zwy
  * ��������: 2023.05.13
  * ������ 	���ĵ��ṩ�˶�����֡���λ������Ķ��в���������������֡���Ȳ��̶���ÿ�����е�
  *      ���ݣ��ڳ��в�����������������һ������ͨ�����ݻ��档
  * 
  * ע�⣺����֡�ĳ�����Ϣ��������Ϣ�������ͬһ��������������֡������Ϣռ��2���ֽ�
  ******************************************************************************
  */

#include "loop_buffer.h"
#include <string.h>
#include <stdbool.h>


/******************************************************************************
 * ������:	GetNextPos 
 * ����: 
 *            --��������ֵ������һλ
 * �������: 
 * 				p_loop_buf-����ָ��
 * 				now_pos-��ǰ����
 * �������: 
 *				
 * ����ֵ: 
 * 				�¸�����ֵ
 *				 
 * 				
 * ����: 
 * ��������:
 * 
 *------------------------
 * �޸���:
 * �޸�����:
 ******************************************************************************/
u16 GetNextPos(LOOP_BUFFER_DEF *p_loop_buf, u16 now_pos)
{
    return now_pos == (p_loop_buf->BufSize - 1) ? 0 : (now_pos + 1);
}

/******************************************************************************
 * ������:	LoopBuffer_IsFull 
 * ����: 
 *            -�ж϶�����
 * �������: 
 * 				p_loop_buf-����ָ��
 * 				
 * �������: 
 *				
 * ����ֵ: 
 * 				true-������,false-δ��
 *				
 * 				
 * ����: 
 * ��������:
 * 
 *------------------------
 * �޸���:
 * �޸�����:
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
 * ������:	LoopBuffer_IsEmpty 
 * ����: 
 *            -�ж϶��п�
 * �������: 
 * 				p_loop_buf-����ָ��
 * 				
 * �������: 
 *				buf-��������
 * ����ֵ: 
 * 				
 *				true-���п�,false-δ��
 * 				
 * ����: 
 * ��������:
 * 
 *------------------------
 * �޸���:
 * �޸�����:
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
 * ������:	GetBlankSize 
 * ����: 
 *            -���г��в���
 * �������: 
 * 				p_loop_buf-����ָ��
 * 				
 * �������: 
 *				
 * ����ֵ: 
 * 				���ض��пռ�ʣ���ֽ���
 *				
 * 				
 * ����: 
 * ��������:
 * 
 *------------------------
 * �޸���:
 * �޸�����:
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
 * ������:	LoopBuffer_Init 
 * ����: 
 *            -���г�ʼ��
 * �������: 
 * 				p_loop_buf-����ָ��
 * 				p_buf-������ָ��
 *              buf_len-��������С
 * �������: 
 *				
 * ����ֵ: 
 * 				QUEUE_OP_SUCCESS-�����ɹ�
 *				QUEUE_OP_FAIL-����ʧ�ܣ��������� 
 * 				
 * ����: 
 * ��������:
 * 
 *------------------------
 * �޸���:
 * �޸�����:
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
 * ������:	LoopBuffer_Enqueue 
 * ����: 
 *            -���г��в���
 * �������: 
 * 				p_loop_buf-����ָ��
 * 				p_buf-��������
 *              buf_len-���������ֽ���
 * �������: 
 *				
 * ����ֵ: 
 * 				�����ɹ����������е��ֽ���
 *				QUEUE_OP_FAIL-����ʧ�ܣ��������� 
 * 				QUEUE_OP_ERR_NO_MEM-����ʧ�ܣ����д治����Ҫ��ŵ�����
 * ����: 
 * ��������:
 * 
 *------------------------
 * �޸���:
 * �޸�����:
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
 * ������:	LoopBuffer_Dequeue 
 * ����: 
 *            -���г��в���
 * �������: 
 * 				p_loop_buf-����ָ��
 * 				buf_len-�������ݴ������С��������е����ݳ��ȴ��ڴ������С����ֻ����
 *                  �������С���ȵ����ݡ�
 * �������: 
 *				buf-��������
 * ����ֵ: 
 * 				�����ɹ������س��е��ֽ���
 *				QUEUE_OP_FAIL-����ʧ�ܣ��������� 
 * 				QUEUE_OP_ERR_EMPTY-����ʧ�ܣ������ѿ�
 * ����: 
 * ��������:
 * 
 *------------------------
 * �޸���:
 * �޸�����:
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

    //����datstartpos
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