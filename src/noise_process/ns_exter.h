#ifndef NS_EXTER_H
#define NS_EXTER_H

#include"common_type.h"
#include"ns_inter.h"
#include"stft.h"


L3M_ERROR_TYPE noise_estimate_fun(MIC_ARRAY * p_mic_array, STFT_PARA * p_stft_para, TDOA_VAR* p_tdoa_var, NS_PARA* p_ns_para, NS_VAR* p_ns_var);
L3M_ERROR_TYPE noise_supp_fun(MIC_ARRAY * p_mic_array, STFT_PARA * p_stft_para, NS_VAR* p_ns_var, GAIN_PARA* p_gain_para, GAIN_VAR* p_gain_var);

#endif