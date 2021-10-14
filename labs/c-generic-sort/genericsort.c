#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MLX 5000
#define MLEN 1000
#define ALLOC 100000
static char allocbuf[ALLOC];
static char *allocp = allocbuf;
char *lptr[MLX];
int lines;

char *alloc(int n){
	if(allocbuf + ALLOC - allocp >= n ){
		allocp += n;
		return allocp - n;
	}else{
		return 0;
	}
}

void writelines(char *lptr[], int nl, char *name){
	FILE *fp;
	char *nn = malloc(sizeof(name) + 9);
	char *prefix = "sorted_";
	strcpy(nn, prefix);
	strcat(nn, name);
	printf("... Results file can be found at ./%s \n", nn);
	fp = fopen(nn, "w");
	if(fp == NULL){
		exit(-1);
	}
	for(int i = 1; i < nl; i++){
		fprintf(fp,"%s", lptr[i-1]);
	}
	fclose(fp);
}

int readlines(char *lptr[], int mls, char *file){
	char *p;
	int lines = 0;
	FILE *fp = fopen(file, "r");
	if(fp == NULL){
		printf("Error, no se pudo abrir el archivo\n");
		return 1;
	}
	char *line = NULL;
	size_t len = 0;
	while(getline(&line, &len, fp) != -1 )
		if(lines >= mls || (p = alloc(len)) == NULL){
			return -1;
		}else{
			line[len-1] = '\0';
			strcpy(p, line);
			lptr[lines++] = p;
		}

	fclose(fp);
	free(line);
	return lines;
}

int numcmp(char *s1, char *s2){
	double v1,v2;
	v1 = atof(s1);
	v2 = atof(s2);
	if(v1 < v2)
		return -1;
	else if(v1 > v2)
		return 1;
	else
		return 0;
}

void quicksort(void *lptr[], int left, int right, int (*comp)(void *, void *));

void mergesort(void *choicename[], int left, int right, int (*comp)(void *, void *));

int main(int argc, char **argv){
	int choice;
	int nl;
	int aux = 0;
	char *filen, *choicename;
	int arg;
	if(argc > 1 && strcmp(argv[1], "-n") == 0){ //Comparacion numerica
		choice = 3;
		arg = 2;
  	aux = 1;
	}else{
    choice = 2;
		arg = 1;
	}
	filen = argv[arg];
	choicename = argv[choice];
	char * quick = strstr(choicename, "quick");
	char * merge = strstr(choicename, "merge");
	if(quick){
		printf("... sorting %s file with quicksort\n", filen);
		if((nl = readlines(lptr, MLX, argv[arg])) >= 0){
			if(aux){
				quicksort((void**) lptr, 0, nl-1, (int (*) (void*, void*)) (numcmp));
			}else{
				quicksort((void**) lptr, 0, nl-1, (int (*) (void*, void*)) (strcmp));
			}
			if(aux){
				mergesort((void**) lptr, 0, nl-1, (int (*) (void*, void*)) (numcmp));
			}else{
				mergesort((void**) lptr, 0, nl-1, (int (*) (void*, void*)) (strcmp));
			}
			writelines(lptr, nl, filen);
			return 0;
		}else{
			printf("Input too big or to sort \n");
			return 1;
		}

	}else if (merge){
		printf("... sorting %s file with mergesort\n", filen);
		if((nl = readlines(lptr, MLX, argv[arg])) >= 0){
			if(aux){
				mergesort((void**) lptr, 0, nl, (int (*) (void*,void*)) (numcmp));
			}else{
				mergesort((void**) lptr, 0, nl, (int (*) (void*,void*)) (strcmp));
			}
			writelines(lptr, nl, filen);
			return 0;
		}else{
			printf("Input too big or to sort \n");
			return 1;
		}
	}else{
		printf("ERROR, please check the arguments used. \n");
		return -1;
	}
	return 0;
}
