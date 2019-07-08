#include <stdlib.h>
#include <stdio.h>   
#include <string.h>    

#include "main.h" 
#include "common_type.h"
#include "wave_file.h"  
#include "linear_3Mic_manage.h"


// 准备自动化生成一个名字
//void L3M_parseInput(int argc, char* argv[])
//{
//	if (argc != 5)
//	{
//		printf("wrong input parameters!\n");
//		goto  L3M_MAINLOOP_EXIT_PATH;
//	}
//	if ((fp_in_R = fopen(argv[1], "rb")) == NULL)
//	{
//		printf("Can't open input1 file: %s\n", argv[1]);
//		goto  L3M_MAINLOOP_EXIT_PATH;
//	}
//	if ((fp_in_C = fopen(argv[2], "rb")) == NULL)
//	{
//		printf("Can't open input2 file: %s\n", argv[2]);
//		goto  L3M_MAINLOOP_EXIT_PATH;
//	}
//	if ((fp_in_L = fopen(argv[3], "rb")) == NULL)
//	{
//		printf("Can't open input3 file: %s\n", argv[3]);
//		goto  L3M_MAINLOOP_EXIT_PATH;
//	}
//	if ((fp_out = fopen(argv[4], "wb")) == NULL)
//	{
//		printf("Can't open output file: %s\n", argv[4]);
//		goto  L3M_MAINLOOP_EXIT_PATH;
//	}
//}

int main(int argc, char* argv[])
{      

	L3M_ERROR_TYPE val;
	int   val_len = 0,idx=0 ;
	unsigned short channel1 = 0, channel2 = 0, channel3 = 0;
	unsigned int ckSize1 = 0, ckSize2 = 0, ckSize3 = 0;
	unsigned int samplerate1 = 0, samplerate2 = 0, samplerate3 = 0;
	int i, frmNo , bufferlen;
	S16_T In_xR_block[STFT_DATBLKLEN], In_xC_block[STFT_DATBLKLEN], In_xL_block[STFT_DATBLKLEN], Out_x_block_supp[STFT_DATBLKLEN];
	LINEAR_3MIC_MANAGE Linear_3Mic_manage;
	LINEAR_3MIC_MANAGE* p_Linear_3Mic_manage = &Linear_3Mic_manage;
	//S16_T In_xL_block[STFT_DATBLKLEN];
/*	int simulation_on = 0, ext_fac = 1;
	char mode[3];j, cnt,*/
	
	
	FILE *fp_in_R = NULL, *fp_in_C = NULL, *fp_in_L = NULL, *fp_out = NULL, *fp_dat_out = NULL;;
	/*open file*/
	if (argc != 5)
	{
		printf("wrong input parameters!\n");
		goto  L3M_MAINLOOP_EXIT_PATH;
	}	
	if((fp_in_R = fopen(argv[1],"rb"))==NULL)
	{
		printf("Can't open input1 file: %s\n",argv[1]);
		goto  L3M_MAINLOOP_EXIT_PATH;
	}
	if ((fp_in_C = fopen(argv[2], "rb")) == NULL)
	{
		printf("Can't open input2 file: %s\n", argv[2]);
		goto  L3M_MAINLOOP_EXIT_PATH;
	}
	if ((fp_in_L = fopen(argv[3], "rb")) == NULL)
	{
		printf("Can't open input3 file: %s\n", argv[3]);
		goto  L3M_MAINLOOP_EXIT_PATH;
	}
	if ((fp_out = fopen(argv[4],"wb"))==NULL)
	{
		printf("Can't open output file: %s\n",argv[4]);
		goto  L3M_MAINLOOP_EXIT_PATH;
	}
	/*if ((fp_dat_out = fopen(argv[5],"wb"))==NULL)
	{
		printf("Can't open dat file: %s\n",argv[5]);
		goto  L3M_MAINLOOP_EXIT_PATH;
	}*/
	/*wav read*/
	val_len = WVReadHead(fp_in_R,&channel1,&samplerate1,&ckSize1);
	if (val_len<0)
	{
		goto  L3M_MAINLOOP_EXIT_PATH;
	}
	val_len = WVReadHead(fp_in_C, &channel2, &samplerate2, &ckSize2);
	if (val_len<0)
	{
		goto  L3M_MAINLOOP_EXIT_PATH;
	}	val_len = WVReadHead(fp_in_L, &channel3, &samplerate3, &ckSize3);
	if (val_len<0)
	{
		goto  L3M_MAINLOOP_EXIT_PATH;
	}
	if ((samplerate1 != samplerate2) || (samplerate1 != samplerate3))
	{
		printf("Three input have different samplerate\n");
		goto  L3M_MAINLOOP_EXIT_PATH;
	}
	if (48000 != samplerate1)
	{
		goto  L3M_MAINLOOP_EXIT_PATH;
	}
	ckSize1 = min(ckSize1, ckSize2);
	ckSize1 = min(ckSize1, ckSize3);
				

	//初始化
	val = NS_process_Init(p_Linear_3Mic_manage);
	if (L3M_ERROR_NONE != val)
	{
		printf("error No %d is occured",val);
	
		goto L3M_MAINLOOP_EXIT_PATH;
	}

	bufferlen = STFT_DATBLKLEN;
	frmNo = ckSize1 / bufferlen;

	for (i = 0; i < frmNo; i++)
	{
		if ((fread(In_xR_block, sizeof(S16_T), bufferlen, fp_in_R) == bufferlen) &&
			(fread(In_xC_block, sizeof(S16_T), bufferlen, fp_in_C) == bufferlen) &&
			(fread(In_xL_block, sizeof(S16_T), bufferlen, fp_in_L) == bufferlen))
		{
			
			//主处理
			NS_process_fun(In_xR_block, In_xC_block, In_xL_block, Out_x_block_supp, p_Linear_3Mic_manage);
			fwrite(Out_x_block_supp, sizeof(S16_T), bufferlen, fp_out);
			printf("frame Number is %d\r", (i + 1));
			frmNo++;
			NULL;
		}
	}
	//if (samplerate==8000)
	//{
	//	bufferlen = 32;
	//	strcpy(mode, "nb");
	//	val = AHS_Manage_Init(&handle, &init_parameters, samplerate);
	//}
	//else if (samplerate==16000)
	//{
	//	bufferlen = 64;
	//	strcpy(mode, "wb");
	//	val = AHS_Manage_Init(&handle, &init_parameters, samplerate);
	//}
	//
	//if (val != 0)
	//{
	//	goto L3M_MAINLOOP_EXIT_PATH;
	//}
	//{
	//	char wav_header[WV_LHMIN];

	//	if (fp_out!=0)
	//	{
	//		if (1!=fwrite(wav_header,WV_LHMIN,1,fp_out))
	//		{
	//			goto L3M_MAINLOOP_EXIT_PATH;
	//		}
	//	}		
	//} 	
	//for (j=0; j<bufferlen; j++)
	//{
	//	inputbuf[j] = 0;
	//	refbuf[j] = 0;
	//	outputbuf[j] = 0;
	//}

 //   
	//cnt = (int)ckSize/bufferlen ;
	//for ( i = 0 ; i< cnt;i++)
	//{
	//	if (fread(inputbuf,sizeof(S16_T),bufferlen,fp_in)==bufferlen)
	//	{
	//		
	//		AHS_Manage_Process( inputbuf, refbuf, outputbuf, handle ,bufferlen);		
	//			

	//		printf("frame Number is %d\r",frmNo);

	//		for(idx = 0; idx < bufferlen; idx++)
	//		{
	//			fprintf(fp_dat_out, "%d\n", inputbuf[idx]);
	//		}

	//		fwrite(outputbuf,sizeof(S16_T),bufferlen,fp_out);
	//		if(frmNo ==240)   
	//		{
	//			idx = 0 ;
	//		} 
	//		frmNo++;
	//	}
	//}  
	
	L3M_MANAGE_DESTORY(p_Linear_3Mic_manage);
	
	
	L3M_MAINLOOP_EXIT_PATH:

	/*close file pointers*/
	if (fp_in_R)
		fclose(fp_in_R);
	if (fp_in_C)
		fclose(fp_in_C);
	if (fp_in_L)
		fclose(fp_in_L);
	if (fp_out) 
	{
		#ifdef WAV_INPUT
				long int filelen = 0;
				filelen = FLfileSize(fp_out);
				fseek(fp_out,0,SEEK_SET);
				WVWritehead(fp_out,1,samplerate1,filelen-WV_LHMIN);
		#endif
		fclose(fp_out);
	}
#ifdef DEBUG_ON
//	fclose(fp_dat_out);
	fclose(fp_dft);
#endif
	getchar();
}                 




