# Makefile for libconf

CC=clang
CFLAGS=-c -Wall -Wextra -pedantic -fPIC
LDFLAGS=-shared
SRC_DIR=source
INC_DIR=include
OBJ_DIR=build/obj
LIB_DIR=build/lib
BIN_DIR=build/bin

SOURCES=$(wildcard $(SRC_DIR)/*.c)
OBJECTS=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
HEADERS=$(wildcard $(INC_DIR)/*.h)
LIBRARY=$(LIB_DIR)/libconf.so

.PHONY: all clean install examples run_examples tests format analyze

all: $(LIBRARY)

$(LIBRARY): $(OBJECTS)
	mkdir -p $(LIB_DIR)
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(LIB_DIR) $(BIN_DIR)

tests:
	mkdir -p $(BIN_DIR)
	$(CC) -Wall -Wextra -pedantic -I$(INC_DIR) -lcmocka tests/test_libconf.c source/libconf.c -o $(BIN_DIR)/test_libconfig
	cd $(BIN_DIR) && ./test_libconfig

examples:
	mkdir -p $(BIN_DIR)
	$(CC) -Wall -Wextra -pedantic -I$(INC_DIR) examples/example-1.c -lconf -o $(BIN_DIR)/example
	cp examples/example-1.conf $(BIN_DIR)/example.conf

run_examples:
	$(BIN_DIR)/example $(BIN_DIR)/example.conf

install: $(LIBRARY)
	install -D -m 644 $(HEADERS) $(DESTDIR)/usr/include/
	install -D -m 755 $(LIBRARY) $(DESTDIR)/usr/lib/
	ldconfig

uninstall:
	rm -f $(addprefix $(DESTDIR)/usr/include/, $(notdir $(HEADERS)))
	rm -f $(addprefix $(DESTDIR)/usr/lib/, $(notdir $(LIBRARY)))

format:
	clang-format --style=file -i $(shell find . -type f \( -name "*.c" -o -name "*.h" \))

analyze:
	clang-tidy $(shell find . -type f \( -name "*.c" -o -name "*.h" \)) --format-style=file -checks=-*,clang-analyzer-*,-clang-analyzer-cplusplus*,portability-*,readability-*
