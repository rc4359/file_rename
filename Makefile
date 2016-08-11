

all: file_rename

OBJS = main.o

clean:
	rm -f *.elf *.gdb *.o *~ *.a .depend cscope*  file_rename include/*~


file_rename: $(OBJS)
	@echo 'CC $@'
	@$(CC)  $(CFLAGS) $(INCLUDES) -o $@ $? $(LIBS) -lpthread


