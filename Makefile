
#--- WARNING: NO USER EDITABLE CODE ABOVE THIS LINE ---#

CFLAGS = -g -fpack-struct
LIBS =
LDFLAGS = 
INCLUDES = ./src/include 

SRC_DIR = ./src
OBJ_DIR = ./obj
BUILD_DIR = ./build
DIST_DIR = ./dist
CC = gcc
LD = ld
DEBUGGER = ddd

SRC = main.c thinfat32.c fat32_ui.c
OBJS = $(SRC:%.c=$(OBJ_DIR)/%.o)

VPATH = $(SRC_DIR)
all: begin main end

begin:
	@echo 
	@echo "Starting Build..."
	@echo "-----------------"
	mkdir -p $(SRC_DIR)
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BUILD_DIR)
	mkdir -p $(DIST_DIR)
end:
	@echo 
	@echo "Done."
	@echo

$(OBJS) : $(OBJ_DIR)/%.o : %.c
	@echo
	@echo "Compiling C File" $<
	@echo "----------------"
	$(CC) -I$(INCLUDES) -c $(CFLAGS) $< -o $@ 

main : $(OBJS)
	@echo
	@echo "Creating Executable"
	@echo "-------------------"
	$(CC) $(CFLAGS) $(OBJS) --output $(BUILD_DIR)/$@ $(LDFLAGS)
clean:  
	@echo
	@echo "Cleaning the Build"
	@echo "------------------"	
	rm -rf $(OBJS) core fs $(BUILD_DIR)/* $(DIST_DIR)/fat32.tar.gz

dist: clean
	tar -cv src scripts Makefile | gzip -c > $(DIST_DIR)/fat32.tar.gz

#
# Targets for creating and manipulating the test filesystem
#
create:
	sudo ./scripts/makefs make

mount:
	sudo ./scripts/makefs mount

unmount:
	sudo ./scripts/makefs unmount

populate:
	sudo ./scripts/makefs populate

rebuild:
	make unmount; make create; make mount; make populate; make unmount;

.PHONY : begin end clean create mount unmount populate rebuild debug
