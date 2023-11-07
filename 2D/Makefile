OPTS := -O0

CC = g++

LANGUAGE = cpp

BIN_DIR = bin
TARGET = $(BIN_DIR)/game

SRCS := $(wildcard *.$(LANGUAGE))

OBJS := $(addprefix $(BIN_DIR)/,$(SRCS:.$(LANGUAGE)=.o))

CFLAGS := $(OPTS) -g -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-function -Wno-unused-value
LDLIBS := -lGL -lSOIL -lGLEW -lGLU -lglut -lm -lX11 -lXrandr -lpthread -lfreetype

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

$(BIN_DIR)/%.o: %.$(LANGUAGE) | bin
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@

bin: 
	mkdir -p bin

run: $(TARGET)
	$<

gdb: $(TARGET)
	gdb $<

clean:
	rm -rf $(BIN_DIR)/


