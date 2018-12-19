CLI_SRC=client/client.c
LIST_SRC=client/listener.c
SERV_SRC=server/server.c
PROTOC_SRC=protoc/* network/*

FLAGS=-g -Wall -Wpedantic
DEBUG_FLAGS=-D DEBUG

BINS: client listener server

client: $(CLI_SRC) $(PROTOC_SRC)
	gcc $(FLAGS) -o $@ $^

listener: $(LIST_SRC) $(PROTOC_SRC)
	gcc $(FLAGS) -o $@ $^

server: $(SERV_SRC) $(PROTOC_SRC)
	gcc $(FLAGS) -o $@ $^

client_debug: $(CLI_SRC) $(PROTOC_SRC)
	gcc $(DEBUG_FLAGS) $(FLAGS) -o $@ $^

listener_debug: $(LIST_SRC) $(PROTOC_SRC)
	gcc $(DEBUG_FLAGS) $(FLAGS) -o $@ $^

server_debug: $(SERV_SRC) $(PROTOC_SRC)
	gcc $(DEBUG_FLAGS) $(FLAGS) -o $@ $^

clean:
	rm -rf $(BINS)

