CPPFLAGS=-Wall -Werror -Wextra -Wfatal-errors
assignment=raytracer

$(assignment): $(assignment).o Pixel.o Light.o Model.o Face.o Raytracer.h 
	g++  -I ./Eigen/ $(CPPFLAGS) -O3 $(assignment).o Light.o Model.o Pixel.o Face.o -o raytracer

Light.o:: Raytracer.h
Model.o:: Raytracer.h
Pixel.o:: Raytracer.h
Face.o:: Raytracer.h
$(assignment).o::  $(assignment).cc Raytracer.h

tar:
	tar -cv $(MAKEFILE_LIST) *.cc README.txt Raytracer.h Eigen cube_centered.obj cube_centered.mtl driver00.txt driver01.txt driver03.txt>$(assignment).tar

clean:
	rm -f $(assignment) $(assignment).tar *.o *.gch
