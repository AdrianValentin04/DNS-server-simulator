/* GHEORGHE Adrian-Valentin - 321CB */

#include <stdio.h>
#include <string.h>

#include "structs.h"

/////////////Functii auxiliare/////////////////

//functie pentru initializarea tabelei hash(laborator)
THash* IniTH(size_t M, TFHash fh)
{
	THash* h = (THash *) calloc(sizeof(THash), 1);
    if (!h) 
    {
        return NULL;
    }

    h->v = (TLDI *) calloc(M, sizeof(TLDI));
    if(!h->v) 
    {
        free(h);
        return NULL;
    }

    h->M = M;
    h->fh = fh;
    return h;	
} 

//functie pentru distrugerea tabelei hash(laborator)
void DistrTH(THash* h)
{
    TLDI * p, el, aux;

    for (p = h->v; p < h->v + h->M; p++)
    {
        for(el = *p; el != NULL; ) 
        {
            aux = el;
            el = el->urm;
            free(aux->info->key);
            free(aux->info->value);
            free(aux->info);
            free(aux);
        }
    }

    free(h->v);
    free(h);
    h = NULL;
}

//functie pentru afisarea tabelei hash(laborator)
void AfiTH(THash* h, TF afiEl, FILE* output)
{
    TLDI p, el;
    int i;
    for(i = 0; i < h->M; i++) 
    {
        p = h->v[i];
        if(p) 
        {
            fprintf(output, "%d: ",i);
            for(el = p; el != NULL; el = el->urm)
                afiEl(el->info, output);
            fprintf(output, "\n");
        }
    }
}

//functie pentru comanda print_bucket
void print_bucket(THash* h, TF afiEl, int i, FILE* output)
{
    TLDI p, el;
    p = h->v[i];

    //testez daca mi se cere sa afisez o lista care nu exista
    if(i >= h->M)
        return;

    //testez daca lista pe care trebuie sa o afisez e nula sau nu
    if(p == NULL){
        fprintf(output, "VIDA\n");
    }
    else
    {
        for(el = p; el != NULL; el = el->urm)
            afiEl(el->info, output);
        fprintf(output, "\n");
    }
    
}

//functia hash(cursul de tabele hash)
int codH(void* element, size_t M)
{

	unsigned long suma;
	char* x = (char*) element;

	for (suma = 0; *x != '\0'; x++)
	    suma += *x;


	return(suma % M);
}

//functie care calculeaza doar suma caracterelor cheilor
int sumCH(void* element)
{
    unsigned long suma;
	char* x = (char*) element;

	for (suma = 0; *x != '\0'; x++)
		suma += *x;

    return suma;
}

//functie de inserare in tabela hash
void InsTH(THash*h, void* cel, TFCmp cmp, TFSum sum)
{
    TCelula2 *el, *celula, *ant;
    celula = (TCelula2*) cel;

    //calculam pozitia pe care trebuie inserat elementul
    int cod = h->fh(celula->info->key, h->M);
    
    //testez daca lista e goala ca sa inserez elementul la inceput de lista
    if(h->v[cod] == NULL)
        h->v[cod] = celula;
    else{
        //luam la rand lista si testam daca elementul se afla deja in lista
        for(el = h->v[cod]; el != NULL; el = el->urm){
            if (cmp(el->info, celula->info) == 1)
            {
                free(celula->info->key);
			    free(celula->info->value);
                free(celula->info);
                free(celula);
                return;
            }
        }
        
        //ne intoarcem pe pozitia initiala si testam daca 
        //elementul trebuie inserat inaintea primului element din lista
        el = h->v[cod];
        if(strcmp(el->info->key,celula->info->key) >= 0)
        {
            h->v[cod] = celula;
            celula->urm = el;
            el->pre = celula; 
        }
        else
        {
            //comparam elementul cu toate din lista in afara de ultimul 
            //pentru a vedea unde trebuie inserat
            for(el = h->v[cod]; el->urm != NULL; el = el->urm)
            {
                if(strcmp(el->info->key,celula->info->key) >= 0)
                {
                    ant = el->pre;
                    ant->urm = celula;
                    celula->urm = el;
                    el->pre = celula;
                    celula->pre = ant;

                    break;
                }
            }
            //testam daca a fost inserat sau nu
            if(el->urm == NULL)
            {   
                //vedem daca trebuie inserat inainte sau dupa ultimul element
                if(strcmp(el->info->key, celula->info->key) > 0)
                {
                    el->pre->urm = celula;
                    celula->pre = el->pre;
                    celula->urm = el;
                    el->pre = celula;
                }
                else
                {
                    el->urm = celula;
                    celula->pre = el;
                    celula->urm = NULL;
                }       
            }
        }  
    }   
}

//functie pentru comanda get
REG* getRegister(THash* h, void* cel, TFCmp cmp)
{
    TCelula2 *el, *celula;
    celula = (TCelula2*) cel;

    //calculam functia hash apoi cautam elementul
    int cod = h->fh(celula->info->key, h->M);
    
    if(h->v[cod] == NULL)
        return NULL;
    else
    {
        for(el = h->v[cod]; el != NULL; el = el->urm)
        {
            if(cmp(el->info, celula->info))
                return el->info;
        }
    }
    return NULL;
}

//initializam o celula cu info de tip registru
TCelula2* initRegistru(char* k, char* v)
{
    REG* reg = (REG*)malloc(sizeof(REG));
    reg->key = k;
    reg->value = v;

    TCelula2* celula = (TCelula2*)malloc(sizeof(TCelula2));
    celula->info = reg;
    celula->pre = celula->urm = NULL;
    return celula;
}

//functie de afisare a unei intrari in DNS(Key, Value)
void afisareDNS(void* element, FILE* output)
{
	REG *reg = (REG*) element;
	fprintf(output,"%s ",reg->value);
}

//functie de comparare a 2 registre
int cmpRegister(void* e1, void* e2)
{
	REG* register1 = (REG*)e1;
	REG* register2 = (REG*)e2;

	if(strcmp(register1->key, register2->key) != 0)
		return 0;
	if(strcmp(register1->value, register2->value) != 0)
		return 0;

	return 1;

}

//functie de comparare a 2 chei de registre
int cmpKey(void* e1, void* e2)
{    
    REG* register1 = (REG*)e1;
	REG* register2 = (REG*)e2;

	if(strcmp(register1->key, register2->key) != 0)
		return 0;

    return 1;
}

//functie pentru comanda remove
void removeReg(THash *h, void* cel, TFCmp cmp)
{
    TCelula2 *el, *celula, *aux;
    celula = (TCelula2*) cel;
    int cod = h->fh(celula->info->key, h->M);
   
    //testam daca lista de unde trebuie sa stergem e goala
    if(h->v[cod] == NULL)
        return;
    
    //testam daca elementul ce trebuie sters e primul
    if(cmp(h->v[cod]->info, celula->info))
    {
        aux = h->v[cod];
        h->v[cod] = h->v[cod]->urm;
        free(aux->info->key);
        free(aux->info->value);
        free(aux->info);
        free(aux);
    }
    else
    {
        //cautam elementul in lista
        for(el = h->v[cod]; el->urm != NULL; el = el->urm)
        {
            if(cmp(el->info, celula->info))
            {
                el->pre->urm = el->urm;
                el->urm->pre = el->pre;
                free(el->info->key);
                free(el->info->value);
                free(el->info);
                free(el);
                break;
            }
        }

        //verificam daca ultimul element e cel ce trebuie sters
        if(el->urm == NULL)
        {
            if(cmp(el->info, celula->info))
            {
                el->pre->urm = NULL;
                free(el->info->key);
                free(el->info->value);
                free(el->info);
                free(el);
            }
        }
    }
}
