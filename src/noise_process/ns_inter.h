#ifndef NS_INTER_H
#define NS_INTER_H
#include"common_type.h"
//#include"ns_exter.h"

typedef struct
{
	S16_T fs;
	FLOAT_T mic_distance;
	FLOAT_T* xR_block;
	FLOAT_T* xC_block;
	FLOAT_T* xL_block;
	FLOAT_T* XC;
	S16_T xr_index;
	S16_T xc_index;
	S16_T xl_index;
}MIC_ARRAY;

typedef struct
{
	S16_T tau_s;
	S16_T tau_n;
	S16_T tau1;
	S16_T tau2;
}TDOA_VAR;

typedef struct
{
	FLOAT_T epsilon1;
	FLOAT_T epsilon2;
}NS_PARA;

typedef struct
{
	FLOAT_T* xR_buff;
	FLOAT_T* xC_buff;
	FLOAT_T* xL_buff;
	//FLOAT_T* noise_spec;
	complexDouble* noise_spec;
	FLOAT_T* X_supp;
	FLOAT_T* x_supp;
}NS_VAR;

typedef struct
{
	FLOAT_T alpha;
	FLOAT_T beta;
	FLOAT_T minGain;
}GAIN_PARA;

typedef struct
{
	FLOAT_T* Gain;
	FLOAT_T* postSNR;
}GAIN_VAR;
L3M_ERROR_TYPE L3m_Mic_Array_Init(MIC_ARRAY* p_mic_array);
L3M_ERROR_TYPE L3m_Tdoa_Init(TDOA_VAR* p_tdoa_var);
L3M_ERROR_TYPE L3m_Ns_Init(NS_PARA* p_ns_para, NS_VAR* p_ns_var);
L3M_ERROR_TYPE L3m_Gain_Init(GAIN_PARA* p_gain_para, GAIN_VAR* p_gain_var);
void L3m_Noise_process_Destory(MIC_ARRAY* p_mic_array, NS_VAR* p_ns_var, GAIN_VAR* p_gain_var);

L3M_ERROR_TYPE gcr_fun(NS_VAR* p_ns_var, TDOA_VAR* p_tdoa_var, FLOAT_T* gcr, FLOAT_T* sigC1, FLOAT_T* sigC2, FLOAT_T* sigR1, FLOAT_T* sigR2);
L3M_ERROR_TYPE glr_fun(NS_VAR* p_ns_var, TDOA_VAR* p_tdoa_var, FLOAT_T* glr, FLOAT_T* sigL1, FLOAT_T* sigL2, FLOAT_T* sigR1, FLOAT_T* sigR2);
L3M_ERROR_TYPE shift_sig_fun(FLOAT_T* x, S16_T tau, FLOAT_T* x_shift, S16_T len);
#endif