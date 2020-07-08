#include <stdio.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "wsq.h"
#include "stb_image.h"
int debug =1;

int main(void) {

    int width,height,channels;
    unsigned char *img = stbi_load("image.jpg",&width,&height,&channels,0);
    unsigned char *gray_img=(unsigned char *)malloc(640 * 480 * sizeof(unsigned char));
    if(img == NULL){
        printf("Error in loading image\n");
        exit(1);
    }
    else{
        int pointer=0;
        for(int i=0;i<921600;i=i+3){
            unsigned char dot =(0.3*img[i])+(0.59*img[i+1])+(0.11*img[i+2]);
            if (dot == '0'){
                dot=10;
                gray_img[pointer] = dot;
                pointer++;
            }
            else{
                gray_img[pointer] = dot;
                pointer++;
        }
        }
    }



    unsigned char* odata;
    int olen;
    float bitrate = 2.25;
    char comment;

    if(!wsq_encode_mem(&odata,&olen,bitrate,gray_img,640,480,8,-1,&comment)){
        printf("worked\n");
        printf("%d\n",olen);
    }


    FILE * filePointer;
    filePointer=fopen("encode.txt","w");
    for(int r=0;r<olen;r++){
        fprintf(filePointer,"%d,",odata[r]);
    }
    fclose(filePointer);


 return 0;


}

