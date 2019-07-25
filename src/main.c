#include <stdlib.h>
#include <stdio.h>   


#include "main.h" 
#include "common_type.h"
#include "linear_3Mic_manage.h"
#include "wave_file.h"  
#ifdef DEBUG_ON
#include "file.h"
#endif

int main(int argc, char* argv[])
{      
	FILE *fp_r = NULL, *fp_c = NULL, *fp_l = NULL, *fp_out = NULL;
	
	short channel1 = 0, channel2 = 0, channel3 = 0;
	int i,bufferlen, file_len =0;
	int val_len1 = 0, val_len2 = 0, val_len3 = 0;
	int ckSize1 = 0, ckSize2 = 0, ckSize3 = 0;
	int samplerate1, samplerate2, samplerate3;
	LINEAR_3MIC_MANAGE  Manage;
	LINEAR_3MIC_MANAGE* p_Manage = &Manage;

	S16_T xR_block[STFT_DATBLKLEN], xC_block[STFT_DATBLKLEN], xL_block[STFT_DATBLKLEN];
	S16_T x_supp[STFT_DATBLKLEN];
	if (argc != 5)
	{
		printf("wrong input parameters!\n");
		goto  L3M_MAINLOOP_EXIT_PATH;
	}	
	if ((fp_r = fopen(argv[1], "rb")) == NULL)
	{
		printf("Can't open input file: %s\n",argv[1]);
		goto  L3M_MAINLOOP_EXIT_PATH;
	}
	if ((fp_c = fopen(argv[2], "rb")) == NULL)
	{
		printf("Can't open input file: %s\n",argv[2]);
		goto  L3M_MAINLOOP_EXIT_PATH;
	}
	if ((fp_l = fopen(argv[3], "rb")) == NULL)
	{
		printf("Can't open input file: %s\n",argv[3]);
		goto  L3M_MAINLOOP_EXIT_PATH;
	}
	if ((fp_out = fopen(argv[4], "wb")) == NULL)
	{
		printf("Can't open output file: %s\n", argv[3]);
		goto  L3M_MAINLOOP_EXIT_PATH;
	}

	val_len1 = WVReadHead(fp_r, &channel1, &samplerate1, &ckSize1);
	val_len2 = WVReadHead(fp_c, &channel2, &samplerate2, &ckSize2);
	val_len3 = WVReadHead(fp_l, &channel3, &samplerate3, &ckSize3);

	if ((val_len1<0) ||(val_len2<0) ||(val_len3<0))
	{
		goto  L3M_MAINLOOP_EXIT_PATH;
	}
	if ((samplerate1 != 48000) || (samplerate1 != samplerate2) || (samplerate1 != samplerate3))
	{
		goto  L3M_MAINLOOP_EXIT_PATH;
	}
	ckSize1 = min(ckSize1, ckSize2);
	ckSize1 = min(ckSize1, ckSize3);
	val_len1 = NS_process_Init(p_Manage);
	bufferlen = STFT_DATBLKLEN;
	if (fp_out != 0)
	{
		char wav_header[WV_LHMIN];
		if (1 != fwrite(wav_header, WV_LHMIN, 1, fp_out))
		{
			goto L3M_MAINLOOP_EXIT_PATH;
		}
	}
	for (i = 0; i <(ckSize1 / bufferlen); i++)
	{
		if ((fread(xR_block, sizeof(S16_T), bufferlen, fp_r) == bufferlen) &&
			(fread(xC_block, sizeof(S16_T), bufferlen, fp_c) == bufferlen) &&
			(fread(xL_block, sizeof(S16_T), bufferlen, fp_l) == bufferlen))
		{
			NS_process_fun(xR_block, xC_block, xL_block, x_supp, p_Manage);
			fwrite(x_supp, sizeof(S16_T), bufferlen, fp_out);
			printf("frame Number is %d\r", (i+1));
		//	j


			
		}
	}




L3M_MAINLOOP_EXIT_PATH:


	if (fp_out)
	{
		file_len = FLfileSize(fp_out);
		fseek(fp_out, 0, SEEK_SET);
		WVWritehead(fp_out, 1, samplerate1, file_len-WV_LHMIN);//
		fclose(fp_out);
	}
	if (fp_r != NULL) fclose(fp_r);
	if (fp_c != NULL) fclose(fp_c);
	if (fp_l != NULL) fclose(fp_l);
#ifdef DEBUG_ON

	File_close(fp_dft);
	File_close(fp_glr);
	File_close(fp_gcr);
#endif
	


}                 




