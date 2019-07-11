#include "fft.h"


const complex16 twidder_512[129] =
{
	{32767,0},{32766,-402},{32758,-804},{32746,-1206},{32729,-1608},{32706,-2009},{32679,-2411},{32647,-2811},
	{32610,-3212},{32568,-3612},{32522,-4011},{32470,-4410},{32413,-4808},{32352,-5205},{32286,-5602},{32214,-5998},
	{32138,-6393},{32058,-6787},{31972,-7180},{31881,-7571},{31786,-7962},{31686,-8351},{31581,-8740},{31471,-9127},
	{31357,-9512},{31238,-9896},{31114,-10279},{30986,-10660},{30853,-11039},{30715,-11417},{30572,-11793},{30425,-12167},
	{30274,-12540},{30118,-12910},{29957,-13279},{29792,-13646},{29622,-14010},{29448,-14373},{29269,-14733},{29086,-15091},
	{28899,-15447},{28707,-15800},{28511,-16151},{28311,-16500},{28106,-16846},{27897,-17190},{27684,-17531},{27467,-17869},
	{27246,-18205},{27020,-18538},{26791,-18868},{26557,-19195},{26320,-19520},{26078,-19841},{25833,-20160},{25583,-20475},
	{25330,-20788},{25073,-21097},{24812,-21403},{24548,-21706},{24279,-22006},{24008,-22302},{23732,-22595},{23453,-22884},
	{23170,-23170}

};

/*___________________________________________________________________________
|                                                                           |
|   Function Name : norm_l                                                  |
|                                                                           |
|   Purpose :                                                               |
|                                                                           |
|   Produces the number of left shifts needed to normalize the 32 bit varia-|
|   ble L_var1 for positive values on the interval with minimum of          |
|   1073741824 and maximum of 2147483647, and for negative values on the in-|
|   terval with minimum of -2147483648 and maximum of -1073741824; in order |
|   to normalize the result, the following operation must be done :         |
|                   norm_L_var1 = L_shl(L_var1,norm_l(L_var1)).             |
|                                                                           |
|   Complexity weight : 30                                                  |
|                                                                           |
|   Inputs :                                                                |
|                                                                           |
|    L_var1                                                                 |
|             32 bit long signed integer (S32_T) whose value falls in the  |
|             range : 0x8000 0000 <= var1 <= 0x7fff ffff.                   |
|                                                                           |
|   Outputs :                                                               |
|                                                                           |
|    none                                                                   |
|                                                                           |
|   Return Value :                                                          |
|                                                                           |
|    var_out                                                                |
|             16 bit short signed integer (S16_T) whose value falls in the |
|             range : 0x0000 0000 <= var_out <= 0x0000 001f.                |
|___________________________________________________________________________|
*/

S16_T norm_l (S32_T L_var1)
{
	S16_T var_out;

	if (L_var1 == 0)
	{
		var_out = 0;
	}
	else
	{
		if (L_var1 == (S32_T) 0xffffffffL)
		{
			var_out = 31;
		}
		else
		{
			if (L_var1 < 0)
			{
				L_var1 = ~L_var1;
			}
			for (var_out = 0; L_var1 < (S32_T) 0x40000000L; var_out++)
			{
				L_var1 <<= 1;
			}
		}
	}

	return (var_out);
}


S32_T  fft_BitReordering(complexDouble* data, S32_T Len)
{

	S32_T n=0,p=0,m=0;
	complexDouble tmp;

	for (p=0; p<(Len-1); p++)
	{
		if (n>p) 
		{
			tmp = data[p];
			data[p] = data[n];
			data[n] = tmp;

		}
		m = Len>>1;
		while (m>=2 && n>=m) 
		{
			n -= m;
			m >>= 1;
		}

		n += m;

	}

	return 0;
}

S32_T fft_RtoC_Convert( complexDouble *x ,complexDouble *y , S32_T len )
{
	S32_T i ;
	complexDouble X1 , X2;
	complex16 twidder;
	S32_T tw_ind;

	S32_T factor;

	if(256 == len)
	{
		factor = 1;
	}else
	{
		factor = 2;
	}

	y[0].real = (x[0].real+ x[0].imag);
	y[0].imag = (x[0].real- x[0].imag);


	for(i = 1; i <= len/2 ; i++)
	{
		X1.real  = (x[i].real + x[len - i].real)/2;
		X1.imag  = (x[i].imag - x[len - i].imag)/2;

		X2.real  = (x[i].imag       + x[len - i].imag)/2;
		X2.imag  = (x[len - i].real - x[i].real)/2;

		tw_ind = i*factor;
		twidder = load_twidder(tw_ind);	
		butterfly_fft( &X1 , &X2 , &twidder);

		y[i] = X1;
		X2.imag = - (X2.imag);
		y[len - i] = X2;
	}

	return 0;

}

S32_T ifft_RtoC_Convert( complexDouble *x ,complexDouble *y , S32_T len )
{
	S32_T i ;
	complexDouble X1 , X2;
	complex16 twidder;
	S32_T tw_ind;
	S32_T factor;

	if(256 == len)
	{
		factor = 1;
	}else
	{
		factor = 2;
	}

	y[0].real   = (x[0].real + x[0].imag)/2;
	y[0].imag   = (x[0].real - x[0].imag)/2;


	for(i = 1; i <= len/2 ; i++)
	{
		X1.real  = (x[i].real + x[len - i].real)/2;
		X1.imag  = (x[i].imag - x[len - i].imag)/2;

		X2.real  = (-x[len - i].imag -  x[i].imag)/2;
		X2.imag  = (x[i].real - x[len - i].real)/2;

		tw_ind = i*factor;
		twidder = load_twidder(tw_ind);	
		butterfly_fft( &X1 , &X2 , &twidder);

		X2.imag = -(X2.imag);
		y[i] = X1;
		y[len - i] = X2;
	}


	return 0;

}

S32_T fft( complexDouble* time ,  S32_T len )
{
	S32_T step = 1;
	//	S32_T group_step = 1;
	S16_T m = 0;
	S32_T group_num = 1;
	S32_T butterfly_num = len>>1;

	S32_T i , j , k;
	S32_T jj = 2;

	S32_T tw_ind =0;
	S32_T tw_offset = 0;
	complexDouble *x , *y , *xm;
	complex16 twidder;

	S32_T factor;

	if(256 == len)
	{
		factor = 2;
	}else
	{
		factor = 4;
	}

	m = norm_l(len);
	m = 30-m;

	tw_offset = ((S32_T)1)<<(m - 1);

	for(i = 0 ; i < m ; i++)
	{
		xm = time ;
		tw_ind = 0;

		for(j = 0 ;j < group_num ; j++)
		{

			x = xm ;
			xm++;

			tw_ind = tw_offset*factor;
			twidder = load_twidder(tw_ind*j);

			for(k = 0 ; k < butterfly_num ; k++)
			{   
				y = x + step;
				butterfly_fft( x , y , &twidder);			
				x = x + jj;
			}

		}

		step = step<< 1;
		jj = jj<<1;
		group_num = group_num<<1;
		butterfly_num = butterfly_num>>1;
		tw_offset = tw_offset >> 1;

	}		  

	return 0;

}

//frequency format:0 , N/2,  1.real ,1.imag,   2.real, 2.imag......,	N/2-1.REAL, N/2-1.IMAG
S32_T RealFFT(FLOAT_T * time, FLOAT_T * freq, S32_T len)
{
	complexDouble freq_t[256];
	S32_T i;
	complexDouble * pCtime = (complexDouble *)time;
	//FLOAT_T temp;
	S32_T len_s;
	len_s = len>>1;

	fft_BitReordering((complexDouble *)time , len_s);

	for(i = 0 ; i < len_s ; i++)
	{
		freq_t[i].real = (FLOAT_T)pCtime[i].real;
		freq_t[i].imag = (FLOAT_T)pCtime[i].imag;
	}

	fft(freq_t,  len_s);

	fft_RtoC_Convert( freq_t, (complexDouble *)freq,  len_s);


	//Array the order :  FROM 0 , N/2,  1.real ,1.imag,   2.real, 2.imag......,	N/2-1.REAL, N/2-1.IMAG
	//                   TO:  0 , 1.real ,1.imag,   2.real, 2.imag......,	N/2-1.REAL, N/2-1.IMAG ,N/2,
	/*temp = freq[1];	
	for(i = 1 ; i < (len-1) ; i ++ )
		freq[i] = freq[i+1];
	freq[len-1] = temp;*/

	return 0;

}


S32_T iRealFFT(FLOAT_T * freq , FLOAT_T * time , S32_T len)
{
	complexDouble freq_t[256];//MAX support 256-REAL FFT
	S32_T i ;
	complexDouble * pCtime = (complexDouble *)time;
	//FLOAT_T temp;

	/*temp = freq[len-1];     

	for(i = (len-1) ; i >= 2; i -- )
		freq[i] = freq[i-1];
	freq[1] = temp;*/


	for(i = 3 ; i < len ; i+=2)
		freq[i] = -(freq[i]);

	len = len>>1;

	ifft_RtoC_Convert( (complexDouble *)freq , freq_t,  len);

	fft_BitReordering( freq_t , len);

	fft( freq_t, len );

	for(i = 0 ; i < len ; i++)
	{	
		pCtime[i].imag = (freq_t[i].imag)/len;
		pCtime[i].real = (freq_t[i].real)/len;
	}

	return 0;
}


S32_T butterfly_fft(complexDouble * x , complexDouble * y , complex16 *twidder)
{
	FLOAT_T temp1 , temp2 , temp3 ,temp4;
	//complex16  multpily
	temp1 = y->real*((FLOAT_T)(twidder->real)/32767.0);
	temp2 = y->imag*((FLOAT_T)(twidder->imag)/32767.0);

	temp3 = y->real * ((FLOAT_T)(twidder->imag)/32767.0);
	temp4 = y->imag * ((FLOAT_T)(twidder->real)/32767.0 );

	temp1 = (temp1 - temp2);
	temp3 = (temp3 + temp4); 


	temp2 = x->real;// +0x8000;
	temp4 = x->imag;// +0x8000;

	// complex16 add 
	x->real = temp2 + temp1;//round(L_shr(L_add(temp2 ,temp1), (S32_T)fft_type));
	y->real = temp2 - temp1;//round(L_shr(L_sub(temp2 ,temp1), (S32_T)fft_type));

	x->imag = temp4 + temp3;//round(L_shr(L_add(temp4 ,temp3), (S32_T)fft_type));
	y->imag = temp4 - temp3;//round(L_shr(L_sub(temp4 ,temp3), (S32_T)fft_type));

	return 0;

}


complex16 load_twidder(S32_T index)
{

	complex16 twidder ;
	S16_T temp;

	if(index <= 64)
	{
		twidder = *(((complex16 *) twidder_512) + index);

	}else if(index < 128)
	{
		index  = 128 - index ;
		twidder = *(((complex16 *) twidder_512) + index);
		temp    = twidder.real;
		twidder.real  = -twidder.imag;
		twidder.imag = -temp;

	}else if(index <= 192)
	{
		index  = index - 128 ;
		twidder = *(((complex16 *) twidder_512) + index);
		temp    = twidder.real;
		twidder.real  = twidder.imag;
		twidder.imag = -temp;

	}else 
	{
		index  = 256 - index ;
		twidder = *(((complex16 *) twidder_512) + index);
		twidder.real  = -twidder.real;
		twidder.imag = twidder.imag;
	}

	return twidder;
}



