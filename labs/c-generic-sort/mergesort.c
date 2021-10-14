#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>



void cparray(char *lineptr[], int l, int r, char *aux[]){

	for(int k = l; k < r; k++){
		aux[k] = lineptr[k];
	}
}

void split(char *aux[], int l, int r, char *lptr,int (*comp)(void *, void *)){
	if(r - l <= 1){
		return;
  }
	int m = (r + l) / 2;
	split(lptr, l, m, aux, comp);
	split(lptr, m, r, aux, comp);
	merge(aux, l, m, r, lptr, comp);
}

void merge(char *lp[], int l, int m, int r, char *aux[], int (*comp)(void *, void *)){
	int i = l, j = m;

	for(int k = l; k < r; k++){
		if(i < m && (j >= r || (*comp)(lp[i], lp[j]) <= 0)){
			aux[k] = lp[i++];
		}else{
			aux[k] = lp[j++];
		}
	}
}

void mergesort(void *lineptr[], int left, int right, int (*comp)(void *, void *)) {
	char *auxlineptr[10000];
	cparray(lineptr, left, right, auxlineptr);
	split(auxlineptr, left, right,lineptr, comp);

}
