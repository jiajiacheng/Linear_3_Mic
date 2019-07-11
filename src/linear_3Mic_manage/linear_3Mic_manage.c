#include<stdlib.h>
#include"linear_3Mic_manage.h"

L3M_ERROR_TYPE  NS_process_Init(LINEAR_3MIC_MANAGE * pManage)
{
	//初始化各模块
	L3M_ERROR_TYPE val;
	val = L3m_Stft_Init(&(pManage->m_stft_para), &(pManage->m_stft_var));
	ERROR_CHECK(val);
	val = L3m_Mic_Array_Init(&(pManage->m_mic_array));
	ERROR_CHECK(val);
	val = L3m_Tdoa_Init(&(pManage->m_tdoa_var));
	ERROR_CHECK(val);
	val = L3m_Ns_Init(&(pManage->m_ns_para), &(pManage->m_ns_var));
	ERROR_CHECK(val);
	val = L3m_Gain_Init(&(pManage->m_gain_para),&(pManage->m_gain_var));
	ERROR_CHECK(val);
	
	//if (NULL ==( fp_dft = fopen("../test_out/dft.dat", "wb"))){ return L3M_DAT_ERR; }
	return L3M_ERROR_NONE;
}

L3M_ERROR_TYPE  NS_process_fun(S16_T *Xr, S16_T *Xc, S16_T* Xl, S16_T* output, LINEAR_3MIC_MANAGE * p_Manage)
{
#if 1
	S16_T index;
	FLOAT_T div = 1;
	FLOAT_T fl_output[STFT_WINLEN];
	//if (p_Manage->m_flag.normalize_data_flag)
	{
		div = 32768.0;
	}
	/*else
	{
		div = 1.0;
	}*/
	for (index = 0; index < STFT_DATBLKLEN; index++)
	{
		
		p_Manage->m_mic_array.xC_block[index] = (FLOAT_T)(Xc[index] / div);
		
	}
#endif

	//////////////////////////////////////////////////////
	DFTanalysis_fun_2((p_Manage->m_mic_array.xC_block), (p_Manage->m_mic_array.XC), &(p_Manage->m_stft_para), &(p_Manage->m_stft_var));
	
	//noise_estimate_fun
	//noise_supp_fun
#ifdef DEBUG_ON
	File_Test(fp_dft, p_Manage->m_mic_array.XC, STFT_DATBLKLEN);
#endif
	DFTsynthesis_fun_2((p_Manage->m_mic_array.XC), fl_output, &(p_Manage->m_stft_para), &(p_Manage->m_stft_var));
	
	
	
	for (index = 0; index < STFT_DATBLKLEN; index++)
	{
		fl_output[index] = (FLOAT_T)(fl_output[index] * div);
	}

	//#ifdef DEBUG_ON
//	File_Test(fp_dft, fl_output, STFT_DATBLKLEN);
//#endif
	FLOAT_to_U16_T(fl_output, output, (STFT_DATBLKLEN));
	/*for (index = 0; index < STFT_DATBLKLEN; index++)
		output[index] = (S16_T)fl_output[index];
	*/
	return L3M_ERROR_NONE;
}
void L3M_MANAGE_DESTORY(LINEAR_3MIC_MANAGE* p_Manage)
{
	L3m_Stft_Destory(&(p_Manage->m_stft_var));
	L3m_Noise_process_Destory(&(p_Manage->m_mic_array), &(p_Manage->m_ns_var), &(p_Manage->m_gain_var));
}
S16_T FLOAT_to_U16_T(FLOAT_T* inputbuf, S16_T * outputbuf, S16_T M)
{
	S16_T i;
	for (i = 0; i < M; i++)
	{
		if (inputbuf[i] >= 0)
		{
			if (inputbuf[i] <= 32767)
			{
				outputbuf[i] = (S16_T)(inputbuf[i] + 0.5);
			}
			else
			{
				outputbuf[i] = 32767;
			}

		}
		else
		{
			if (inputbuf[i] >= -32768)
			{
				outputbuf[i] = (S16_T)(inputbuf[i] - 0.5);
			}
			else
			{
				outputbuf[i] = -32768;
			}
		}
	}
	return 	L3M_ERROR_NONE;
}