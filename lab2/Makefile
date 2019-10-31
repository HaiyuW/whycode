compile: client server

client: localclient.cpp
	g++ localclient.cpp -o localclient

server: localserver.cpp
	g++ localserver.cpp -o localserver

test_server: server client
	./localserver

test_client: client
	./localclient