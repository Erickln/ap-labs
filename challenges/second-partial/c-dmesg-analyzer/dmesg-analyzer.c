#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <unistd.h>

#define REPORT_FILE "_report.txt"

//Erick Eduardo López Nava A01114283

//Implementar Hash Table
struct nlist{
    struct nlist *next;
    char *name;
    char *val;
};

unsigned hash(char *s){
    unsigned hv;
    for (hv=0; *s != '\0'; s++) {
        hv = *s + 31  *hv;
    }
    return hv % 500;
}

static struct nlist *hashtab[500];

struct nlist *lookup(char *s){
    struct nlist *np;

    for (np=hashtab[hash(s)]; np!=NULL; np=np->next) {
        if(strcmp(s, np->name)==0){
            return np;
        }
    }
    return NULL;
}

struct nlist *add(char *name, char *val){

    struct nlist *np;
    unsigned hv;

    if( (np=lookup(name) )==NULL){
        np = (struct nlist *) malloc(sizeof(*np));
        if(np == NULL || (np->name = strdup(name))==NULL){
         return NULL; 
         }
        hv= hash(name);
        np->next=hashtab[hv];
        hashtab[hv]=np;
    }else{
        // printf("nlistfinded\n");
    }

    if(!np->val){
        np->val = (char*) malloc(strlen(val)*sizeof(char)+6);
        strcpy(np->val, "    ");
        strcat(np->val, val);
        strcat(np->val, "\n");
    }else{
        if(strstr(np->val, val)!=NULL){
         return np;
        }
        char *tmp = (char*) malloc(strlen(np->val)*sizeof(char)+1);
        strcpy(tmp, np->val);
        np->val = (char*) malloc( (strlen(tmp) + strlen(val))  *sizeof(char) + 6);
        strcpy(np->val, tmp);
        strcat(np->val, "    ");
        strcat(np->val, val);
        strcat(np->val,"\n");
    }
    
    return np;
}

//Helpers
struct descAndKey{
    char *name;
    char *desc;
};

struct descAndKey descriptionAndKey(char *str){

    struct descAndKey res;

    char *timeStamp;
    int length = 0;

    char *current = str;
    while(*current){
        length++;
        current++;
        if(*(current-1)==']') break;
    }
    if(length<1){
        res.name="General:";
        res.desc= (char*) malloc(strlen(str)*sizeof(char)+1);
        strcpy(res.desc, str);
        return res;
    };
    timeStamp = (char*) malloc(length*sizeof(char));
    strncpy(timeStamp, str, length);

    int keyLength = 0;
    int trailingS = 0;
    int colonFound = 0;
    while(*current){
        if(keyLength==0 && *current==' ') {
            trailingS++;
        }else if(colonFound && *current==' '){
            break;
        }else if(*current==':' && !colonFound){
            colonFound=1;
            keyLength++;
        }else{
            keyLength++;
        }
        current++;

    }

    char *auxKey;
    char *lastD;
    if(colonFound){
        auxKey=(char*) malloc(keyLength*sizeof(char));
        strncpy(auxKey,str+trailingS+length,keyLength);

        lastD = (char*) malloc(strlen(str)*sizeof(char) - trailingS - keyLength + 1);
        strcpy(lastD, timeStamp);
        strcat(lastD, current);
        //printf("Found %s\n", lastD);
    }else{
        auxKey = (char*) malloc(strlen("General:")*sizeof(char)+1);
        strcpy(auxKey, "General:");
        lastD = (char*) malloc(strlen(str)*sizeof(char)+1);
        strcpy(lastD, str);
    }
    res.name=auxKey;
    res.desc=lastD;
    return res;

}

//Analizer
void analizeLog(char *logFile, char *report);

int main(int argc, char **argv) {

    if (argc < 2) {
	printf("Usage:./dmesg-analizer logfile.txt\n");
	return 1;
    }

    //Generate resOutput file name

    char *reportName = (char*) malloc(strlen(argv[1])-4+strlen(REPORT_FILE)+1);
    strncpy(reportName, argv[1], strlen(argv[1])-4);
	strcat(reportName, REPORT_FILE);

    //Run analyzer
    analizeLog(argv[1], reportName);

    return 0;
}

void analizeLog(char *logFile, char *report) {
    printf("Generating Report from: [%s] log file\n", logFile);

    int file = open(logFile, O_RDONLY);
	if (file == -1){
        printf("Error reading the log file\n");
        return;
    }

	int file_len = 0;
	char buff;
    int bytes_read;
    int newLine = 1;
	do {
        
        bytes_read = read(file, &buff, 1);
        if(newLine && bytes_read>0) {
            file_len++;
            newLine=0;
        }
        if(buff=='\n') newLine=1;
    } while (bytes_read>0);

    // printf("Analyzing: %d lines\n",file_len);
    lseek(file,0,SEEK_SET);
    int lineLen = 0;
    do{
        bytes_read = read(file, &buff, 1);
        if(bytes_read>0){
            if(buff =='\n'){
                if(lineLen<1) {
                    continue;
                }

                lineLen++;
                lseek(file,-lineLen,SEEK_CUR);
                char *lineBuff = (char*) malloc(lineLen*sizeof(char));
                read(file,lineBuff,lineLen);
                lineBuff[strcspn(lineBuff, "\n")] = 0;
                struct descAndKey res = descriptionAndKey(lineBuff);
                add(res.name,res.desc);
                lineLen=0;
            }else{
                lineLen++;
            }
        }
    } while(bytes_read>0);

    close(file);
    //Iterate before clos
    //printf("Close done\n")
    FILE *resOutput = fopen(report, "w+");
    struct nlist *start;

    for (int i=0; i<=500; i++) {
        start=hashtab[i];
        if(start==NULL) continue;
        struct nlist *current;
        for (current=start; current!=NULL; current=current->next) {
            fputs(current->name, resOutput);
            fputs(current->val, resOutput);
            fputs("\n", resOutput);
        }   
    }
    //print("Before Close\n")
    fclose(resOutput);

    printf("Report is generated at: [%s]\n", report);
}