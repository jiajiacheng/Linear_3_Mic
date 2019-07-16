#ifndef  COMMON_TYPES_H
#define  COMMON_TYPES_H


#define SAMPLATE_48K 48000
#define WAV_INPUT
//#define DEBUG_ON




#ifdef SAMPLATE_48K
#define STFT_WINLEN  256 //[window length,using turkey window]
#define STFT_DATBLKLEN STFT_WINLEN>>1  //[frame shift: max support 16k  20ms frame length]
#define STFT_KBIN  ((STFT_WINLEN>>1)+1) //effective frequency bin number
#endif
#define TDOA_TAU_S 13
#define TDOA_TAU_N 1
//const
#define M_PI       3.14159265358979323846

#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

#define CONST           const
#define VOLATILE        volatile
#define  STATIC         static

#define ERROR_CHECK(val) {if (val != L3M_ERROR_NONE)	return val;}

#define  SAFE_FREE(p)  if(p!=NULL) free(p);p=NULL;



typedef short               S16_T;
typedef unsigned short      U16_T;
typedef int                 S32_T;
typedef unsigned int        U32_T;
typedef long long             S64_T;
typedef unsigned long long    U64_T;
typedef double              FLOAT_T;

typedef struct _complex16
{
	S16_T real;
	S16_T imag;
}complex16;
typedef struct _complex_double
{
	double real;
	double imag;
}complexDouble;

typedef enum{
	L3M_ERROR_NONE           = 0,
	L3M_INVALID_SAMPLERATE = 1,
	L3M_MALLOC_ERR = 2,
	L3M_DAT_ERR = 3,
	//L3M_RFS_ERR = 4,
}L3M_ERROR_TYPE;

//
//typedef  short int   Word16;
//typedef unsigned short UWord16;	
//typedef  long  int   Word32;
//typedef unsigned long UWord32;

//#define MAX_16 (S16_T)0x7fff
//#define MIN_16 (S16_T)0x8000

//
//
//#ifndef SAFE_DELETE
//#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
//#endif
//
//#ifndef SAFE_RELEASE
//#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
//#endif


//struct
//----------------------------------------------------------------------------//
//

//
//typedef struct _complex32
//{
//	S32_T real;
//	S32_T imag;
//}complex32;
//
//typedef struct _complex64
//{
//	S64_T real;
//	S64_T imag;
//}complex64;



/********   error type   *********/
//typedef enum{
//	DAS_ERROR_NONE           = 0,
//	DAS_INVALID_SAMPLERATE   = 1,
//	DAS_MALLOC_ERR			 = 2,
//	DAS_DELAY_ERR           = 3,
//	DAS_RFS_ERR           = 4,
//}DAS_ERROR_TYPE;


//typedef struct{
//	U16_T  LSB;// 0x1 BY Q15 unsign
//	U16_T  RAIL; //0x07fff by q15 unsign
//	U16_T k_shift;
//	U16_T NoChan;
//	U16_T NoActChan;
//	U16_T k_start;
//	U16_T k_stop;
//
//}AHS_GLOBAL;
//
//typedef struct{
//	S16_T normalize_data_flag      ;                // Normalize data to +/- 1.0 range.
//	S16_T shift_k_flag            ;                // Introduce a K scaling in the FFT to emulate shift in DSP  
//	S16_T simulation_on           ;                //When this flag is set to be on, it runs the simualtion data, otherwise runs the real data  
//	S16_T aclip_on                ;                // If this flag is set to be on, then we introduce the non-linearity in the reference
//	S16_T hd_on                   ;               //When it is set to be on, it enables the howling detection
//	S16_T hs_on                   ;               // When HD_on is set, the howling detection result determins if HS_on is set or not; 
//												//otherwise, HS_on is set manually, if it is on, then it enables the HS , otherwise it diables the HS
//}AHS_FLAG;

//complexDouble complexAdd(complexDouble X1,complexDouble X2);
//complexDouble complexMul(complexDouble X1,complexDouble X2);
//complexDouble complexMulConj(complexDouble X1,complexDouble X2);
//float complexAbsSqr(complexDouble X1);
//complexDouble complexSub(complexDouble X1,complexDouble X2);
complexDouble Real_complexMul(float X1,complexDouble X2);



#endif                  //COMMON_TYPES_H//



