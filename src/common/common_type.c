#include <math.h>
#include "common_type.h"


/*  complex x1+x2  */
complexDouble complexAdd(complexDouble X1,complexDouble X2)
{
	complexDouble result;
	result.real = X1.real + X2.real;
	result.imag = X1.imag + X2.imag;
	return result;

}
/*  complex x1-x2  */
complexDouble complexSub(complexDouble X1,complexDouble X2)
{
	complexDouble result;
	result.real = X1.real - X2.real;
	result.imag = X1.imag - X2.imag;
	return result;

}

/*  complex x1*x2  */
complexDouble complexMul(complexDouble X1,complexDouble X2)
{
	complexDouble result;
	result.real = X1.real * X2.real - X1.imag * X2.imag;
	result.imag = X1.real * X2.imag + X1.imag * X2.real;
	return result;

}
complexDouble Real_complexMul(float X1,complexDouble X2)
{
	complexDouble result;
	result.real = X1 * X2.real ;
	result.imag = X1 * X2.imag ;
	return result;

}

/*  complex x1*conj(x2)  */
complexDouble complexMulConj(complexDouble X1,complexDouble X2)
{
	complexDouble result;
	result.real = X1.real * X2.real + X1.imag * X2.imag;
	result.imag = - X1.real * X2.imag + X1.imag * X2.real;
	return result;

}

/*  complex abs */

FLOAT_T complexAbsSqr(complexDouble X)  
{
	FLOAT_T result;
	result = X.real * X.real + X.imag * X.imag;
	return result;
}

