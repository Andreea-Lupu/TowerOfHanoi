/*LUPU ANDREEA - 311CB*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct celulag
{
	void *info;
	struct celulag *urm;
} Celula, *TLista, **ALista;

typedef struct stiva
{
	size_t dime;
	TLista vf;
} TStiva;

typedef struct coada
{
	size_t dime;
	TLista ic, sc;
} TCoada;

typedef struct SistemHanoi
{
	char *culoare;
	void *A, *B, *C;
	void *mutari;
} TSistemHanoi;

TLista AlocareCelula(void *x)
{
	TLista aux;
	aux = (TLista)malloc(sizeof(Celula));
	if (!aux) return NULL;
	aux->info = x;
	aux->urm = NULL;
	return aux;
}

int AdaugareCelulaInceput(ALista L, void* x)
/*intoarce 1-succes, 0-eroare*/
{
	TLista aux;
	aux = AlocareCelula(x);
	if(!aux) return 0;
	if(*L == NULL) { 
		*L = aux;
	} else {	
		aux->urm = *L;
		*L = aux;
	}
	return 1;
}

int AdaugareCelulaCoada(ALista ic, ALista sc, void *x)
/*intoarce 1-succes, 0-esec*/
{
	TLista aux;
	aux = AlocareCelula(x);
	if(!aux) return 0;
	if (*sc == NULL) {
		*ic = aux;
		*sc = aux;
	} else {
		(*sc)->urm = aux;
		aux->urm = NULL;
		*sc = aux;
	}
	return 1;
}

void * InitS(size_t d) /*initializare stiva*/
{
	TStiva *stiva = (TStiva *)malloc(sizeof(TStiva));
	if (!stiva) { 
		return NULL;
	}
	stiva->dime = d;
	stiva->vf = NULL;
	return (void *)stiva;
}

void * InitQ(size_t d) /*initializare coada*/
{
	TCoada *coada = (TCoada *)malloc(sizeof(TCoada));
	if (!coada) { 
		return NULL;
	}
	coada->dime = d;
	coada->ic = NULL;
	coada->sc = NULL;
	return (void *)coada;
}

void EliberareDiametru(void *x)
{
	free((int *)x);
}

void EliberareMutare(void *x)
{
	free((char *)x);
}

void DistrugeLista(ALista L, void(*felib)(void*))
{
	TLista aux;
	while(*L != NULL)
	{
		aux = *L;
		*L = aux->urm;
		felib(aux->info);
		free(aux);
	}
	
}

void DistrugeS(void **astiva) /*distrugere stiva*/
{
	DistrugeLista(&(((TStiva *)(*astiva))->vf), EliberareDiametru);
	free(*astiva);
	*astiva = NULL;	
}

void DistrugeQ(void **acoada) /*distrugere coada*/
{
	DistrugeLista(&(((TCoada *)(*acoada))->ic), EliberareMutare);
	free(*acoada);
	*acoada = NULL;	
}

void EliberareSistemHanoi(void *x)
{
	free(((TSistemHanoi *)x)->culoare);
	DistrugeS(&(((TSistemHanoi *)x)->A));
	DistrugeS(&(((TSistemHanoi *)x)->B));
	DistrugeS(&(((TSistemHanoi *)x)->C));
	DistrugeQ(&(((TSistemHanoi *)x)->mutari));
	free((TSistemHanoi *)x);
}

int InitSistHanoi (char *color, size_t d, void *diam, ALista L)
/*initializare sistem Hanoi de o anumita culoare*/
/*intoarce 1-succes, 0-eroare*/
{
	int ok;
	TSistemHanoi *sistem = (TSistemHanoi*)malloc(sizeof(TSistemHanoi));
	if (!sistem) { 
		return 0;
	}
	sistem->culoare = strdup(color);
	if (!sistem->culoare) {
		free(sistem);
		return 0;
	}
	sistem->A = InitS(d);
	if(!sistem->A) {
		free(sistem);
		free(sistem->culoare);
		return 0;
	}
	sistem->B = InitS(d);
	if(!sistem->B) {
		free(sistem);
		free(sistem->culoare);
		DistrugeS(&(sistem->A));
		return 0;
	}
	sistem->C = InitS(d);
	if(!sistem->C) {
		free(sistem);
		free(sistem->culoare);
		DistrugeS(&(sistem->A));
		DistrugeS(&(sistem->B));
		return 0;
	}
	sistem->mutari = InitQ(d);
	if(!sistem->mutari) {
		free(sistem);
		free(sistem->culoare);
		DistrugeS(&(sistem->A));
		DistrugeS(&(sistem->B));
		DistrugeS(&(sistem->C));
		return 0;
	}
	/*adaugarea noului sistem Hanoi in lista sistemelor Hanoi*/
	ok = AdaugareCelulaInceput(L, (void*) sistem);
	if(!ok) {
		free(sistem);
		free(sistem->culoare);
		DistrugeS(&(sistem->A));
		DistrugeS(&(sistem->B));
		DistrugeS(&(sistem->C));
		DistrugeQ(&(sistem->mutari));
		return 0;
	}
	/*adaugarea primului disc in stiva A a noului sistem Hanoi construit*/
	ok = AdaugareCelulaInceput(&(((TStiva *)(sistem->A))->vf), diam);
	if(!ok) {
		printf("Nu am putut insera discul\n");	
		return 0;
	}
	return 1;
}

TLista CautareSistemHanoi(char * color, TLista ListaSisteme)
/*intoarce adreasa sistemului Hanoi cautat*/
{
	TLista aux = ListaSisteme;
	for(; aux != NULL; aux = aux-> urm) {
		if(strcmp(((TSistemHanoi*)(aux->info))->culoare, color) == 0) {
			return aux;	
		}
	}
	return NULL;
}

void MutaS(void *ad, void *as)
/*muta un element din varful stivei sursa in varful stivei destinatie*/
{
	TLista aux = ((TStiva*)as)->vf;
	((TStiva*)as)->vf = aux->urm;
	if (((TStiva*)ad)->vf == NULL) {
		((TStiva*)ad)->vf = aux;
		aux->urm = NULL;
	} else {
		aux->urm = ((TStiva*)ad)->vf;
		((TStiva*)ad)->vf = aux;
	}
}

void MutaQ(void *ad, void *as)
/*muta un element din coada sursa in coada destinatie*/
{
	TLista aux = ((TCoada*)as)->ic;
	((TCoada*)as)->ic = aux->urm;
	if(aux->urm == NULL) ((TCoada*)as)->sc = NULL;
	if (((TCoada*)ad)->ic == NULL) {
		((TCoada*)ad)->ic = aux;
		aux->urm = NULL;
		((TCoada*)ad)->sc = aux;
	} else {
		((TCoada*)ad)->sc->urm = aux;
		aux->urm = NULL;
		((TCoada*)ad)->sc = aux;
	}
}

void SuprapuneQ(void *ad, void *as)
/*suprapune coada sursa la finalul cozii destinatie*/
{
	while( ((TCoada*)as)->ic != NULL)
	{
		MutaQ(ad, as);
	}
}

void RastoarnaS(void *ad, void *as)
/*suprapune inversul stivei sursa peste stiva destinatie*/
{
	while( ((TStiva*)as)->vf != NULL)
	{
		MutaS(ad, as);
	}
}

void Add(char * color, int dim_disc, ALista ListaSist)
/*adaugare disc pe o stiva ce contine discuri de aceeasi culoare*/
{
	int ok;
	int * diam = (int *) malloc(sizeof(int));
	if(!diam) {
		printf("Nu s-a putut face alocare");
		return ;
	}
	*diam = dim_disc;
	TLista sist_cautat = CautareSistemHanoi(color, *ListaSist);
	if (!sist_cautat) {
		/*nu avem un sistem hanoi de culoarea discului ce se doreste a fi
	inserat, deci vom initializa un nou sistem Hanoi*/
		ok = InitSistHanoi (color, sizeof(int), (void*) diam, ListaSist);
		if(!ok) {
			printf("Nu s-a putut initializa sistemul Hanoi\n");
		}
		
	} else {
		ALista varf = &(((TStiva*)(((TSistemHanoi*)(sist_cautat->info))->A))->
						vf);
		if (dim_disc < *(int*)((*varf)->info)) {
		/*discul pe care vrem sa il inseram are un diametru mai mic decat
	discul aflat in varful stivei A, deci il putem pune in varf*/
			ok = AdaugareCelulaInceput(varf, (void*)diam);
			if(!ok) {
				free(diam);
				printf("Nu s-a putut adauga discul\n");
			}
		} else {
		/*discul pe care vrem sa il inseram nu are un diametru mai mic decat
	discul aflat in varful stivei A, deci nu il putem pune in varf, asa ca
	trebuie sa ii gasim locul potrivit in stiva A in care il putem insera */
			void * saux = InitS(sizeof(int));
			if(!saux) {
				printf("Eroare initializare\n");
				return;
			}
			while((*varf != NULL) && (dim_disc > *(int*)((*varf)->info)))
			{
				MutaS(saux, ((TSistemHanoi*)(sist_cautat->info))->A);
			}
			ok = AdaugareCelulaInceput(varf, (void*)diam);
			if(!ok) {
				free(diam);
				printf("Nu s-a putut adauga discul\n");
			}
			RastoarnaS(((TSistemHanoi*)(sist_cautat->info))->A, saux);
			DistrugeS(&saux);
		}
	}
}

void AfisareStiva(void *stiva, FILE * f) /*afiseaza elementele dintr-o stiva
										incepand cu elementul de la baza ei*/
{
	if (((TStiva*)stiva)->vf == NULL) {
		fprintf(f, "\n");
		return;
	} else {
		void * saux = InitS(sizeof(int));
		if(!saux) {
			printf("Eroare initializare\n");
			return;
		}
		RastoarnaS(saux, stiva);
		while(((TStiva*)saux)->vf != NULL)
		{
			fprintf(f, " %d", *(int *)((((TStiva*)saux)->vf)->info));
			MutaS(stiva, saux);
		}
		fprintf(f, "\n");
		DistrugeS(&saux);
	}
}

void Show(char *color, TLista ListaSist, FILE * f)
/*afisarea elementelor celor 3 stive ale unui sistem Hanoi*/
{
	TLista sist_cautat = CautareSistemHanoi(color, ListaSist);
	if (!sist_cautat) {
		fprintf(f, "A_%s:\n", color);
		fprintf(f, "B_%s:\n", color);
		fprintf(f, "C_%s:\n", color);
	} else {
		fprintf(f, "A_%s:", color);
		AfisareStiva(((TSistemHanoi*)(sist_cautat->info))->A, f);
		fprintf(f, "B_%s:", color);
		AfisareStiva(((TSistemHanoi*)(sist_cautat->info))->B, f);
		fprintf(f, "C_%s:", color);
		AfisareStiva(((TSistemHanoi*)(sist_cautat->info))->C, f);
	}
}

int NrElemS(void *stiva) /*numara elementele dintr-o stiva*/
{
	int nr = 0;
	if (((TStiva*)stiva)->vf != NULL) {
		void * saux = InitS(sizeof(int));
		if(!saux) {
			printf("Eroare initializare\n");
			return -1;
		}
		RastoarnaS(saux, stiva);
		while(((TStiva*)saux)->vf != NULL)
		{
			nr++;
			MutaS(stiva, saux);
		}
		DistrugeS(&saux);
	}
	return nr;
}

int NrElemQ(void *coada)  /*numara elementele dintr-o coada*/
{
	int nr = 0;
	if (((TCoada*)coada)->ic != NULL) {
		if (((TCoada*)coada)->ic == ((TCoada*)coada)->sc) {
			nr = 1; 
		} else {
			void * caux = InitQ(sizeof(char) * 5);
			if(!caux) {
				printf("Eroare initializare\n");
				return -1;
			}
			SuprapuneQ(caux, coada);
			while(((TCoada*)caux)->ic != NULL)
			{
				nr++;
				MutaQ(coada, caux);
			}
			DistrugeQ(&caux);
		}
	}
	return nr;
}

void PasiRezolvare(char * pas, void *q)
/*adauga pasii necesari rezolvarii unui sistem Hanoi in coada in care se
pastreaza toate mutarile care trebuie facute*/
{
	char *mutare = strdup(pas);
	if(!mutare) return;
	AdaugareCelulaCoada(&(((TCoada*)q)->ic), &(((TCoada*)q)->sc),(void*)mutare);
}

void RezolvareHanoi(int nr, char a, char c, char b, void *q)
/*algoritmul necesar rezolvarii unui sistem Hanoi*/
{
	char m[5];
	if (nr == 1) {
		m[0] = a;
		m[1] = '-';
		m[2] = '>';
		m[3] = c;
		m[4] = '\0';
		PasiRezolvare(m, q); /*adaugam mutarea in coada de mutari*/
	} else {
		RezolvareHanoi(nr-1, a, b, c, q);
		m[0] = a;
		m[1] = '-';
		m[2] = '>';
		m[3] = c;
		m[4] = '\0';
		PasiRezolvare(m, q); /*adaugam mutarea in coada de mutari*/
		RezolvareHanoi(nr-1, b, c, a, q);
	}	
}

void Play(char * color, int nr_mutari, ALista ListaSist)
/*executarea unui numar de mutari pentru rezolvarea sistemului Hanoi*/
{
	TLista sist_cautat = CautareSistemHanoi(color, *ListaSist);
	if(!sist_cautat) {
		return;
	}
/*verificam daca a fost construita coada in care se pastreaza mutarile*/
	if(((TCoada*)(((TSistemHanoi*)(sist_cautat->info))->mutari))->ic == NULL &&
		((TStiva*)(((TSistemHanoi*)(sist_cautat->info))->C))->vf == NULL) {
/*calculam numarul elementelor din stiva A*/
		int nr_elem = NrElemS(((TSistemHanoi*)(sist_cautat->info))->A);
		char a = 'A';
		char b = 'B';
		char c = 'C';
/*construim coada cu mutarile necesare rezolvarii sistemului*/
		RezolvareHanoi(nr_elem, a, c, b, ((TSistemHanoi*)(sist_cautat->info))
			->mutari);
	}
/*calculam numarul total de pasi necesari rezolvarii sistemului Hanoi*/
	int nr_pasi = NrElemQ(((TSistemHanoi*)(sist_cautat->info))->mutari);
	int i, n;
/*verificam daca numarul de mutari pe care trebuie sa le facem e mai mic
decat numarul total de mutari care trebuie efectuate pentru rezolvarea 
sistemului*/
	if (nr_mutari < nr_pasi) {
		n = nr_mutari;
	} else {
		n = nr_pasi;
	}
/*efectuam numarul de mutari cerut si stergem din coada de mutari fiecare
mutare pe care o efectuam*/
	for (i = 0; i < n; i++) {
		char operatie[5];
		strcpy(operatie, (char *)(((TCoada*)(((TSistemHanoi*)(sist_cautat->
			info))->mutari))->ic->info));
		void * from;
		void * to;
		if(operatie[0] == 'A') {
			from = ((TSistemHanoi*)(sist_cautat->info))->A;
		} else if(operatie[0] == 'B') {
			from = ((TSistemHanoi*)(sist_cautat->info))->B;
		} else if(operatie[0] == 'C') {
			from = ((TSistemHanoi*)(sist_cautat->info))->C;
		}
		if(operatie[3] == 'A') {
			to = ((TSistemHanoi*)(sist_cautat->info))->A;
		} else if(operatie[3] == 'B') {
			to = ((TSistemHanoi*)(sist_cautat->info))->B;
		} else if(operatie[3] == 'C') {
			to = ((TSistemHanoi*)(sist_cautat->info))->C;
		}
		TLista aux;
		ALista ic = &(((TCoada*)(((TSistemHanoi*)(sist_cautat->info))->mutari))
			->ic);
		ALista sc = &(((TCoada*)(((TSistemHanoi*)(sist_cautat->info))->mutari))
			->sc); 
		aux = *ic;
		*ic = aux->urm;
		if(aux->urm == NULL) {
			*sc = NULL;
		}
		aux->urm = NULL;
		EliberareMutare((void*)(aux->info));
		free(aux);
/*mutam discul din stiva sursa in stiva destinatie asa cum se specifica in
coada mutarilor*/
		MutaS(to, from);
	}
}

void Show_moves(char * color, int nr_mutari, FILE *f, TLista ListaSist)
/*arata urmatoarele [nr_mutari] mutari ce trebuie efectuate in vederea
rezolvarii sistemului Hanoi*/
{
	TLista sist_cautat = CautareSistemHanoi(color, ListaSist);
	fprintf(f, "M_%s:", color);
	if(!sist_cautat) {
		fprintf(f, "\n");
		return;
	}
/*verificam daca a fost construita coada in care se pastreaza mutarile*/
	if(((TCoada*)(((TSistemHanoi*)(sist_cautat->info))->mutari))->ic == NULL &&
		((TStiva*)(((TSistemHanoi*)(sist_cautat->info))->C))->vf == NULL) {
/*calculam numarul elementelor din stiva A*/
		int nr_elem = NrElemS(((TSistemHanoi*)(sist_cautat->info))->A);
		char a = 'A';
		char b = 'B';
		char c = 'C';
/*construim coada cu mutarile necesare rezolvarii sistemului*/
		RezolvareHanoi(nr_elem, a, c, b, ((TSistemHanoi*)(sist_cautat->info))
			->mutari);
	}
/*calculam numarul total de pasi necesari rezolvarii sistemului Hanoi*/
	int nr_pasi = NrElemQ(((TSistemHanoi*)(sist_cautat->info))->mutari);
	int i, n;
/*verificam daca numarul de mutari pe care trebuie sa le afisam e mai mic
decat numarul total de mutari continut de coada mutarilor*/
	if (nr_mutari < nr_pasi) {
		n = nr_mutari;
	} else {
		n = nr_pasi;
	}
/*verificam daca avem mutari de afisat*/
	if (n != 0) {
/*afisam din coada mutarilor numarul de mutari cerut*/
		void * caux = InitQ(sizeof(char) * 5);
				if(!caux) {
					printf("Eroare initializare\n");
					return;
				}
		for (i = 0; i < n; i++) {
			fprintf(f, " %s", (char *)(((TCoada*)(((TSistemHanoi*)(sist_cautat->
				info))->mutari))->ic->info));
			MutaQ(caux, ((TSistemHanoi*)(sist_cautat->info))->mutari);
		}
		if(((TCoada*)(((TSistemHanoi*)(sist_cautat->info))->mutari))->ic 
			!= NULL) {
			SuprapuneQ(caux, ((TSistemHanoi*)(sist_cautat->info))->mutari);
		}
		SuprapuneQ(((TSistemHanoi*)(sist_cautat->info))->mutari, caux);
		DistrugeQ(&caux);
	}
	fprintf(f, "\n");
}

int main(int argc, char *argv[])
{
	char operatie[15], color[15];
	int i, N, diametru, nr_mutari;
	TLista ListaSistemeHanoi = NULL;
	FILE *in = fopen(argv[1], "rt");
	if (in == NULL) {
		fprintf(stderr, "ERROR: Can`t open file %s", argv[1]);
		return -1;
	}
	FILE *out = fopen(argv[2], "wt");
	if (out == NULL) {
		fprintf(stderr, "ERROR: Can`t open file %s", argv[2]);
		return -1;
	}
	fscanf(in, "%d", &N);
	for(i = 1; i <= N; i++) {
		fscanf(in, "%s", operatie);
		if(strcmp(operatie,"add") == 0) {
			fscanf(in, "%s %d", color, &diametru);
			Add(color, diametru, &ListaSistemeHanoi);
		} else if(strcmp(operatie,"play") == 0) {
			fscanf(in, "%s %d", color, &nr_mutari);
			Play(color, nr_mutari, &ListaSistemeHanoi);
		} else if(strcmp(operatie,"show") == 0) {
			fscanf(in, "%s", color);
			Show(color, ListaSistemeHanoi, out);	
		} else if(strcmp(operatie,"show_moves") == 0) {
			fscanf(in, "%s %d", color, &nr_mutari);
			Show_moves(color, nr_mutari,out, ListaSistemeHanoi);
		} 
	}	
	DistrugeLista(&ListaSistemeHanoi, EliberareSistemHanoi);
	fclose(in);
	fclose(out);
	return 0;
}
