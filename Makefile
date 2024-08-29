all:
	g++ -std=c++23 main.cpp  server/hashmap/hashmap.h server/server.cpp client/client.cpp shared/shared.h -o app
runClient:
	g++ -std=c++23 main.cpp  server/hashmap/hashmap.h server/server.cpp client/client.cpp shared/shared.h -o clientPro && ./clientPro client
runServer:
	g++ -std=c++23 main.cpp  server/hashmap/hashmap.h server/server.cpp client/client.cpp shared/shared.h -o serverPro && ./serverPro server 10