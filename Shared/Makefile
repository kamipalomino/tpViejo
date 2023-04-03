#GLOBALES
RM=rm -rf
CC=gcc

C_SRCS=$(shell find ./src -type f -iname "*.c" -exec basename {} \;)
H_SRCS=$(shell find ./include -iname "*.h" | tr '\n' ' ')
OBJS=$(C_SRCS:./%.c=build/%.o)
LINK = -lcommons -lpthread -lreadline -lm

# Add debug parameters and compile
debug: CC += -DDEBUG -g
debug: all

# Clean build files
clean:
	$(RM) build

install: all
	mkdir /usr/include/biblio
	cp -u Release/libbiblio.so /usr/lib
	cp --parents -u $(H_SRCS) /usr/include/biblio

uninstall:
	rm -f /usr/lib/libbiblio.so
	rm -rf /usr/include/biblio

.PHONY: all create-dirs clean install uninstall
	