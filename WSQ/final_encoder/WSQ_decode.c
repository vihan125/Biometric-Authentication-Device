#include <stdio.h>
#include <stdlib.h>
#include "wsq.h"
int main(){
    //get encoded data and store
    unsigned char* odata2;
    int olen2;
    int ow ;
    int oh ;
    int od ;
    int ppi;
    int lossyflag;

    if(!wsq_decode_mem(&odata2,&ow,&oh,&od,&ppi,&lossyflag,odata,olen)){
        printf("decoded !!");
    }

    FILE * filePointer;
    filePointer=fopen("result.txt","w");
    for(int r=0;r<307200;r++){
        fprintf(filePointer,"%d\n",odata2[r]);
    }
    fclose(filePointer);
}
