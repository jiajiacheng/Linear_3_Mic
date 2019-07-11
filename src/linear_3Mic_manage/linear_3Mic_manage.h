#ifndef LINEAR_3MIC_MANAGE_H
#define LINEAR_3MIC_MANAGE_H

#include "common_type.h"
#include "stft.h"
#include "ns_exter.h"
#ifdef DEBUG_ON
//#include"file_test.h"
#endif
typedef struct{
	STFT_PARA m_stft_para;
	STFT_VAR m_stft_var;
	MIC_ARRAY m_mic_array;
	TDOA_VAR m_tdoa_var;
	NS_PARA m_ns_para;
	NS_VAR m_ns_var;
	GAIN_PARA m_gain_para;
	GAIN_VAR m_gain_var;
}LINEAR_3MIC_MANAGE;
L3M_ERROR_TYPE NS_process_Init(LINEAR_3MIC_MANAGE * pManage);
L3M_ERROR_TYPE NS_process_fun(S16_T *Xr, S16_T *Xc, S16_T* Xl, S16_T* output, LINEAR_3MIC_MANAGE * p_Manage);
void L3M_MANAGE_DESTORY(LINEAR_3MIC_MANAGE* p_Manage);
S16_T FLOAT_to_U16_T(FLOAT_T* inputbuf, S16_T * outputbuf, S16_T M);
#endif