all: control user
	gcc -o control control.o
	gcc -o user user.o

# control: control.o

#
# user: user.o


control: control.c
	gcc -c control.c

user: user.c
	gcc -c user.c

clean:
	rm -rf *.o

run:
	./program
