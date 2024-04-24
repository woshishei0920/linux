

CC=gcc
FLAGS=-I ./NtyCo/core/ -L ./NtyCo/ -l ntyco
SRCS = kvstore.c netyco_entry.c  epoll_entry.c kvstore_array.c
TARGET=kvstore
SUBDIR=./NtyCo/

OBJS=$(SRCS:.c=.o)


all:$(SUBDIR) $(TARGET)


$(SUBDIR):ECHO
	make -C $@

ECHO:
	@echo $(SUBDIR)
	
$(TARGET):$(OBJS)
	$(CC) -o $@ $^  $(FLAGS)

%.o:%.c
	$(CC) $(FLAGS) -c $^ -o $@


clean:
	rm -rf $(OBJS) $(TARGET)
