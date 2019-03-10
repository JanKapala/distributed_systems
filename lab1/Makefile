# Sources' names
CLIENT = client
TCP_COMMUNICATION = tcp_communication
UDP_COMMUNICATION = udp_communication
MESSAGES = messages


#compilator's flags
FLAGS = -Wall

all: $(CLIENT).out

$(CLIENT).out: src/$(CLIENT).c src/$(TCP_COMMUNICATION).c src/$(UDP_COMMUNICATION).c src/$(MESSAGES).c
	gcc $(FLAGS) src/$(CLIENT).c src/$(TCP_COMMUNICATION).c src/$(UDP_COMMUNICATION).c src/$(MESSAGES).c -o $(CLIENT).out


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