#ifndef  WAVE_HEAD_H
#define  WAVE_HEAD_H

#include "stdio.h"
#include "string.h"

typedef unsigned char UT_uint1_t;	/* 1-byte unsigned int */
typedef unsigned short int UT_uint2_t;	/* 2-byte unsigned int */
typedef unsigned int UT_uint4_t;/* 4-byte unsigned int */

struct WV_GUID {
	UT_uint2_t wFormatTag;
	UT_uint1_t guidx[14];
};

struct WV_CKpreamb {
	char ckID[4];
	UT_uint4_t ckSize;
};

struct WV_CKfmt {
	char ckID[4];
	UT_uint4_t ckSize;
	UT_uint2_t wFormatTag;		/* Format type */
	UT_uint2_t nChannels;		/* Number of channels */
	UT_uint4_t nSamplesPerSec;	/* Sample rate */
	UT_uint4_t nAvgBytesPerSec;	/* nBlockAlign * nSamplesPerSec */
	UT_uint2_t nBlockAlign;		/* Block size (bytes), multiple of nChannels */
	/* End of common area */
	UT_uint2_t wBitsPerSample;	/* Bits/sample: for WAVE_FORMAT_EXTENSIBLE,
								this is the sample container size */
	/* Start of EXTENSIBLE part */
	UT_uint2_t cbSize;		/* Number of bytes following */
	UT_uint2_t wValidBitsPerSample; /* Actual number of bits in each sample */
	UT_uint4_t dwChannelMask;	/* Channel assignment mask */
	struct WV_GUID SubFormat;
};

struct WV_CKfact {
	char ckID[4];
	UT_uint4_t ckSize;
	UT_uint4_t dwSampleLength;
};

struct WV_CKdata {
	char ckID[4];
	UT_uint4_t ckSize;
};

struct WV_CKRIFF {
	char ckID[4];
	UT_uint4_t ckSize;
	char WAVEID[4];
	struct WV_CKfmt CKfmt;
	struct WV_CKfact CKfact;
	struct WV_CKpreamb CKdata;	/* Not including audio data */
};


/* Machine and data byte order codes */
enum {
	DS_UNDEF	= -1,	/* undefined */
	DS_EB		= 0,	/* big-endian */
	DS_EL		= 1,	/* little-endian */
	DS_NATIVE	= 2,	/* native */
	DS_SWAP       = 3	/* byte-swapped */
};

typedef enum{
	DAS_FORMAT_INVALID =-1,
	DAS_PCM = 0,
	DAS_WAV = 1,
	DAS_FORMAT_NO,
}DAS_FILE_FORMAT;

/* Header read/write definitions */
#define RHEAD_S(fp,string) \
	AFreadHead (fp, (void *)(string), 1, (int) (sizeof (string)), \
	DS_NATIVE)
#define RHEAD_SN(fp,string,N) \
	AFreadHead (fp, (void *) (string), 1, (int) (N), DS_NATIVE)
#define RHEAD_V(fp,value,swap) \
	AFreadHead (fp, (void *) &(value), (int) (sizeof (value)), 1, \
	(int) (swap))
#define RSKIP(fp,nbytes) \
	fseek(fp,nbytes,SEEK_CUR)
#define WHEAD_S(fp,string) \
	AFwriteHead (fp, (const void *) (string), 1, (int) (sizeof (string)), \
	DS_NATIVE)
#define WHEAD_SN(fp,string,N) \
	AFwriteHead (fp, (const void *) (string), 1, (int) (N), DS_NATIVE)
#define WHEAD_P(fp,string) \
	AFwriteHead (fp, (const void *) (string), 1, (int) (strlen (string)), \
	DS_NATIVE)
#define WHEAD_V(fp,value,swap) \
	AFwriteHead (fp, (const void *) &(value), (int) (sizeof (value)), 1, \
	(int) (swap))

#define SAME_CSTR(str,ref) 	(memcmp (str, ref, sizeof (str)) == 0)
#define MCOPY(src,dest)		memcpy ((void *) (dest), \
	(const void *) (src), sizeof (dest))
#define ICEILV(n,m)	(((n) + ((m) - 1)) / (m))	/* int n,m >= 0 */
#define RNDUPV(n,m)	((m) * ICEILV (n, m))		/* Round up */
#define WV_FMT_MINSIZE	16
#define WV_LHMIN	(8 + 4 + 8 + WV_FMT_MINSIZE + 8)
#define ALIGN		2	/* Chunks padded out to a multiple of ALIGN */

#ifdef __cplusplus 
extern "C"
{
#endif

static int AF_rdRIFF_WAVE (FILE *fp, struct WV_CKRIFF *CKRIFF);
int AFreadHead (FILE *fp, void *Buf, int Size, int Nv, int Swapb);
int AFwriteHead (FILE * fp, const void *Buf, int Size, int Nv, int Swapb);
long int FLfileSize (FILE *fp);
static int AF_rdFMT (FILE *fp, struct WV_CKfmt *CKfmt);
static int AF_rdFACT (FILE *fp, struct WV_CKfact *CKfact);
int WVReadHead (FILE *fp, unsigned short *channel, unsigned int *samplerate,unsigned int *ckSize);
void WVWritehead (FILE *fp,UT_uint2_t nChannels,UT_uint4_t nSamplesPerSec, long int Ldata);
DAS_FILE_FORMAT GetFileFormat(char* filename);

#ifdef __cplusplus
}
#endif

#endif