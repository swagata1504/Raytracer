# Raytracer
This is a program that was created for my Graphics class (CS410). Here I wrote code for a ray casting system capable of rendering the scenes described in the driver files that have the capability to perform reflection or recursively raytracing.

- Raytracer.h: Is the header file for all the classes used in the project
- Face.cc: Has the vectors of the points on each face
- raytracer.cc: reads in the driver file and calls other classes to apply changes to render the file
- Pixel.cc: Makes calculation for each pixels and generates a ppm file.
- Light.cc: Is a class that has the vectors that store the values of the position and emission
- Makefile: Compiles project

- After you are in the correct directory, type 'make' to compile and type './raytracer `<driverFile>`' to get an image.
