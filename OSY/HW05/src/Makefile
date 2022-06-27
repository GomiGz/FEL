CC=gcc
CFLAGS=-std=gnu99 -Wall -g -pthread

SRC_DIR=src
OBJ_DIR=obj
SRC_FILES=$(wildcard $(SRC_DIR)/*.c)
OBJ_FILES=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))
DEP_FILES=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.d,$(SRC_FILES))

prod-cons: $(OBJ_DIR) $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $(OBJ_FILES)

$(OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -MMD -c -o $@ $<

-include $(DEP_FILES)

zip:
	zip prod-cons.zip $(SRC_DIR)/* Makefile

clean:
	rm -f prod-cons.zip
	rm -f prod-cons
	rm -f -r $(OBJ_DIR)
