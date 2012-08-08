DIRECTORIES = -ICabbageCore -ICabbageScene -ICabbageFramework
OPENGL = -lGL -lGLU `sdl-config --libs`

CPP = g++ -O1 -s
CPPFLAGS = $(DIRECTORIES) $(OPENGL)

all: lab4

lab4: scene framework Main.o
	$(CPP) $(CFLAGS) CabbageFramework/*.o CabbageScene/*.o CabbageScene/ImageLoaders/*.o Main.o -o $@ $(OPENGL)

Main.o: Main.cpp

scene:
	@cd CabbageScene && $(MAKE)

framework:
	@cd CabbageFramework && $(MAKE)

depend:
	@cd CabbageScene && $(MAKE) depend
	@cd CabbageFramework && $(MAKE) depend

clean:
	rm -f Main.o lab4
	@cd CabbageScene && $(MAKE) clean
	@cd CabbageFramework && $(MAKE) clean

remake:
	rm -f Main.o lab4
	@cd CabbageScene && $(MAKE) remake
	@cd CabbageFramework && $(MAKE) remake

cleanish:
	rm -f Main.o
	@cd CabbageScene && $(MAKE) cleanish
	@cd CabbageFramework && $(MAKE) cleanish

