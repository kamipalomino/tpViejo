#GLOBALES
RM := rm -rf
CC=gcc

H_SRCS=$(shell find ./include -iname "*.h" | tr '\n' ' ')
C_SRCS=$(shell find ./src -type f -iname "*.c" -exec basename {} \;)
OBJS=$(C_SRCS:./%.c=build/%.o)
LINK = -lcommons -lpthread -lreadline -lm -lbiblio
CFLAGS = -c

# Add debug parameters and compile
debug: CC += -DDEBUG -g
debug: all

# Clean build files
clean:
	$(RM) build

install: all
	mkdir /usr/include/matelib
	cp -u Release/libmatelib.so /usr/lib
	cp --parents -u $(H_SRCS) /usr/include/matelib

uninstall:
	rm -f /usr/lib/libmatelib.so
	rm -rf /usr/include/matelib

.PHONY: all create-dirs clean install uninstall
	