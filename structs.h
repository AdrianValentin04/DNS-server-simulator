/* GHEORGHE Adrian-Valentin - 321CB */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

typedef int (*TFElem)(void*);     /* functie prelucrare element */
typedef int (*TFCmp)(void*, void*); /* functie de comparare doua elemente */
typedef void (*TF)(void*, FILE*);     /* functie afisare/eliberare un element */
typedef int (*TFHash)(void*, size_t);
typedef int (*TFSum)(void*);

typedef struct
{
	char *key;
	char *value;

} REG;

typedef struct celula2
{
  REG *info;              
  struct celula2 *pre, *urm; 

} TCelula2, *TLDI;

typedef struct
{
	size_t M;
	TLDI *v;
	TFHash fh;

} THash;

THash* IniTH(size_t M, TFHash fh);
void DistrTH(THash *aa);
void AfiTH(THash*a, TF afiEl, FILE* output);
int codH(void* element, size_t M);
void removeReg(THash *h, void* cel, TFCmp cmp);
void afisareDNS(void* element, FILE* output);
void InsTH(THash*h, void* cel, TFCmp cmp, TFSum sum);
int cmpRegister(void* e1, void* e2);
TCelula2* initRegistru(char* k, char* v);
void print_bucket(THash* h, TF afiEl, int i, FILE* output);
REG* getRegister(THash* h, void* cel, TFCmp cmp);
int cmpKey(void* e1, void* e2);
int sumCH(void* element);