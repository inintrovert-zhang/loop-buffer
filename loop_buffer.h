/**
  ******************************************************************************
  * ��Ȩ���У� 
  * �ļ���:   loop_buffer.h
  * ���ߣ�    zwy
  * ��������: 2023.05.13
  * ������ 	���ĵ��ṩ�˶�����֡���λ������Ķ��в���������������֡���Ȳ��̶���ÿ�����е�
  *      ���ݣ��ڳ��в�����������������һ������ͨ�����ݻ��档
  * 
  * ע�⣺����֡�ĳ�����Ϣ��������Ϣ�������ͬһ��������������֡������Ϣռ��2���ֽ�
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
    u8 *pBuf;           //ָ�򻺴���
    u16 BufSize;        //����������
    u16 DatStartPos;    //������ʼλ��
    u16 BlankStartPos;  //�հ�����ʼλ��
}LOOP_BUFFER_DEF;


enum
{
	QUEUE_OP_SUCCESS = 0,          //�����ɹ�
	QUEUE_OP_FAIL = (-1),          //����ʧ��
	QUEUE_OP_ERR_NO_MEM = (-2),    //��������
	QUEUE_OP_ERR_EMPTY = (-3),     //�����ѿ�
};

u16 GetBlankSize(LOOP_BUFFER_DEF *p_loop_buf);
s32 LoopBuffer_Init(LOOP_BUFFER_DEF *p_loop_buf, u8 *p_buf, u16 buf_len);
s32 LoopBuffer_Enqueue(LOOP_BUFFER_DEF *p_loop_buf,const u8 *p_buf, u16 buf_len);
s32 LoopBuffer_Dequeue(LOOP_BUFFER_DEF *p_loop_buf,u8 *p_buf, u16 buf_len);


#endif