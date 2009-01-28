CFLAGS = -g -fpack-struct
LIBS = 
INCLUDES = 

CC = gcc
LD = ld
DEBUGGER = ddd

OBJS = fat32.o fat32_ui.o main.o

.c.o:
	${CC} ${CFLAGS} -c $<

fat32: ${OBJS} 
	${CC} ${CFLAGS} ${OBJS} -o fat32

clean: 
	rm -rf ${OBJS} core fs fat32 fat32.tar.gz
 
create:
	sudo ./makefs make

mount:
	sudo ./makefs mount

unmount:
	sudo ./makefs unmount

populate:
	sudo ./makefs populate

rebuild:
	make unmount; make create; make mount; make populate; make unmount;

dist: clean
	tar -cv *.c *.h Makefile makefs | gzip -c > fat32.tar.gz
debug:
	${DEBUGGER} ./fat32 &
	
.PHONY : clean create mount unmount populate rebuild debug
