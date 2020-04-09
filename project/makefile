# set this variable to the director in which you saved the common files
commondir = ../common/

all : testmall main

testmall : testmall.c terrain.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o testmall -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES testmall.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm

main : main.c terrain.c user.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o main -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES main.c terrain.c user.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm -g



clean :
	rm  testmall main
