# set this variable to the director in which you saved the common files
commondir = ../common/

all : main mac

main : main.c terrain.c user.c lightball.c flashlight.c gameoverhandler.c lightballhandler.c particles.c gun.c laser.c particlegenerator.c flarehandler.c flare.c laserhandler.c enemy.c enemyhandler.c collisionhandler.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o main -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES main.c terrain.c  gameoverhandler.c user.c lightball.c  flashlight.c lightballhandler.c particles.c gun.c laser.c particlegenerator.c flarehandler.c flare.c laserhandler.c enemy.c enemyhandler.c collisionhandler.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm -g


mac : main.c terrain.c user.c lightball.c flashlight.c gameoverhandler.c lightballhandler.c particles.c gun.c laser.c particlegenerator.c flarehandler.c flare.c laserhandler.c enemy.c enemyhandler.c collisionhandler.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m
	gcc -Wall -o mac -I$(commondir) -I../common/Mac -DGL_GLEXT_PROTOTYPES main.c terrain.c user.c lightball.c flashlight.c gameoverhandler.c lightballhandler.c particles.c gun.c laser.c particlegenerator.c flarehandler.c flare.c laserhandler.c enemy.c enemyhandler.c collisionhandler.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m -framework OpenGL -framework Cocoa -lm -g -Wno-deprecated-declarations

clean :
	rm  main mac
