#Project 2 Make File
#Kelvin Lu G01194210
#CS 262, Lab 205
# Project 2

CFLAGS = -g -Wall 

all: project2_klu21_205.c
	gcc $(CFLAGS) -o project2_klu21_205 project2_klu21_205.c
clean:
	rm project2_klu21_205

