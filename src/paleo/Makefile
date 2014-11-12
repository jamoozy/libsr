.PHONY : all

c_files:=$(shell ls *.c)
o_files:=$(c_files:%.c=%.o)

all : $(o_files)

%.o : %.c %.h
	gcc -std=c99 -c -o $@ $<
