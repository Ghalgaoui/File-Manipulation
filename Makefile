CC=gcc
CFLAGS=-Wall -Werror
files = original.txt copy.txt
bin = file_manipulation

$(bin): file_manipulation.c
	$(CC) $(CFLAGS) -o file_manipulation file_manipulation.c

execute:
	./file_manipulation

clean:
	rm -rf *.o $(files)
