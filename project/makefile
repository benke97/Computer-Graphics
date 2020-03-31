# set this variable to the director in which you saved the common files
commondir = ../common/

all : lab4-5

lab4-5 : lab4-5.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o lab4-5 -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES lab4-5.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm


clean :
	rm lab4-5
