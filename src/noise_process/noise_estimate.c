#include<string.h>
#include<math.h>
#include "ns_exter.h"
#include"fft.h"


L3M_ERROR_TYPE noise_estimate_fun(MIC_ARRAY * p_mic_array, STFT_PARA * p_stft_para, TDOA_VAR* p_tdoa_var, NS_PARA* p_ns_para, NS_VAR* p_ns_var)
{
	S16_T i = 0;
	//FLOAT_T *p_xr_buff, *p_xc_buff, *p_xl_buff;, index
	FLOAT_T glr[STFT_WINLEN], gcr[STFT_WINLEN];
	FLOAT_T sigL1[STFT_WINLEN], sigL2[STFT_WINLEN], sigR1[STFT_WINLEN], sigR2[STFT_WINLEN], sigC1[STFT_WINLEN], sigC2[STFT_WINLEN];
	FLOAT_T f_per_point, w, sin_w_t_2, sin_w_halft_2;
	complexDouble e_neg_jw_halft;
	memset(sigL1, 0, sizeof(FLOAT_T)*STFT_WINLEN);
	memset(sigL2, 0, sizeof(FLOAT_T)*STFT_WINLEN);
	memset(sigR1, 0, sizeof(FLOAT_T)*STFT_WINLEN);
	memset(sigR2, 0, sizeof(FLOAT_T)*STFT_WINLEN);
	memset(sigC1, 0, sizeof(FLOAT_T)*STFT_WINLEN);
	memset(sigC2, 0, sizeof(FLOAT_T)*STFT_WINLEN);


	/*p_mic_array->xr_index = (p_mic_array->xr_index + 1) % 2;
	index = p_mic_array->xr_index;
	memcpy(&(p_ns_var->xR_buff[ index * STFT_DATBLKLEN]), p_mic_array->xR_block, sizeof(FLOAT_T)*STFT_DATBLKLEN);

	p_mic_array->xc_index = (p_mic_array->xc_index + 1) % 2;
	index = p_mic_array->xc_index;
	memcpy(&(p_ns_var->xC_buff[index * STFT_DATBLKLEN]), p_mic_array->xC_block, sizeof(FLOAT_T)*STFT_DATBLKLEN);

	p_mic_array->xl_index = (p_mic_array->xl_index + 1) % 2;
	index = p_mic_array->xl_index;
	memcpy(&(p_ns_var->xL_buff[index * STFT_DATBLKLEN]), p_mic_array->xL_block, sizeof(FLOAT_T)*STFT_DATBLKLEN);*/
	memcpy(&(p_ns_var->xR_buff[0]), &(p_ns_var->xR_buff[STFT_DATBLKLEN]), sizeof(FLOAT_T)*STFT_DATBLKLEN);
	memcpy(&(p_ns_var->xC_buff[0]), &(p_ns_var->xC_buff[STFT_DATBLKLEN]), sizeof(FLOAT_T)*STFT_DATBLKLEN);
	memcpy(&(p_ns_var->xL_buff[0]), &(p_ns_var->xL_buff[STFT_DATBLKLEN]), sizeof(FLOAT_T)*STFT_DATBLKLEN);
	memcpy(&(p_ns_var->xR_buff[STFT_DATBLKLEN]), p_mic_array->xR_block, sizeof(FLOAT_T)*STFT_DATBLKLEN);
	memcpy(&(p_ns_var->xC_buff[STFT_DATBLKLEN]), p_mic_array->xC_block, sizeof(FLOAT_T)*STFT_DATBLKLEN);
	memcpy(&(p_ns_var->xL_buff[STFT_DATBLKLEN]), p_mic_array->xL_block, sizeof(FLOAT_T)*STFT_DATBLKLEN);

	glr_fun(p_ns_var, p_tdoa_var, glr, sigL1, sigL2, sigR1, sigR2);
	gcr_fun(p_ns_var, p_tdoa_var, gcr, sigC1, sigC2, sigR1, sigR2);
	//RealFFT(temp, pfO_Freq, N);
	RealFFT(glr, glr, STFT_WINLEN);
	RealFFT(gcr, gcr, STFT_WINLEN);
	f_per_point = p_mic_array->fs / p_stft_para->winLen;
	for (i = 0; i < p_stft_para->datBlkLen; i++)
	{
		w = 2 * M_PI * f_per_point*(i - 1);
		sin_w_t_2 = sin(w * p_tdoa_var->tau1 / p_mic_array->fs);
		sin_w_t_2 = sin_w_t_2 * sin_w_t_2;
		sin_w_halft_2 = sin(w * p_tdoa_var->tau2 / p_mic_array->fs);
		sin_w_halft_2 = sin_w_halft_2 * sin_w_halft_2;
		if (sin_w_t_2 > p_ns_para->epsilon1)
		{
			p_ns_var->noise_spec[i].real = (glr[i] / sin_w_t_2);
			p_ns_var->noise_spec[i].imag = (glr[i+1] / sin_w_t_2);
		}
		else if ((sin_w_t_2 <= p_ns_para->epsilon1) && (sin_w_halft_2 > p_ns_para->epsilon2))
		{
			e_neg_jw_halft.real = cos( -w * p_tdoa_var->tau2 / p_mic_array->fs);
			e_neg_jw_halft.imag = - sin(w * p_tdoa_var->tau2 / p_mic_array->fs);
			p_ns_var->noise_spec[i].real = (gcr[i] * e_neg_jw_halft.real / sin_w_t_2);
			p_ns_var->noise_spec[i].imag = (gcr[i + 1] * e_neg_jw_halft .imag / sin_w_t_2);
		}
		else
		{
			p_ns_var->noise_spec[i].real = (glr[i] / (p_ns_para->epsilon2 * p_ns_para->epsilon2));
			p_ns_var->noise_spec[i].imag = (glr[i + 1] / (p_ns_para->epsilon2 * p_ns_para->epsilon2));
		}
	}
	/*w = 2 * M_PI * f_per_point*(p_stft_para->datBlkLen - 1);
	sin_w_t_2 = sin(w * p_tdoa_var->tau1 / p_mic_array->fs);
	sin_w_t_2 = sin_w_t_2 * sin_w_t_2;
	sin_w_halft_2 = sin(w * p_tdoa_var->tau2 / p_mic_array->fs);
	sin_w_halft_2 = sin_w_halft_2 * sin_w_halft_2;
	if (sin_w_t_2 > p_ns_para->epsilon1)
	{
		p_ns_var->noise_spec[0].imag = (glr[i + 1] / sin_w_t_2);
	}
	else if ((sin_w_t_2 <= p_ns_para->epsilon1) && (sin_w_halft_2 > p_ns_para->epsilon2))
	{
		e_neg_jw_halft.real = cos(-w * p_tdoa_var->tau2 / p_mic_array->fs);
		e_neg_jw_halft.imag = -sin(w * p_tdoa_var->tau2 / p_mic_array->fs);
		p_ns_var->noise_spec[i].real = (gcr[i] * e_neg_jw_halft.real / sin_w_t_2);
		p_ns_var->noise_spec[i].imag = (gcr[i + 1] * e_neg_jw_halft.imag / sin_w_t_2);
	}
	else
	{
		p_ns_var->noise_spec[i].real = (glr[i] / (p_ns_para->epsilon2 * p_ns_para->epsilon2));
		p_ns_var->noise_spec[i].imag = (glr[i + 1] / (p_ns_para->epsilon2 * p_ns_para->epsilon2));
	}*/
	return L3M_ERROR_NONE;
}
L3M_ERROR_TYPE glr_fun(NS_VAR* p_ns_var, TDOA_VAR* p_tdoa_var, FLOAT_T* glr,
	FLOAT_T* sigL1, FLOAT_T* sigL2, FLOAT_T* sigR1, FLOAT_T* sigR2)
{
	S16_T i=0;
	FLOAT_T xL[STFT_WINLEN], xR[STFT_WINLEN];
	memcpy(xL, p_ns_var->xL_buff, sizeof(FLOAT_T)*STFT_WINLEN);
	memcpy(xR, p_ns_var->xR_buff, sizeof(FLOAT_T)*STFT_WINLEN);
	shift_sig_fun(xL, (p_tdoa_var->tau_s + p_tdoa_var->tau1), sigL1, STFT_WINLEN);
	shift_sig_fun(xL, (p_tdoa_var->tau_s - p_tdoa_var->tau1), sigL2, STFT_WINLEN);
	shift_sig_fun(xR, (-p_tdoa_var->tau_s + p_tdoa_var->tau1), sigR1, STFT_WINLEN);
	shift_sig_fun(xR, (-p_tdoa_var->tau_s - p_tdoa_var->tau1), sigR2, STFT_WINLEN);
	for (i = 0; i < STFT_WINLEN; i++)
	{
		glr[i] = 0.25*((sigL1[i] - sigL2[i]) - (sigR1[i] - sigR2[i]));
	}

	return L3M_ERROR_NONE;
}
L3M_ERROR_TYPE gcr_fun(NS_VAR* p_ns_var, TDOA_VAR* p_tdoa_var, FLOAT_T* gcr,
	FLOAT_T* sigC1, FLOAT_T* sigC2, FLOAT_T* sigR1, FLOAT_T* sigR2)
{
	
	S16_T i = 0;
	FLOAT_T xC[STFT_WINLEN], xR[STFT_WINLEN];
	memcpy(xC, p_ns_var->xC_buff, sizeof(FLOAT_T)*STFT_WINLEN);
	memcpy(xR, p_ns_var->xR_buff, sizeof(FLOAT_T)*STFT_WINLEN);
	shift_sig_fun(xC, ( p_tdoa_var->tau2), sigC1, STFT_WINLEN);
	shift_sig_fun(xC, (- p_tdoa_var->tau2), sigC2, STFT_WINLEN);
	shift_sig_fun(xR, (-p_tdoa_var->tau_s + p_tdoa_var->tau2), sigR1, STFT_WINLEN);
	shift_sig_fun(xR, (-p_tdoa_var->tau_s - p_tdoa_var->tau2), sigR2, STFT_WINLEN);
	for (i = 0; i < STFT_WINLEN; i++)
	{
		gcr[i] = 0.25*((sigC1[i] - sigC2[i]) - (sigR1[i] - sigR2[i]));
	}
	return L3M_ERROR_NONE;
}

L3M_ERROR_TYPE shift_sig_fun(FLOAT_T* x, S16_T tau,FLOAT_T* x_shift,S16_T len)
{
	S16_T i;
	memset(x_shift, 0, (sizeof(FLOAT_T)*len));
	if (tau > 0)
	{
		for (i = 0; i < (len - tau); i++)
		{
			x_shift[i] = x[tau + i];
		}
	}
	else if (tau < 0)
	{
		tau = -tau;
		for (i = 0; i < (len - tau); i++)
		{
			x_shift[i + tau] = x[i];
		}
	}
	else
	{
		for (i = 0; i < len; i++)
		{
			x_shift[i] = x[i];
		}
	}
	return L3M_ERROR_NONE;
}