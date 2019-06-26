#include <stdlib.h>
#include <stdio.h>   
#include <math.h>   
#include <string.h>    

#include "main.h" 
//#include "common_type.h"
//#include "das_manage.h"
#include <time.h>  
#include "wave_file.h"  


S16_T refbuf[FRAME_SIZE];
S16_T inputbuf[FRAME_SIZE], outputbuf[FRAME_SIZE];
S16_T bufferlen;
int frmNo = 1;
int samplerate = 16000 ; 
void *handle = NULL;




int main(int argc, char* argv[])
{      

	int   val, val_len = 0,idx=0 ;
	short channel = 0;
	int i,j,ckSize = 0,cnt;

	int simulation_on = 0, ext_fac = 1;
	char mode[3];
	INIT_PARA init_parameters;

	FILE *fp_in = NULL,*fp_out = NULL,*fp_dat_out= NULL;;
	
	if (argc != 4)
	{
		printf("wrong input parameters!\n");
		goto  HS_MAINLOOP_EXIT_PATH;
	}	
	if((fp_in = fopen(argv[1],"rb"))==NULL)
	{
		printf("Can't open input file: %s\n",argv[1]);
		goto  HS_MAINLOOP_EXIT_PATH;
	}
	if ((fp_out = fopen(argv[2],"wb"))==NULL)
	{
		printf("Can't open output file: %s\n",argv[2]);
		goto  HS_MAINLOOP_EXIT_PATH;
	}
	if ((fp_dat_out = fopen(argv[3],"wb"))==NULL)
	{
		printf("Can't open output file: %s\n",argv[3]);
		goto  HS_MAINLOOP_EXIT_PATH;
	}

	val_len = WVReadHead(fp_in,&channel,&samplerate,&ckSize);
	if (val_len<0)
	{
		goto  HS_MAINLOOP_EXIT_PATH;
	}
	
	if (samplerate!=8000 && samplerate!=16000)
	{
		goto  HS_MAINLOOP_EXIT_PATH;
	}
	 				
	init_parameters.td_network = 1638; //Q14  0.1f [0,2)
	init_parameters.aAclip = 32767;    //Q15  1.0f [0,1)non-linear limiter
	init_parameters.mu = 16384;         //Q14  1.0f [0,2)
	init_parameters.rfs_alpha = 512;
	init_parameters.rfs_lamtaD = 32256;
	init_parameters.Aclip_on = 0;
	init_parameters.HS_on = 0;

	if (samplerate==8000)
	{
		bufferlen = 32;
		strcpy(mode, "nb");
		val = AHS_Manage_Init(&handle, &init_parameters, samplerate);
	}
	else if (samplerate==16000)
	{
		bufferlen = 64;
		strcpy(mode, "wb");
		val = AHS_Manage_Init(&handle, &init_parameters, samplerate);
	}
	
	if (val != 0)
	{
		goto HS_MAINLOOP_EXIT_PATH;
	}
	{
		char wav_header[WV_LHMIN];

		if (fp_out!=0)
		{
			if (1!=fwrite(wav_header,WV_LHMIN,1,fp_out))
			{
				goto HS_MAINLOOP_EXIT_PATH;
			}
		}		
	} 	
	for (j=0; j<bufferlen; j++)
	{
		inputbuf[j] = 0;
		refbuf[j] = 0;
		outputbuf[j] = 0;
	}

    
	cnt = (int)ckSize/bufferlen ;
	for ( i = 0 ; i< cnt;i++)
	{
		if (fread(inputbuf,sizeof(S16_T),bufferlen,fp_in)==bufferlen)
		{
			
			AHS_Manage_Process( inputbuf, refbuf, outputbuf, handle ,bufferlen);		
				

			printf("frame Number is %d\r",frmNo);

			for(idx = 0; idx < bufferlen; idx++)
			{
				fprintf(fp_dat_out, "%d\n", inputbuf[idx]);
			}

			fwrite(outputbuf,sizeof(S16_T),bufferlen,fp_out);
			if(frmNo ==240)   
			{
				idx = 0 ;
			} 
			frmNo++;
		}
	}  
	

	
	//AHS_Manage_Destory(handle);
	
	HS_MAINLOOP_EXIT_PATH:
	
	/*close file pointers*/
	if (fp_in)
	{
		fclose(fp_in);
		//fclose(fp_dat_out);
	}

	if (fp_out) 
	{
		#ifdef WAV_INPUT
				long int filelen = 0;

				filelen = FLfileSize(fp_out);
				fseek(fp_out,0,SEEK_SET);
				WVWritehead(fp_out,1,samplerate,filelen-WV_LHMIN);
		#endif
		fclose(fp_out);
	}



}                 




