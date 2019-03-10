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

test_udp: all
	gnome-terminal --window -x bash -c "./client.out Client1 127.0.0.1 9001 127.0.0.1 9001 1 UDP;bash"
	gnome-terminal --window -x bash -c "./client.out Client2 127.0.0.1 9002 127.0.0.1 9001 0 UDP;bash"
	gnome-terminal --window -x bash -c "./client.out Client3 127.0.0.1 9003 127.0.0.1 9001 0 UDP;bash"

test_tcp: all
	gnome-terminal --window -x bash -c "./client.out Client1 127.0.0.1 9001 127.0.0.1 9001 1 TCP;bash"
	gnome-terminal --window -x bash -c "./client.out Client2 127.0.0.1 9002 127.0.0.1 9001 0 TCP;bash"
	gnome-terminal --window -x bash -c "./client.out Client3 127.0.0.1 9003 127.0.0.1 9001 0 TCP;bash"

	# gnome-terminal --window -x bash -c "./client.out Client1 127.0.0.4 9001 127.0.0.4 9001 0 TCP;bash"
	# gnome-terminal --window -x bash -c "./client.out Client2 127.21.13.1 9002 127.0.0.4 9001 1 TCP;bash"
	# gnome-terminal --window -x bash -c "./client.out Client3 127.1.234.5 9003 127.21.13.1 9002 0 TCP;bash"


.PHONY: clean
clean:
	-rm -f $(CLIENT).out

rebuild: clean all