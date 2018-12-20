CLI_SRC=client/snowcast_control.c
LIST_SRC=client/snowcast_listener.c
SERV_SRC=server/snowcast_server.c
PROTOC_SRC=protoc/* network/*
DEBUG_SRC=debug/*

FLAGS=-g -Wall -Wpedantic
DEBUG_FLAGS=-D DEBUG

BINS=snowcast_control snowcast_listener snowcast_server
DEBUG_RULES=snowcast_control_dbg snowcast_listen_dbg snowcast_server_dbg

all: $(BINS)
dbgall: $(DEBUG_RULES)

snowcast_control: $(CLI_SRC) $(PROTOC_SRC) $(DEBUG_SRC)
	gcc $(FLAGS) -o $@ $^

snowcast_listener: $(LIST_SRC) $(PROTOC_SRC) $(DEBUG_SRC)
	gcc $(FLAGS) -o $@ $^

snowcast_server: $(SERV_SRC) $(PROTOC_SRC) $(DEBUG_SRC)
	gcc $(FLAGS) -o $@ $^

snowcast_control_dbg: $(CLI_SRC) $(PROTOC_SRC) $(DEBUG_SRC)
	gcc $(DEBUG_FLAGS) $(FLAGS) -o snowcast_control $^

snowcast_listen_dbg: $(LIST_SRC) $(PROTOC_SRC) $(DEBUG_SRC)
	gcc $(DEBUG_FLAGS) $(FLAGS) -o snowcast_listener $^

snowcast_server_dbg: $(SERV_SRC) $(PROTOC_SRC) $(DEBUG_SRC)
	gcc $(DEBUG_FLAGS) $(FLAGS) -o snowcast_server $^

# m = my
# r = reference
# c = snowcast_control
# l = snowcast_listener
# s = snowcast_server
mcmlrs: snowcast_control snowcast_listener
	./snowcast listener 2345
	./reference/snowcast_server 1234
	./snowcast_control localhost 1234 2345

clean:
	rm -f $(BINS)

