#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// adds/subtracts/multiplies all values that are in the *values array.
// nValues is the number of values you're reading from the array
// operator will indicate if it's an addition (1), subtraction (2) or
// multiplication (3)
long calc(int operator, int nValues, int *values) {
    //char valuess=*values;
    int res=0;
    char charRes[nValues*2+1];
    //printf("\nOperator: %i , nValues: %i \n",operator,nValues);
    if(operator==1){
        for (int i = 0; i < nValues; i++){
            res+=values[i];
            if(i!=nValues-1){
                printf("%i + ",values[i]);
            }else{
                printf("%i ",values[i]);
            }
        }
    }
    if(operator==2){
        res=values[0];
        printf("%i - ",values[0]);
        for(int i = 1; i < nValues; i++){
            res-=values[i];

            if(i!=nValues-1){
                printf("%i - ",values[i]);
            }else{
                printf("%i ",values[i]);
            }
        }
    }
    if(operator==3){
        res=1;
        for (int i = 0; i < nValues; i++){
            res*=values[i];
            if(i!=nValues-1){
                printf("%i * ",values[i]);
            }else{
                printf("%i ",values[i]);
            }
        }
    }

    printf(" = ");
    //printf("\n%s",charRes);
    printf("%i\n",res);

    return 0;
}

int main(int argc, char **argv[]) {
    if(argc < 3){
        printf("Not enough data \n");
        return -1;
    }
    //printf("Initialize");
    int in = 0;
    int counter;
    char* aux= argv[1];
    int operator = 0;
    char *next;
    long curr;
    int *values = malloc(sizeof(int) * (argc - 2));;
    for(int i = 2; i < argc; i++){
       curr = strtol(argv[i], &next, 10);
       if(*next != '\0'){
           printf("Error\n");
           return -1;
       }else{
           values[in] = (int)curr;
           in++;
       }
    }
    //int nValues = argc-2;

    if(argc==1)
        printf("\nNo Extra Command Line Argument Passed Other Than Program Name");
    if(argc>3){
        if(strcmp(aux,"add")==0){
            operator=1;
        }else if(strcmp(aux,"sub")==0){
            operator=2;
        }else if(strcmp(aux,"mult")==0){
            operator=3;
        }else{
            printf("\n Identificador inválido");
            return 0;
        }
        calc(operator,argc-2,values);
  
    }

    return -1;
}
