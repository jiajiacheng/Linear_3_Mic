#include<string.h>
#include"file.h"
#include"common_type.h"

#ifdef DEBUG_ON
U32_T  g_Frame = 1;
char  debug_save_path[FILENAME_LEN_MAX];
U32_T  debug_flag = 8;



void generate_debug_file_name(char*  pFileArray, char* filename)
{
	char  index_name[8];
	strcpy_s(pFileArray, FILENAME_LEN_MAX, DEBUG_PATH);
	strcat_s(pFileArray, FILENAME_LEN_MAX, debug_save_path);
	strcat_s(pFileArray, FILENAME_LEN_MAX, filename);
	_itoa_s(g_Frame, index_name, sizeof(index_name), 10);
	strcat_s(pFileArray, FILENAME_LEN_MAX, "_");
	strcat_s(pFileArray, FILENAME_LEN_MAX, index_name);
	strcat_s(pFileArray, FILENAME_LEN_MAX, "_vs");
	strcat_s(pFileArray, FILENAME_LEN_MAX, ".dat");
}
void File_open(char* filename,FILE* fp)
{
	char  fullname_bat[FILENAME_LEN_MAX];
	S16_T err;
	generate_debug_file_name(fullname_bat, filename);
	err = fopen_s(fp, fullname_bat, "wb");
	if (err != 0)
	{
		printf("ERROR OPENGING FILE %s\n", fullname_bat);
		return L3M_DAT_ERR;
	}
}

void File_Test(FILE* fp, FLOAT_T * p_TestData, int len)
{
	FILE  *fp_dat = NULL;
	
	S16_T err,m;
	for (m = 0; m<len; m++)
	{
		fprintf(fp, "%.16f\n", *(p_TestData++));
	}
}
void File_Complex_Test(FILE* fp, complexDouble * p_TestData, int len)
{
	FILE  *fp_dat = NULL;

	S16_T err, m;
	fprintf(fp, "%.16f\n", (p_TestData[0].real));
	fprintf(fp, "%.16f\n", 0);
	for (m = 1; m<len; m++)
	{
		fprintf(fp, "%.16f\n", (p_TestData[m].real));
		fprintf(fp, "%.16f\n", (p_TestData[m].imag));
	}
	fprintf(fp, "%.16f\n", (p_TestData[len].real));
	fprintf(fp, "%.16f\n", 0);
}
void File_close(FILE* fp)
{
	fclose(fp);
}
#endif