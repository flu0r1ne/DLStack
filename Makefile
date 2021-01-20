CC = gcc
CFLAGS = -Wall

PREFIX=/usr

OBJECTS = dls.o

OBJECTS += arg_parser.o
OBJECTS += config_parser.o
OBJECTS += dl_stack.o
OBJECTS += subcommands.o

OBJECTS += tomlc99/toml.o

OBJECTS += lib/handle.o
OBJECTS += lib/time_delta.o
OBJECTS += lib/left_pad.o
OBJECTS += lib/fs_utils.o

TARGET = dls
PUSH_TARGET=dls_push

SOURCES = $(patsubst %.o, %.c, OBJECTS)

%.o: $(SOURCES)
	$(CC) $(CFLAGS) $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	$(RM) $(OBJECTS)
	$(RM) $(TARGET)

install:
	chmod 551 $(TARGET)
	mv $(TARGET) $(PREFIX)/bin
	ln -s $(PREFIX)/bin/$(TARGET) $(PREFIX)/bin/$(PUSH_TARGET)
