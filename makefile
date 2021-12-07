all: control user

control: control.o
	gcc -o control control.o

write: write.o
	gcc -o user user.o

write.o: user.c
	gcc -c user.c

control.o: control.c
	gcc -c control.c

clean:
	rm *.o
