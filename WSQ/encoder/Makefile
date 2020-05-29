# -*- MakeFile -*-

main:main.o encoder.o globals.o decoder.o huff.o tableio.o tree.o util.o huffs.o dataio.o nistcom.o computil.o jpeg_tableio.o strfet.o freefet.o filesize.o allocfet.o updatfet.o lkupfet.o delfet.o
	gcc main.o encoder.o globals.o decoder.o huff.o tableio.o tree.o util.o huffs.o dataio.o nistcom.o computil.o jpeg_tableio.o strfet.o freefet.o filesize.o allocfet.o updatfet.o lkupfet.o delfet.o -o main

main.o:main.c wsq.h
	gcc -c main.c

encoder.o:encoder.c wsq.h dataio.h
	gcc -c encoder.c

globals.o:globals.c wsq.h
	gcc -c globals.c
	
decoder.o:decoder.c wsq.h dataio.h
	gcc -c decoder.c
	
huff.o:huff.c wsq.h
	gcc -c huff.c

tableio.o:tableio.c wsq.h computil.h dataio.h swap.h defs.h
	gcc -c tableio.c

tree.o:tree.c wsq.h ihead.h 
	gcc -c tree.c
	
util.o:util.c wsq.h defs.h dataio.h 
	gcc -c util.c -l m 
	
huffs.o:huffs.c jpegl.h dataio.h
	gcc -c huffs.c

dataio.o:dataio.c ioutil.h dataio.h swap.h
	gcc -c dataio.c 

nistcom.o:nistcom.c ihead.h nistcom.h
	gcc -c nistcom.c

computil.o:computil.c computil.h dataio.h 
	gcc -c computil.c 
	
jpeg_tableio.o:jpeg_tableio.c jpegl.h computil.h dataio.h
	gcc -c jpeg_tableio.c 

strfet.o:strfet.c fet.h 
	gcc -c strfet.c 
	
freefet.o:freefet.c fet.h
	gcc -c freefet.c 
	
filesize.o:filesize.c 
	gcc -c filesize.c
	
allocfet.o:allocfet.c fet.h util.h 
	gcc -c allocfet.c 
	
updatfet.o:updatfet.c fet.h defs.h util.h 
	gcc -c updatfet.c 
	
lkupfet.o:lkupfet.c fet.h defs.h 
	gcc -c lkupfet.c 

delfet.o:delfet.c fet.h util.h 
	gcc -c delfet.c 
	
clear:
	rm -f *.o main