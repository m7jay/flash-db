server:
	g++ -Wall -Wextra -O2 -g server.cpp -o server
	./server
client:
	g++ -Wall -Wextra -O2 -g client.cpp -o client
	./client