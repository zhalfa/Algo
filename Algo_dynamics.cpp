// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

/*p82 3.5*/

#define COINCNT 4
#define MAXVAL  12

unsigned FindLightest(unsigned y, unsigned v[], unsigned w[], unsigned cnt){

	unsigned f[1 + MAXVAL][1 + COINCNT] = { 0 };
	unsigned c[1 + MAXVAL][1 + COINCNT] = { 0 };

	for (unsigned i = 1; i <= COINCNT; i++ ){
	
		for (unsigned j = 1; j <= MAXVAL; j++){

			if (1 == i){

				f[j][i] = w[i] * j;
				c[j][i] = j;

			}
			else{

				unsigned min = f[j][i - 1];

				if (j < v[i]){ f[j][i] = f[j][i - 1]; c[j][i] = 0; }
				else{
					unsigned tmp = 0;

					if (j >= v[i]){

						tmp = f[j - v[i]][i] + w[i];

						if (tmp < min){
							f[j][i] = tmp;
							c[j][i] = c[j - v[i]][i] + 1;
							min = tmp;
						}else{
						
							f[j][i] = min;//???
							c[j][i] = 0;
						}

					}
				}
			}
		}
	}
	return f[MAXVAL][COINCNT];
}


int _tmain(int argc, _TCHAR* argv[])
{

	unsigned v[1+COINCNT] = { 0, 1, 4, 6, 8 };
	unsigned w[1 + COINCNT] = { 0, 1, 2, 4, 6 };

	unsigned res = FindLightest(MAXVAL, v, w, 4);

	return 0;
}

