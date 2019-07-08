#ifndef _SUM_H_ 
#define _SUM_H_ 
 
 /*ping-pong structure and buffer defination*/
//#define FRAME_SIZE  64  /*4ms in 16K */  
//#define int16_t     short
//#define uint16_t    unsigned short
//
//
//typedef struct{
//	volatile int16_t  sample_buff[FRAME_SIZE];//a pingpong buffer for frame-processing
//	volatile uint16_t  buffer_flag;            //0:loading data; 1:processing;
//	volatile int16_t  output_buff[FRAME_SIZE];
//}pingpong_struct;   
//
//
//extern volatile uint16_t sample_count;         // count frame size
//extern volatile uint16_t buffer_switch;        //0:buffer 1;  1-buffer 2;
//extern volatile uint16_t buffer_process;       //0:buffer 1;  1-buffer 2;
//extern pingpong_struct  pingpong[2];  //ping pong buffer for loading frame data    
//
//
////#define  GET_DATA_METHOD     1
//
//void flash_write(unsigned int data, volatile int *pAddr);   
//unsigned int flash_read(volatile int *pAddr);
//void Erase_Chip(void);

#endif