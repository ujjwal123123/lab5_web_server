
/*
 This is the main program for the simple webserver.

 Your job is to modify it to take additional command line arguments to specify
 the number of threads and scheduling mode, and then, using a pthread monitor,
 add support for a thread pool.

 All of your changes should be made to this file, with the possible exception
 that you may add items to struct request in request.h
 */

/*
 TODOs:
 3. Implement two algorithms:
    a. Shortest File First
    b. First Come First Served
 4. Do not use busy waiting
 */

#include "queue.h" // TODO: use header file instead
#include "request.h"
#include "tcp.h"
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NTHREADS 10

// Equivalent to a consumer in the producer consumer problem
void thread_request_handler(struct queue_t *queue) {
    // to allocate the resource of thread back to the machine on completion of
    // thread
    pthread_detach(pthread_self());

    printf("[Thread] thread created %lu\n", pthread_self());

    while (true) {
        struct request *req = remove_queue(queue);

        printf("[Thread] serving a request %s (TID: %lu)\n", req->filename,
               pthread_self());
        request_handle(req);
        request_delete(req);
    }

    pthread_exit(NULL); // SIGSEGV if not exited
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "use: %s <port>\n", argv[0]);
        return 1;
    }

    if (chdir("webdocs") != 0) {
        fprintf(stderr, "couldn't change to webdocs directory: %s\n",
                strerror(errno));
        return 2;
    }

    int port = atoi(argv[1]);

    struct tcp *master = tcp_listen(port); // TCP master object
    if (!master) {
        fprintf(stderr, "couldn't listen on port %d: %s\n", port,
                strerror(errno));
        return 3;
    }

    // With help from :
    // https://www.thegeekstuff.com/2012/04/create-threads-in-linux/

    struct queue_t *request_queue = new_queue();

    // Some code from
    // https://www.cs.cmu.edu/afs/cs/academic/class/15492-f07/www/pthreads.html

    pthread_t thread_id[NTHREADS]; // TODO: use malloc instead
    for (int i = 0; i < NTHREADS; i++) {
        pthread_create(&thread_id[i], NULL, thread_request_handler,
                       request_queue);
        // pthread_join(thread_id[i], NULL);
    }
    printf("[Main] threads created..\n");

    while (true) {
        struct tcp *conn =
            tcp_accept(master, time(0) + 300); // creates a new TCP connection

        if (conn) {
            printf("[Main] got new connection.\n");
            struct request *req = request_create(conn);

            if (req) {
                printf("[Main] got request for %s\n", req->filename);
                insert_queue(request_queue, req);
            }
            else {
                tcp_close(conn);
            }
        }
        else {
            printf("[Main] shutting down because idle too long\n");
            break;
        }
    }

    return 0;
}