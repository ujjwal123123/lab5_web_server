all: webserver webserver_dynamic

webserver: webserver.o tcp.o request.o
	gcc -Wall webserver.o tcp.o request.o -o webserver -g -lpthread

webserver_dynamic: webserver_dynamic.o tcp.o request.o
	gcc -Wall webserver_dynamic.o tcp.o request.o -o webserver_dynamic -g -lpthread

webserver_dynamic.o: webserver_dynamic.c
	gcc -Wall -g -c webserver_dynamic.c -o webserver_dynamic.o

webserver.o: webserver.c
	gcc -Wall -g -c webserver.c -o webserver.o
 
tcp.o: tcp.c tcp.h
	gcc -Wall -g -c tcp.c -o tcp.o

request.o: request.c request.h
	gcc -Wall -g -c request.c -o request.o

clean:
	rm -f *.o webserver webserver_dynamic