#include <stdio.h>
#include <stdlib.h>
#include "wsq.h"
int debug =1;
//get the plain text length from file

//put plain text data into a memory
int getc_len_text(char name []){
    char ch;
    FILE *fp;
    fp = fopen(name, "r");// read mode
    int len=0;//to get length of characters in file

    //error while opening text file
    if (fp == NULL)
    {
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
    }
    //get character length of text file
    while((ch = fgetc(fp)) != EOF){
            if (ch == ','){
                len++;
            }
    }
    fclose(fp);

    return len;
}

int putc_data(char name [],int len,unsigned char **odata){
    char ch;
    unsigned char *pixel =(unsigned char *)malloc(3);
    int p=0;
    int d=0;
    FILE *fp;
    fp = fopen(name, "r");
    unsigned char *fdata=(unsigned char *)malloc(len*sizeof(unsigned char));
    int i =0;
    char caps;

    //error while opening text file
    if (fp == NULL)
    {
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
    }
    //make characters uppercase and put into buffer
    while((ch = fgetc(fp)) != EOF){
            if(ch == ','){
                int d=atoi(pixel);
                fdata[i]=d;
                for (int r=0;r<3;r++){
                    pixel[r]=0;
                }
                p=0;
                i++;

            }
            else{
                pixel[p]=ch;
                p++;}
    }
    //assign pointer of fdata to odata
    *odata=fdata;

    return 0;


}

int main(void) {
        //opening file and getting length of total characters
    char name[] ="decode.txt";
    unsigned char *data;
    int length;
    //file_len=getc_len_text(name);
    //storing data in the buffer
    length=getc_len_text(name);
    printf("%d\n",length);
    putc_data(name,length,&data);
    printf("got data");
    printf("%d",data[length]);

    unsigned char* odata2;
    int olen2;
    int ow ;
    int oh ;
    int od ;
    int ppi;
    int lossyflag;

    if(!wsq_decode_mem(&odata2,&ow,&oh,&od,&ppi,&lossyflag,data,length)){
        printf("decoded !!");
    }

    FILE * filePointer;
    filePointer=fopen("result.txt","w");
    for(int r=0;r<307200;r++){
        fprintf(filePointer,"%d\n",odata2[r]);
    }
    fclose(filePointer);



}
