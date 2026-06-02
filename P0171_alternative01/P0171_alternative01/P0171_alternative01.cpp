#include <stdio.h>
#include <time.h>

#define TAILLE_T 2000
#define MODULO 1000000000ULL


int main(int argc, const char* argv[])
{
	clock_t t1, t2;

	unsigned long long ta[TAILLE_T], tn[TAILLE_T], na[TAILLE_T], nn[TAILLE_T], puiss, somme = 0;
	int i, j, k, n, indice;

	t1 = clock();

	for (i = 0; i < TAILLE_T; i++) { ta[i] = 0; na[i] = 0; };
	for (i = 0; i <= 9; i++)
	{
		j = i * i;
		ta[j] = i;
		na[j] = 1;
	}
	puiss = 10;
	for (n = 1; n <= 19; n++)
	{
		for (j = 1; j < TAILLE_T; j++) { tn[j] = 0; nn[j] = 0; }; nn[0] = 1;
		for (k = 1; k < TAILLE_T; k++)
		{
			for (i = 0; i <= 9; i++)
			{
				indice = k - i * i;
				if (indice >= 0 && na[indice] > 0)
				{
					nn[k] += na[indice];
					tn[k] = (tn[k] + na[indice] * puiss * i + ta[indice]) % MODULO;
				}
			}
		}
		for (k = 0; k < TAILLE_T; k++) { ta[k] = tn[k]; na[k] = nn[k]; }
		puiss = puiss * 10 % MODULO;
	}

	for (i = 1; i < 40; i++) { j = i * i; somme = (somme + ta[j]) % MODULO; }

	t2 = clock();
	printf("temps de calcul : %lf secondes\n", (t2 - t1) / 1e6);
	printf("somme : %llu ", somme);
	return 0;
}