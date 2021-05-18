all: webserver webserver_dynamic webserver_pool

webserver: webserver.o tcp.o request.o
	gcc -Wall webserver.o tcp.o request.o -o webserver -g -lpthread

webserver.o: webserver.c
	gcc -Wall -g -c webserver.c -o webserver.o
 
webserver_dynamic: webserver_dynamic.o tcp.o request.o
	gcc -Wall webserver_dynamic.o tcp.o request.o -o webserver_dynamic -g -lpthread

webserver_dynamic.o: 
	gcc -Wall -g -c webserver_dynamic.c -o webserver_dynamic.o

webserver_pool: webserver_pool.o tcp.o request.o queue.o priority_queue
	gcc -Wall webserver_pool.o tcp.o request.o queue.o priority_queue.o heap.o -o webserver_pool -g -lpthread

webserver_pool.o: webserver_pool.c
	gcc -Wall -g -c webserver_pool.c -o webserver_pool.o 

queue.o: queue.h queue.c
	gcc -Wall -g -c queue.c

tcp.o: tcp.c tcp.h
	gcc -Wall -g -c tcp.c

request.o: request.c request.h
	gcc -Wall -g -c request.c

priority_queue:
	gcc -Wall -g -c heap.c
	gcc -Wall -g -c priority_queue.c

clean:
	rm -f *.o webserver webserver_dynamic webserver_pool

clear:
	clear