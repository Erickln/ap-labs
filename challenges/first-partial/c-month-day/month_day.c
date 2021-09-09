#include <stdio.h>
#include <stdlib.h>
#define monDay {{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};
static char dayMon[2][13] = monDay;

/* month_day function's prototype*/
void month_day(int year, int yearday, int *pmonth, int *pday){
    int bisiesto = (year % 4 == 0) && (year % 100 != 0) || (year % 400 == 0);
    int monthdays;
    int monthNum = 0;
    
    monthdays = dayMon[bisiesto][2];
    while(yearday > monthdays){
        yearday -= monthdays;
        if (yearday<0) break;
        if (monthNum<0) break;
        monthdays = dayMon[bisiesto][(++monthNum)];
    }
    *pmonth = monthNum;
    *pday = yearday;    
}

int main(int argc, char **argv) {
    if(argc < 3 || argc > 3){
        printf("\nNo enough dara");
        return -1;
    }
    char *m;
    char *auxYear;
    char *auxDay;
    //int year        = atoi(argv[1]);
    //int yearday     = atoi(argv[2]);
    int year    = strtol(argv[1], &auxYear, 10);
    int yearday = strtol(argv[2], &auxDay, 10);

    static char *mm[] = {"No Month", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sept", "Oct", "Nov", "Dec"};

    if(yearday > 366){
        printf("\n El año no tiene más de 366 días\n");
        return 0;
    }
    else if(year < 0){
        printf("\nOnly numbers above 0 in year\n");
        return 0;
    }
    else if(*auxYear != '\0'){ 
        printf("\nSolo acepto valores numéricos para el año \n");
        return 0;
    }
    else if(*auxDay != '\0'){ 
        printf("\nSolo acepto valores numéricos para día \n");
        return 0;
    }
    
    int pmonth;
    int pday;

    month_day(year,yearday, &pmonth, &pday);
    
    if (pmonth>12 || pmonth <1){
        printf("\n Invalid month  DEBUG ERROR");

    }else{
        m=mm[pmonth+1];
    }

    printf("%s %02i, %i\n", m, pday, year);

    return 0;
}
