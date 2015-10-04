#include <stdio.h>
#include <stdlib.h>

#include "..\Binum\Binum.h"
#ifdef _DEBUG
#pragma comment(lib,"..\\Debug\\Binum.lib")
#else
#pragma comment(lib,"..\\Release\\Binum.lib")
#endif

int main(void)
{
	BINUMBER a = NULL;
	BINUMBER b = NULL;

	BinumNew(&a);
	BinumNew(&b);

	//BinumSet(a,"2000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
	//BinumSet(b,"5000000000000000000000000000000000000000000000000000000000000000000000000");

	BinumSet(a,"300000000000000000000000000000000000000");
	BinumSet(b,"50000000000");

	BINUMBER c = NULL;

	BinumNew(&c);

	//BinumCopy(c,b);

	//BinumAdd(c,a,b);
	//BinumMultiplySmall(c,a,23);
	BinumSubstract(c,a,b);
	//BinumMultiply(c,a,b);
	//BinumShiftRight(c,a,22);
	//BinumShiftLeft(c,c,22);

	CHAR result[4096];

	BinumGet(result,c);

	printf("Result: %s\r\n",result);
	system("pause");

	return 0;
}