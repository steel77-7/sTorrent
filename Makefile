CC = g++
SRC = src

client: $(SRC)/client.cpp
	$(CC) $(SRC)/client.cpp -o client

tracker: $(SRC)/tracker.cpp
	$(CC) $(SRC)/tracker.cpp -o tracker

clean:
	rm -f client tracker
