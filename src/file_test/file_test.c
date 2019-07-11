
#include"file_test.h"

void File_Test(FILE* fp ,double * p_TestData ,int len )
{
	int i;
	for (i = 0; i<len; i++)
	{
		fprintf(fp, "%18.16f\n", (p_TestData[i]));
	}
}
