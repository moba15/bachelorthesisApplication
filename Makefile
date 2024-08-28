all:
	g++ -std=c++23 main.cpp  server/hashmap/hashmap.h server/server.cpp client/client.cpp shared/shared.h -o app