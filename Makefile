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

SRC = thinfat32.c fat32_ui.c tests.c
OBJS = $(SRC:%.c=$(OBJ_DIR)/%.o)

VPATH = $(SRC_DIR)
all: begin tests end

begin:
	@echo 
	@echo "Starting Build..."
	@echo "-----------------"
	mkdir -p $(SRC_DIR)
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BUILD_DIR)
	mkdir -p $(DIST_DIR)
	@echo
end:
	@echo 
	@echo "Done."
	@echo

$(OBJS) : $(OBJ_DIR)/%.o : %.c
	@echo
	@echo "Compiling C File" $<
	@echo "----------------"
	$(CC) -I$(INCLUDES) -c $(CFLAGS) $< -o $@ 
	@echo

# tests : CFLAGS += -D TF_DEBUG -D DEBUG
tests : $(OBJS)
	@echo
	@echo "Creating Test Fixture"
	@echo "---------------------"
	$(CC) $(CFLAGS) $(OBJS)  --output $(BUILD_DIR)/$@ $(LDFLAGS)
	@echo
clean :  
	@echo
	@echo "Cleaning the Build"
	@echo "------------------"	
	rm -rf $(OBJS) core $(BUILD_DIR)/* $(DIST_DIR)/fat32.tar.gz
	@echo

test : tests
	@echo
	@echo "Running Tests..."
	@echo "----------------"
	make rebuild
	./build/tests
	@echo

dist : clean
	tar -cv src scripts Makefile | gzip -c > $(DIST_DIR)/fat32.tar.gz
	@echo
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

rebuild: unmount create mount populate unmount

debug: tests
	@echo
	@echo "Starting Debugger..."
	@echo "--------------------"
	$(DEBUGGER) ./build/tests
	@echo

.PHONY : begin end clean create mount unmount populate rebuild debug
