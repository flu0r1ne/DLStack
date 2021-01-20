CC = clang
CFLAGS = -Wall -g

OBJECTS = dls.o
OBJECTS += arg_parser.o

OBJECTS += config_parser.o

OBJECTS += tomlc99/toml.o

OBJECTS += dl_stack.o
OBJECTS += subcommands.o
OBJECTS += utils.o

OBJECTS += lib/handle.o
OBJECTS += lib/time_delta.o
OBJECTS += lib/left_pad.o

TARGET = dls

SOURCES = $(patsubst %.o, %.c, OBJECTS)

%.o: $(SOURCES)
	$(CC) $(CFLAGS) $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	$(RM) $(OBJECTS)
	$(RM) $(TARGET)

INSTALL_LOC=/usr/bin
PUSH_TARGET=dls_push

install:
	chmod 551 $(TARGET)
	mv $(TARGET) $(INSTALL_LOC)
	ln -s $(INSTALL_LOC)/$(TARGET) $(INSTALL_LOC)/$(PUSH_TARGET)
