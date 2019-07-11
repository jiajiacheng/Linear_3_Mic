#include<stdlib.h>
#include"stft.h"
#include"fft.h"
L3M_ERROR_TYPE L3m_Stft_Init(STFT_PARA* p_stft_para, STFT_VAR* p_stft_var)
{

	//para init
	extern CONST FLOAT_T sqrt_hamming[];
	p_stft_para->winLen = STFT_WINLEN;
	p_stft_para->datBlkLen = STFT_DATBLKLEN;
	p_stft_para->KBin = STFT_KBIN;
	p_stft_para->Win = sqrt_hamming;
	//var init
	p_stft_var->analysisBuf = (FLOAT_T*)calloc( STFT_WINLEN,sizeof(FLOAT_T));
	p_stft_var->synthesisBuf = (FLOAT_T*)calloc(STFT_WINLEN,sizeof(FLOAT_T));
	p_stft_var->ifft_output = (FLOAT_T*)calloc(STFT_WINLEN, sizeof(FLOAT_T));

	if ((NULL == p_stft_var->analysisBuf) ||
		(NULL == p_stft_var->ifft_output)||
		(NULL == p_stft_var->synthesisBuf))
	{
		return L3M_MALLOC_ERR;
	}
	return L3M_ERROR_NONE;
}
L3M_ERROR_TYPE DFTanalysis_fun_2(FLOAT_T * p16I_time, FLOAT_T* pfO_Freq, STFT_PARA* p_stft_para, STFT_VAR* p_stft_var)
{
	S16_T N = p_stft_para->winLen;
	S16_T M = p_stft_para->datBlkLen;
	S16_T K = p_stft_para->KBin;
	CONST FLOAT_T* pWin = NULL;
	FLOAT_T temp[STFT_WINLEN] = {0};
	S16_T i,i_ch,idx=0,num_channel = 1;
	
	pWin = p_stft_para->Win;
	for (i_ch = 0; i_ch < 1; i_ch++)
	{
		//循环buffer算法
		/*
		S16_T* p_idx;
		for (i = 0; i < M;i++)
		{
			idx = idx % 1;
			p_stft_var->analysisBuf[idx*M+i] = p16I_time[i];//new data in
		}
		for (i = 0; i < N; i++)
		{
			temp[i] = p_stft_var->analysisBuf[((idx % 1)*M + i) % N] * pWin[i];//strat from old data 
		}*/
		//传统算法
		for (i = 0; i<(N - M); i++)
		{
			p_stft_var->analysisBuf[i] = p_stft_var->analysisBuf[i + M];
			p_stft_var->analysisBuf[i + M] = p16I_time[i];
		}
		for (i = 0; i<(N - M); i++)
		{
			temp[i] = p_stft_var->analysisBuf[i] * (*pWin++);
		}
		pWin = &(p_stft_para->Win[M - 1]);//points to the last point of the window
		for (i = 0; i<(N - M); i++)
		{
			temp[i + M] = p_stft_var->analysisBuf[i + M] * (*pWin--);
		}
		RealFFT(temp, pfO_Freq, N);
	}
	
	
	return L3M_ERROR_NONE;
}

L3M_ERROR_TYPE DFTsynthesis_fun_2(FLOAT_T* pfI_Freq, FLOAT_T * pfO_time, STFT_PARA* p_stft_para, STFT_VAR* p_stft_var)
{
	S16_T   i;
	S16_T   N = p_stft_para->winLen;
	S16_T   M = p_stft_para->datBlkLen;
	S16_T   K = p_stft_para->KBin;
	CONST FLOAT_T *pWin = NULL;
	FLOAT_T temp[STFT_WINLEN] = { 0 };

	//ifft//
	iRealFFT(pfI_Freq, &p_stft_var->ifft_output[0], N);

	//windowing//
	pWin = p_stft_para->Win;
	for (i = 0; i<(N - M); i++)
	{
		temp[i] = p_stft_var->ifft_output[i] * (*pWin++);
	}

	pWin = &(p_stft_para->Win[M - 1]);//points to the last point of the window
	for (i = 0; i<(N - M); i++)
	{
		temp[i + M] = p_stft_var->ifft_output[i + M] * (*pWin--);
	}

	//Overlap-add and shift synthesis buffer//
	for (i = 0; i<N; i++)
	{
		p_stft_var->synthesisBuf[i] = p_stft_var->synthesisBuf[i] + temp[i];
	}

	for (i = 0; i<M; i++)
	{
		pfO_time[i] = p_stft_var->synthesisBuf[i];
	}

	//Update synthesis buffer//
	for (i = 0; i<(N - M); i++)
	{
		p_stft_var->synthesisBuf[i] = p_stft_var->synthesisBuf[i + M];
		p_stft_var->synthesisBuf[i + M] = 0;
	}
	return L3M_ERROR_NONE;
}
void L3m_Stft_Destory(STFT_VAR* p_stft_var)
{
	SAFE_FREE(p_stft_var->analysisBuf);
	SAFE_FREE(p_stft_var->synthesisBuf);
}


