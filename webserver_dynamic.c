
/*
 This is the main program for the simple webserver.

 Your job is to modify it to take additional command line arguments to specify
 the number of threads and scheduling mode, and then, using a pthread monitor,
 add support for a thread pool.

 All of your changes should be made to this file, with the possible exception
 that you may add items to struct request in request.h
 */

#include "request.h"
#include "tcp.h"
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void thread_request_handler(struct request *req) {
    printf("webserver: got request for %s\n", req->filename);
    request_handle(req);

    printf("webserver: done sending %s\n", req->filename);
    request_delete(req);

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
        return 1;
    }

    int port = atoi(argv[1]);

    struct tcp *master = tcp_listen(port); // TCP master object
    if (!master) {
        fprintf(stderr, "couldn't listen on port %d: %s\n", port,
                strerror(errno));
        return 1;
    }

    // With help from :
    // https://www.thegeekstuff.com/2012/04/create-threads-in-linux/

    printf("webserver: waiting for requests..\n");

    while (1) {
        /* TODO: why are we creating new connections on the same port mulitple
         * times? */
        struct tcp *conn =
            tcp_accept(master, time(0) + 300); // creates a new TCP connection

        if (conn) {
            printf("webserver: got new connection.\n");
            struct request *req = request_create(conn);

            if (req) {
                // creating threds
                printf("Creating new thread\n");
                pthread_t thread;
                pthread_create(&thread, NULL, (void *)thread_request_handler,
                               req);
            }
            else {
                tcp_close(conn);
            }
        }
        else {
            printf("webserver: shutting down because idle too long\n");
            break;
        }
    }

    return 0;
}