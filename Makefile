CLI_SRC=client/control/*.c
LIST_SRC=client/listener/*.c
SERV_SRC=server/*.c
COMMON_SRC=common/*.c
DEBUG_SRC=debug/*

FLAGS=-g -Wall -Wpedantic -pthread
DEBUG_FLAGS=-D DEBUG

BINS=snowcast_control snowcast_listener snowcast_server
DEBUG_RULES=snowcast_control_dbg snowcast_listener_dbg snowcast_server_dbg

all: $(BINS)
alldbg: $(DEBUG_RULES)

snowcast_control: $(CLI_SRC) $(COMMON_SRC) $(DEBUG_SRC)
	gcc $(FLAGS) -o $@ $^

snowcast_listener: $(LIST_SRC) $(COMMON_SRC) $(DEBUG_SRC)
	gcc $(FLAGS) -o $@ $^

snowcast_server: $(SERV_SRC) $(COMMON_SRC) $(DEBUG_SRC)
	gcc $(FLAGS) -o $@ $^

snowcast_control_dbg: $(CLI_SRC) $(COMMON_SRC) $(DEBUG_SRC)
	gcc $(DEBUG_FLAGS) $(FLAGS) -o snowcast_control $^

snowcast_listener_dbg: $(LIST_SRC) $(COMMON_SRC) $(DEBUG_SRC)
	gcc $(DEBUG_FLAGS) $(FLAGS) -o snowcast_listener $^

snowcast_server_dbg: $(SERV_SRC) $(COMMON_SRC) $(DEBUG_SRC)
	gcc $(DEBUG_FLAGS) $(FLAGS) -o snowcast_server $^

mc: snowcast_control_dbg
	./snowcast_control localhost 1234 2345
ml: snowcast_listener_dbg
	./snowcast_listener 2345
ms: snowcast_server_dbg
	./snowcast_server 1234 mp3/*

clean:
	rm -f $(BINS)

