#include "loop_buffer.h"
#include <stdio.h>

//��������
const unsigned char test_buf[11] = {0,1,2,3,4,5,6,7,89, 9, 11};

//����������
unsigned char buffer1_buf[100];
//�������ṹ��
LOOP_BUFFER_DEF buffer1;

int main()
{
    //��ʼ�����λ�����
    LoopBuffer_Init(&buffer1, buffer1_buf, sizeof(buffer1_buf));
    while(1)
    {
        char ch = getchar();
        unsigned char key_code = 0;
        switch(ch)
        {   
            case '1': 
            case '2': 
            case '3': 
            case '4': 
            case '5': 
            case '6': 
            case '7': 
            case '8': 
            case '9': 
            {
                //���в���
                int ret = LoopBuffer_Enqueue(&buffer1, test_buf, ch - 0x30);   
                printf("enqueue:%d....\n",  ret);
                if(ret > 0)
                {
                    printf("in ok!!\n");
                }
                else    
                {
                    printf("in fail.......\n");
                }
                break;
            }
            case 'd': 
            {
                //���в���
                unsigned char  out_buf[11];  
                int ret = LoopBuffer_Dequeue(&buffer1, out_buf, 11);   
                printf("dequeque:%d....\n", ret);
                if(ret > 0)
                {
                    printf("out ok!!\n");
                }
                else
                {
                    printf("out fail.....\n");
                }                
                break;
            }      
            case 'c': 
            {
                //printf("queue total = %d....\n",getTotalFrameNum(&buffer1));
                break;
            }                                  
        }

        if(ch == 'b')
        {
            printf("exit..\n");
            break;
        }
    }
    return 0;
}




