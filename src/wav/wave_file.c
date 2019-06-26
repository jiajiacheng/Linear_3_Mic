#include "wave_file.h"
#include <ctype.h>
/************************************************************************
WAVE file:
Offset Length Type    Contents
0     4    char   "RIFF" file identifier
4     4    int    Chunk length
8     4    char   "WAVE" file identifier
12     4    char   "fmt " chunk identifier
16     4    int    Chunk length
20     2    int      Audio data type
22     2    int      Number of interleaved channels
24     4    int      Sample rate
28     4    int      Average bytes/sec
32     2    int      Block align
34     2    int      Data word length (bits)
36     2    int      Extra info size
C     4    int    "fact" chunk identifier (only for non-PCM data)
C+4     4    int    Chunk length
C+8     4    int      Number of samples (per channel)
...   ...    ...    ...
D     4    char   "data" chunk identifier
D+4     4    int    Chunk length
D+8    ...   ...      Audio data

if function runs successfully return headerlen, or return -1;
************************************************************************/
int WVReadHead (FILE *fp, unsigned short *channel, unsigned int *samplerate,unsigned int *ckSize )
{

	int offs, LRIFF, len, Dstart, EoD;

	struct WV_CKRIFF CKRIFF;
	struct WV_CKpreamb CkHead;

	memset(&CKRIFF,0,sizeof(CKRIFF));
	memset(&CkHead,0,sizeof(CkHead));

	/* Check the file magic for a RIFF/WAVE file */
	if (AF_rdRIFF_WAVE (fp, &CKRIFF)!=0)
	{
		return -1;
	}

	offs = 12L;	/* Positioned after RIFF/WAVE preamble */
	LRIFF = CKRIFF.ckSize + 8;

	Dstart = 0L;
	EoD = 0L;

	while (offs < LRIFF-8) {	/* Leave 8 bytes for the chunk preamble */

		/* Read the chunk preamble */
		offs += RHEAD_S (fp, CkHead.ckID);

		/* fmt chunk */
		if (SAME_CSTR (CkHead.ckID, "fmt ")) 
		{
			len = AF_rdFMT (fp, &CKRIFF.CKfmt);
			if (len==0)
			{
				return -1;
			}
			
			offs += len;
		}

		/* fact chunk */
		else if (SAME_CSTR (CkHead.ckID, "fact")) 
		{
			len = AF_rdFACT (fp, &CKRIFF.CKfact);
			if (len==0)
			{
				return -1;
			}

			offs += len;
		}

		/* data chunk */
		else if (SAME_CSTR (CkHead.ckID, "data")) {
			offs += RHEAD_V (fp, CKRIFF.CKdata.ckSize, DS_EL);

			Dstart = offs;
			EoD = RNDUPV (Dstart + CKRIFF.CKdata.ckSize, ALIGN);
			if (EoD >= LRIFF ) 
			{
				break;
			}
			else 
			{
				break;//return -1;
			}
			
		}
		/* Miscellaneous chunks */
		else {
			offs += RHEAD_V (fp, CkHead.ckSize, DS_EL);
			offs += RSKIP (fp, RNDUPV (CkHead.ckSize, ALIGN));
		}
	}
	

	/* Position at the start of data */
	if (fseek(fp, Dstart, SEEK_SET)!=0)
	{
		return -1;
	}
	*ckSize = CKRIFF.CKdata.ckSize/2;//因为输入数据是short型所以才可以除以2
	*channel = CKRIFF.CKfmt.nChannels;
	*samplerate = CKRIFF.CKfmt.nSamplesPerSec;

	return Dstart;
}

static int AF_rdRIFF_WAVE (FILE *fp, struct WV_CKRIFF *CKRIFF)
{
	long int Lfile, LRIFF;

	RHEAD_S (fp, CKRIFF->ckID);
	if (! SAME_CSTR (CKRIFF->ckID, "RIFF")) 
	{
		printf("ERROR: Reading wave header\n");
		return -1;
	}

	RHEAD_V (fp, CKRIFF->ckSize, 1);
	LRIFF = CKRIFF->ckSize + 8;
	if (LRIFF < WV_LHMIN) {
		printf("ERROR: Reading wave header\n");
		return -1;
	}

	{
		Lfile = FLfileSize (fp);
		if (LRIFF > Lfile) 
		{
			CKRIFF->ckSize = Lfile - 8;
			printf("warning:conflicted file len\n");
		}
	}

	RHEAD_S (fp, CKRIFF->WAVEID);
	if (! SAME_CSTR (CKRIFF->WAVEID, "WAVE")) 
	{
		printf("ERROR: Reading wave header\n");
		return -1;
	}

	return 0;
}

int AFreadHead (FILE *fp, void *Buf, int Size, int Nv, int Swapb)
{
	int n;

	/* Read the data in file byte order */
	n = fread (Buf, Size, Nv, fp);

	/* Error messages */
	if (n < Nv) 
	{
		return 0;
	}

	return (Size * n);
}

int AFwriteHead (FILE * fp, const void *Buf, int Size, int Nv, int Swapb)

{
	int n;

	/* Non-swapped data is written directly from the input buffer */
	n = fwrite (Buf, Size, Nv, fp);

	if (n < Nv) 
	{
		return 0;
	}

	return (Size * n);
}

long int FLfileSize (FILE *fp)
{
	long int pos;
	long int endpos;

	/*
	The ANSI C standard does not guarantee that this method of determining
	the file size will work on all systems.  It works on Unix systems and
	probably a lot of other systems.  The alternative is to use stat, but that
	is probably even less portable.
	*/
	pos = ftell (fp);
	if (pos == -1L)
		return 0;

	if (fseek (fp, 0L, SEEK_END) != 0)
		return 0;

	endpos = ftell (fp);
	fseek (fp, pos, SEEK_SET);

	return endpos;
}

static int AF_rdFMT (FILE *fp, struct WV_CKfmt *CKfmt)
{
	int offs, NB;

	offs = RHEAD_V (fp, CKfmt->ckSize, DS_EL);
	if (CKfmt->ckSize < WV_FMT_MINSIZE) {
		return 0;
	}

	offs += RHEAD_V (fp, CKfmt->wFormatTag, DS_EL);
	offs += RHEAD_V (fp, CKfmt->nChannels, DS_EL);
	offs += RHEAD_V (fp, CKfmt->nSamplesPerSec, DS_EL);
	offs += RHEAD_V (fp, CKfmt->nAvgBytesPerSec, DS_EL);
	offs += RHEAD_V (fp, CKfmt->nBlockAlign, DS_EL);
	offs += RHEAD_V (fp, CKfmt->wBitsPerSample, DS_EL);

	NB = ((int) CKfmt->ckSize + 4) - offs;
	if (NB >= 24) {
		offs += RHEAD_V (fp, CKfmt->cbSize, DS_EL);
		if (CKfmt->cbSize >= 22) {
			offs += RHEAD_V (fp, CKfmt->wValidBitsPerSample, DS_EL);
			offs += RHEAD_V (fp, CKfmt->dwChannelMask, DS_EL);
			offs += RHEAD_V (fp, CKfmt->SubFormat.wFormatTag, DS_EL);
			offs += RHEAD_S (fp, CKfmt->SubFormat.guidx);
		}
		else
			CKfmt->cbSize = 0;
	}

	/* Skip over any extra data at the end of the fmt chunk */
	offs += RSKIP (fp, RNDUPV (CKfmt->ckSize + 4, ALIGN) - offs);

	return offs;
}

static int AF_rdFACT (FILE *fp, struct WV_CKfact *CKfact)
{
	int offs;

	offs = RHEAD_V (fp, CKfact->ckSize, DS_EL);
	if (CKfact->ckSize < 4) {
		return 0;
	}
	offs += RHEAD_V (fp, CKfact->dwSampleLength, DS_EL);
	offs += RSKIP (fp, RNDUPV (CKfact->ckSize + 4, ALIGN) - offs);

	return offs;
}

void WVWritehead (FILE *fp,UT_uint2_t nChannels,UT_uint4_t nSamplesPerSec, long int Ldata)
{
	//Ldata:data length of the data in byte
	long int size;
	struct WV_CKRIFF CKRIFF;

	memset(&CKRIFF,sizeof(CKRIFF),0);
	/* RIFF chunk */
	MCOPY ("RIFF", CKRIFF.ckID);
	/* defer filling in the chunk size */ 
	MCOPY ("WAVE", CKRIFF.WAVEID);

	/* fmt chunk */
	//AF_setFMT (&CKRIFF.CKfmt, AFw);
	{
		MCOPY ("fmt ", CKRIFF.CKfmt.ckID);
		
		CKRIFF.CKfmt.ckSize = sizeof (CKRIFF.CKfmt.wFormatTag) + sizeof (CKRIFF.CKfmt.nChannels)
		+ sizeof (CKRIFF.CKfmt.nSamplesPerSec) + sizeof (CKRIFF.CKfmt.nAvgBytesPerSec)
		+ sizeof (CKRIFF.CKfmt.nBlockAlign) + sizeof (CKRIFF.CKfmt.wBitsPerSample);

		CKRIFF.CKfmt.nChannels = (UT_uint2_t) nChannels;
		CKRIFF.CKfmt.nSamplesPerSec = (UT_uint4_t) nSamplesPerSec;	
		CKRIFF.CKfmt.wBitsPerSample = (UT_uint2_t) 16;

		CKRIFF.CKfmt.nBlockAlign = (UT_uint2_t) (CKRIFF.CKfmt.nChannels*CKRIFF.CKfmt.wBitsPerSample/8);
		CKRIFF.CKfmt.nAvgBytesPerSec = (UT_uint4_t) (CKRIFF.CKfmt.nSamplesPerSec * CKRIFF.CKfmt.nBlockAlign);
		
		CKRIFF.CKfmt.wFormatTag = 1;//WAVE_FORMAT_PCM;
		CKRIFF.CKfmt.cbSize = 0;
	}

	/* data chunk */
	MCOPY ("data", CKRIFF.CKdata.ckID);
	CKRIFF.CKdata.ckSize = (UT_uint4_t) Ldata;

	/* Fill in the RIFF chunk size */
	size = 4 + 8 + RNDUPV(CKRIFF.CKfmt.ckSize, ALIGN) 
		     + 8 + RNDUPV(CKRIFF.CKdata.ckSize, ALIGN);

	CKRIFF.ckSize = (UT_uint4_t) size;

	fwrite(&(CKRIFF),4*3,1,fp); 

	fwrite(&CKRIFF.CKfmt,CKRIFF.CKfmt.ckSize+8,1,fp);

	fwrite(&(CKRIFF.CKdata),sizeof(CKRIFF.CKdata),1,fp);
}

DAS_FILE_FORMAT GetFileFormat(char* filename)
{
	

	if (filename==NULL)
	{
		return DAS_FORMAT_INVALID;
	}
	else
	{
		char* pt;
		int len;
		char postfix[4];
		len = strlen(filename);
		if (len > 3)
		{
			pt = filename + len -3;

			postfix[0] =toupper(pt[0]);
			postfix[1] =toupper(pt[1]);
			postfix[2] =toupper(pt[2]);
			postfix[3] ='\0';

			if (strcmp(postfix,"PCM")==0)
			{
				return DAS_PCM;
			}
			else if (strcmp(postfix,"WAV")==0)
			{
				return DAS_WAV;
			}
			else
			{
				return DAS_FORMAT_INVALID;
			}
		}
		else
		{
			return DAS_FORMAT_INVALID;
		}


	}

}