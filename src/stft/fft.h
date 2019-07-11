#ifndef FFT_H
#define FFT_H

#include "common_type.h"



#ifdef __cplusplus 
extern "C"
{
#endif


S32_T  fft_BitReordering(complexDouble* data, S32_T Len);
S32_T  fft_RtoC_Convert( complexDouble *x ,complexDouble *y , S32_T len );
S32_T  ifft_RtoC_Convert( complexDouble *x ,complexDouble *y , S32_T len );
S32_T  fft( complexDouble* time ,  S32_T len );
S32_T  RealFFT(FLOAT_T * time, FLOAT_T * freq, S32_T len);
S32_T  iRealFFT(FLOAT_T * freq, FLOAT_T * time, S32_T len);
S32_T  butterfly_fft(complexDouble * x , complexDouble * y , complex16 *twidder);
complex16 load_twidder(S32_T index);

#ifdef __cplusplus
}
#endif


#endif

