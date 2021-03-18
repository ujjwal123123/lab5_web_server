
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
 4. use conditional variables
 */

#include "priority_queue.h"
#include "queue.h"
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

#define FCFS 0
#define SFF 1

int mode = -1;

PRIORITY_QUEUE *request_priority_queue = NULL;
struct queue_t *request_queue = NULL;

// Equivalent to a consumer in the producer consumer problem
void thread_request_handler() {
    // to allocate the resource of thread back to the machine on completion of
    // thread
    pthread_detach(pthread_self());

    printf("[Thread] thread created %lu\n", pthread_self());

    while (true) {
        // NOTE: there should be no busy waiting because `remove_queue` function
        // proceeds when a lock is held.
        struct request *req = NULL;
        if (mode == FCFS) {
            req = remove_queue(request_queue);
        }
        else if (mode == SFF) {
            printf("[Priority queue] Removing from the priority queue\n");
            req = extract_maximum(request_priority_queue);
        }

        printf("[Thread] serving a request %s (TID: %lu)\n", req->filename,
               pthread_self());
        request_handle(req);
        request_delete(req);
    }

    pthread_exit(NULL); // SIGSEGV if not exited
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        // ./webserver PORT NTHREADS MODE
        fprintf(stderr, "use: %s <port> <thread count> <mode>\n", argv[0]);
        return 1;
    }

    if (chdir("webdocs") != 0) {
        fprintf(stderr, "couldn't change to webdocs directory: %s\n",
                strerror(errno));
        return 2;
    }

    int port = atoi(argv[1]);

    int thread_count = atoi(argv[2]);

    char *mode_name = argv[3];
    if (strcmp(mode_name, "FCFS") == 0) {
        mode = FCFS;
    }
    else if (strcmp(mode_name, "SFF") == 0) {
        mode = SFF;
    }
    else {
        printf("Error: invalid mode\n");
        return 3;
    }

    struct tcp *master = tcp_listen(port); // TCP master object
    if (!master) {
        fprintf(stderr, "couldn't listen on port %d: %s\n", port,
                strerror(errno));
        return 4;
    }

    // With help from :
    // https://www.thegeekstuff.com/2012/04/create-threads-in-linux/

    if (mode == FCFS) {
        request_queue = new_queue();
    }
    else if (mode == SFF) {
        request_priority_queue = init_priority_queue();
    }

    // Some code from
    // https://www.cs.cmu.edu/afs/cs/academic/class/15492-f07/www/pthreads.html

    pthread_t *thread_id =
        (pthread_t *)malloc(sizeof(pthread_t *) * thread_count);
    for (int i = 0; i < thread_count; i++) {
        pthread_create(&thread_id[i], NULL, thread_request_handler, NULL);
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
                if (mode == FCFS) {
                    insert_queue(request_queue, req);
                }
                else if (mode == SFF) {
                    insert_priority_queue(request_priority_queue, req);
                    printf("[Priority queue] Inserted into priority queue\n");
                }
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