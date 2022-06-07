g++ -lGL -lGLU -lglut -c main.cpp
g++ -lGL -lGLU -lglut -c class/rubik.cpp

g++ -o main -lGL -lGLU -lglut main.o rubik.o

rm *.o

./main