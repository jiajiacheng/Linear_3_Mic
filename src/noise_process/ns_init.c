#include<stdlib.h>
#include"ns_exter.h"


L3M_ERROR_TYPE L3m_Mic_Array_Init(MIC_ARRAY* p_mic_array)
{
	p_mic_array->fs = SAMPLATE_48K; // mic_array includes the information of mic array and the input signals
	p_mic_array->mic_distance = 0.1; // the distance between center mic and right mic
	p_mic_array->xR_block = (FLOAT_T*)calloc(STFT_DATBLKLEN, sizeof(FLOAT_T));
	p_mic_array->xC_block = (FLOAT_T*)calloc(STFT_DATBLKLEN, sizeof(FLOAT_T));
	p_mic_array->xL_block = (FLOAT_T*)calloc(STFT_DATBLKLEN, sizeof(FLOAT_T));
	p_mic_array->XC = (FLOAT_T*)calloc(STFT_WINLEN, sizeof(FLOAT_T));

	if ((NULL == p_mic_array->xR_block) ||
		(NULL == p_mic_array->xC_block) ||
		(NULL == p_mic_array->xL_block)||
		(NULL == p_mic_array->XC))
	{
		return L3M_MALLOC_ERR;
	}
	return L3M_ERROR_NONE;
}
L3M_ERROR_TYPE L3m_Tdoa_Init(TDOA_VAR* p_tdoa_var)
{
	p_tdoa_var->tau_s = 13;
	p_tdoa_var->tau_n = 1;
	p_tdoa_var->tau1 = p_tdoa_var->tau_n - p_tdoa_var->tau_s;
	p_tdoa_var->tau2 = p_tdoa_var->tau1>>1;
	return L3M_ERROR_NONE;
}
L3M_ERROR_TYPE L3m_Ns_Init(NS_PARA* p_ns_para, NS_VAR* p_ns_var)
{
	p_ns_para->epsilon1 = 0.6;
	p_ns_para->epsilon2 = 0.1;
	p_ns_var->xR_buff = (FLOAT_T*)calloc(STFT_DATBLKLEN*2 , sizeof(FLOAT_T));
	p_ns_var->xC_buff = (FLOAT_T*)calloc(STFT_DATBLKLEN * 2, sizeof(FLOAT_T));
	p_ns_var->xL_buff = (FLOAT_T*)calloc(STFT_DATBLKLEN * 2, sizeof(FLOAT_T));
	p_ns_var->noise_spec = (FLOAT_T*)calloc(STFT_KBIN , sizeof(FLOAT_T));
	//p_ns_var->X_supp = (FLOAT_T*)calloc(STFT_KBIN, sizeof(FLOAT_T));
	//p_ns_var->x_supp = (FLOAT_T*)calloc(STFT_DATBLKLEN, sizeof(FLOAT_T));
	if ((NULL == p_ns_var->xR_buff) ||
		(NULL == p_ns_var->xC_buff)||
		(NULL == p_ns_var->xL_buff) ||
		(NULL == p_ns_var->noise_spec) 
		)
	{
		return L3M_MALLOC_ERR;
	}
	return L3M_ERROR_NONE;
}
L3M_ERROR_TYPE L3m_Gain_Init(GAIN_PARA* p_gain_para, GAIN_VAR* p_gain_var)
{
	p_gain_para->alpha = 1;
	p_gain_para->beta = 0.001;
	p_gain_para->minGain = 0.05;
	p_gain_var->Gain = (FLOAT_T*)calloc(STFT_KBIN, sizeof(FLOAT_T));
	p_gain_var->postSNR = (FLOAT_T*)calloc(STFT_KBIN, sizeof(FLOAT_T));
	if ((NULL == p_gain_var->Gain)||
		(NULL == p_gain_var->postSNR)
		)
	{
		return L3M_MALLOC_ERR;
	}
	return L3M_ERROR_NONE;
}

void L3m_Noise_process_Destory(MIC_ARRAY* p_mic_array,
	NS_VAR* p_ns_var,
	GAIN_VAR* p_gain_var)
{
	SAFE_FREE(p_mic_array->xR_block);
	SAFE_FREE(p_mic_array->xC_block);
	SAFE_FREE(p_mic_array->xL_block);
	SAFE_FREE(p_mic_array->XC);
	SAFE_FREE(p_ns_var->xR_buff);
	SAFE_FREE(p_ns_var->xC_buff);
	SAFE_FREE(p_ns_var->xL_buff);
	SAFE_FREE(p_ns_var->noise_spec);
	SAFE_FREE(p_gain_var->Gain);
	SAFE_FREE(p_gain_var->postSNR);

}