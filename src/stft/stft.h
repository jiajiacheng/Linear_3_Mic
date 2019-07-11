#ifndef STFT_H
#define STFT_H

#include"common_type.h"
typedef struct //_stft_para
{
	S16_T winLen;
	S16_T datBlkLen;
	S16_T KBin;
	CONST FLOAT_T* Win;
}STFT_PARA;
typedef struct
{
	FLOAT_T* analysisBuf;
	FLOAT_T* synthesisBuf;
	FLOAT_T* ifft_output;
	//S16_T nFrame;

}STFT_VAR;
L3M_ERROR_TYPE L3m_Stft_Init(STFT_PARA* p_stft_para, STFT_VAR* p_stft_var);
void L3m_Stft_Destory(STFT_VAR* p_stft_var);

L3M_ERROR_TYPE DFTanalysis_fun_2(FLOAT_T * p16I_time, FLOAT_T* pfO_Freq, STFT_PARA* p_stft_para, STFT_VAR* p_stft_var);
L3M_ERROR_TYPE DFTsynthesis_fun_2(FLOAT_T* pfI_Freq, FLOAT_T * pfO_time, STFT_PARA* p_stft_para, STFT_VAR* p_stft_var);
#endif