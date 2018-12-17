CLI_SRC=client/client.c
LIST_SRC=client/listener.c
SERV_SRC=server/server.c
PROTOC_SRC=protoc/* network/*

FLAGS=-g -Wall -Wpedantic

BINS: client listener server

client: $(CLI_SRC) $(PROTOC_SRC)
	gcc $(FLAGS) -o $@ $^

listener: $(LIST_SRC) $(PROTOC_SRC)
	gcc $(FLAGS) -o $@ $^

server: $(SERV_SRC) $(PROTOC_SRC)
	gcc $(FLAGS) -o $@ $^

clean:
	rm -rf $(BINS)

