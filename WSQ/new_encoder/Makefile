# -*- MakeFile -*-

eye:eye.o encoder.o huff.o tableio.o tree.o util.o dataio.o nistcom.o globals.o decoder.o ppi.o computil.o 
	gcc eye.o encoder.o huff.o tableio.o tree.o util.o dataio.o nistcom.o globals.o decoder.o ppi.o computil.o  -o eye 

lena2.o:lena2.c wsq.h
	gcc -c lena2.c

lena.o:lena.c wsq.h 
	gcc -c lena.c 

eye.o:eye.c wsq.h
	gcc -c eye.c

rose.o:rose.c wsq.h
	gcc -c rose.c 

encoder.o:encoder.c wsq.h dataio.h
	gcc -c encoder.c

	
huff.o:huff.c dataio.h huff.h 
	gcc -c huff.c

tableio.o:tableio.c wsq.h dataio.h computil.h
	gcc -c tableio.c

tree.o:tree.c wsq.h
	gcc -c tree.c
	
util.o:util.c wsq.h dataio.h 
	gcc -c util.c -l m 
	
dataio.o:dataio.c dataio.h
	gcc -c dataio.c 

nistcom.o:nistcom.c nistcom.h
	gcc -c nistcom.c
	
globals.o:globals.c wsq.h 
	gcc -c globals.c
	
decoder.o:decoder.c wsq.h dataio.h
	gcc -c decoder.c

ppi.o:ppi.c wsq.h
	gcc -c ppi.c
	
computil.o:computil.c dataio.h computil.h 
	gcc -c computil.c 
	
clear:
	rm -f *.o main