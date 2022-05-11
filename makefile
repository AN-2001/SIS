CC := gcc
flags := -Wall -Werror -std=c99 -g
obj := SIS.o alu.o

all: SIS

SIS: main.c $(obj)
	$(CC) main.c $(obj) $(flags) -o SIS

%.o: %.c %.h
	$(CC) -c $< $(flags) -o $@

clean:
	rm *.o
