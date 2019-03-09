# Sources' names
CLIENT = client
TCP_COMMUNICATION = tcp_communication
MESSAGES = messages


#compilator's flags
FLAGS = -Wall

all: $(CLIENT).out

$(CLIENT).out: $(CLIENT).c $(TCP_COMMUNICATION).c $(MESSAGES).c
	gcc $(FLAGS) $(CLIENT).c $(TCP_COMMUNICATION).c $(MESSAGES).c -o $(CLIENT).out


# test: all
# 	./server.out &

# 	./client.out commands1.txt &
# 	./client.out commands1.txt &
# 	./client.out commands1.txt &

# 	./client.out commands2.txt &

.PHONY: clean
clean:
	-rm -f $(CLIENT).out

rebuild: clean all