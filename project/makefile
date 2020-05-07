# set this variable to the director in which you saved the common files
commondir = ../common/

all : testmall main mac

testmall : testmall.c terrain.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o testmall -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES testmall.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm

main : main.c terrain.c user.c lightball.c flashlight.c lightballhandler.c particles.c gun.c laser.c particlegenerator.c flarehandler.c flare.c laserhandler.c enemy.c enemyhandler.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o main -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES main.c terrain.c user.c lightball.c  flashlight.c lightballhandler.c particles.c gun.c laser.c particlegenerator.c flarehandler.c flare.c laserhandler.c enemy.c enemyhandler.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm -g


mac : main.c terrain.c user.c lightball.c flashlight.c lightballhandler.c particles.c particlegenerator.c flarehandler.c flare.c gun.c laser.c enemy.c enemyhandler.c laserhandler.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m
	gcc -Wall -o mac -I$(commondir) -I../common/Mac -DGL_GLEXT_PROTOTYPES main.c terrain.c user.c lightball.c flashlight.c lightballhandler.c particles.c particlegenerator.c flarehandler.c flare.c gun.c laser.c enemy.c enemyhandler.c laserhandler.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m -framework OpenGL -framework Cocoa -lm -g -Wno-deprecated-declarations

clean :
	rm  testmall main mac
