#include "ns_exter.h"
#include<math.h>

L3M_ERROR_TYPE noise_supp_fun(MIC_ARRAY * p_mic_array, STFT_PARA * p_stft_para,
	NS_VAR* p_ns_var, GAIN_PARA* p_gain_para, GAIN_VAR* p_gain_var)
{
	S16_T i;
	FLOAT_T Power_XC[STFT_KBIN], noise_pow[STFT_KBIN];
	FLOAT_T real, imag;

	for (i = 1; i < (STFT_KBIN - 1); i++)
	{
		real = p_mic_array->XC[2 * i];
		imag = p_mic_array->XC[2 * i + 1];
		Power_XC[i] = (real*real + imag*imag);
		real = p_ns_var->noise_spec[i].real;
		imag = p_ns_var->noise_spec[i].imag;
		noise_pow[i] = (real*real + imag*imag);
		//p_gain_var->postSNR[i] =
		//p_gain_var->Gain[i] = 
	}
	Power_XC[0] = p_mic_array->XC[0] * p_mic_array->XC[0];
	Power_XC[STFT_KBIN - 1] = p_mic_array->XC[1] * p_mic_array->XC[1];
	noise_pow[0] = p_ns_var->noise_spec[0].real * p_ns_var->noise_spec[0].real;
	noise_pow[STFT_KBIN - 1] = p_ns_var->noise_spec[0].imag * p_ns_var->noise_spec[0].imag;
	for (i = 0; i < STFT_KBIN; i++)
	{
		p_gain_var->postSNR[i] = Power_XC[i] / noise_pow[i];
		p_gain_var->postSNR[i] = max(p_gain_var->postSNR[i], 1e-10);
		p_gain_var->Gain[i] = 1 - 1 / (sqrt(p_gain_var->postSNR[i]));
		p_gain_var->Gain[i] = max(min(p_gain_var->Gain[i], 1), p_gain_para->minGain);


	}
	for (i = 1; i < (STFT_KBIN - 1); i++)
	{
		p_ns_var->X_supp[2 * i] = p_gain_var->Gain[i] * p_mic_array->XC[2 * i];
		p_ns_var->X_supp[2 * i + 1] = p_gain_var->Gain[i] * p_mic_array->XC[2 * i + 1];
	}
	p_ns_var->X_supp[0] = p_gain_var->Gain[0] * p_mic_array->XC[0];
	p_ns_var->X_supp[STFT_WINLEN - 1] = p_gain_var->Gain[STFT_KBIN - 1] * p_mic_array->XC[1];

	return L3M_ERROR_NONE;
}