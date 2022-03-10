/* GHEORGHE Adrian-Valentin - 321CB */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"


int main(int argc, char **argv){
	
	FILE *inputfile = fopen(argv[2], "r");
	FILE *outputfile = fopen(argv[3], "w");
	int M = atoi(argv[1]);
	char *token, line[1000], *v, *k;
	int index_bucket;
	REG* reg;
	TCelula2* celula;
	THash *h = NULL;
	h = (THash *) IniTH(M, codH);
	
	//Facem consola
	while(fgets(line, 10000, inputfile) != NULL)
	{
		token = strtok(line, " \n");
		
		//conditie pusa pentru liniile goale date ca si comanda
		if(token == NULL)
			break;
		
		//comanda put
		else if(!strcmp(token, "put"))
		{
			token = strtok(NULL, " ");
			k = strdup(token);
			token = strtok(NULL, " \n\0");
			v = strdup(token);

			celula = initRegistru(k,v);
			InsTH(h, celula, cmpRegister, sumCH);
		}

		//comanda get
		else if(!strcmp(token, "get"))
		{
			token = strtok(NULL, " \n\0");
			k = strdup(token);
			
			//acea valoare este pusa doar pentru a putea crea un registru
			celula = initRegistru(k, "1.1.1.1");

			reg = getRegister(h,celula, cmpKey);
			if(reg == NULL){
				fprintf(outputfile, "NULL\n");
			}
			else
			{
				fprintf(outputfile, "%s\n", reg->value);
			}
			
			free(celula->info);
			free(celula);
			free(k);
		}

		//comanda print
		else if(!strcmp(token, "print"))
		{
			AfiTH(h, afisareDNS, outputfile);
		}

		//comanda print_bucket
		else if(!strcmp(token, "print_bucket"))
		{
			token = strtok(NULL, " \n\0");
			index_bucket = atoi(token);
			print_bucket(h, afisareDNS, index_bucket, outputfile);
		}

		//comanda find
		else if(!strcmp(token, "find"))
		{
			token = strtok(NULL, " \n\0");
			k = strdup(token);
			
			//acea valoare este pusa doar pentru a putea crea un registru
			celula = initRegistru(k, "1.1.1.1");

			reg = getRegister(h, celula, cmpKey);
			if(reg == NULL)
			{
				fprintf(outputfile, "False\n");
			}
			else
			{
				fprintf(outputfile, "True\n");
			}
			
			free(celula->info);
			free(celula);
			free(k);
		}

		//comanda remove
		else if(!strcmp(token, "remove"))
		{
			token = strtok(NULL, " \n\0");
			k = strdup(token);

			//acea valoare este pusa doar pentru a putea crea un registru
			celula = initRegistru(k, "1.1.1.1");
			removeReg(h, celula, cmpKey);
			
			free(celula->info);
			free(celula);
			free(k);
		}
	}

	DistrTH(h);
	fclose(inputfile);
	fclose(outputfile);
	return 0;
}