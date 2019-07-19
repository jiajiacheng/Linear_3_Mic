#ifndef FILE_TEST_H
 #define FILE_TEST_H

#include<stdio.h>

//typedef struct FILE_Struct
//{
//	FILE *fp_gcr;
//	FILE *fp_glr;
//}FILE_Struct;

FILE *fp_dft;
FILE *fp_gcr, *fp_glr;
void File_open(char * filename, FILE* fp);
void generate_debug_file_name(char*  pFileArray, char* filename);
void File_Test(FILE* fp, double * p_TestData, int len);
void File_close(FILE* fp);
#endif