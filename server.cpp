#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h> //used for sockets
#include<sys/wait.h>   //used for WNOHANG and waitpid
#include<netinet/in.h> //used for internet domain addresses
#include<arpa/inet.h>  //contains inet_  structures
#include<signal.h>     //contains sig structures
#include<vector>
#include<fstream>
#include<sstream>
#include<chrono>
#include<unistd.h>
#include<cstring>
#include<string>
#include "items.h"

#define MYPORT 8080
#define BACKLOG 10
#define NUMOFITEMS 10

using namespace std;

void sigchld_handler(int s)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

int main(void){
    int sockfd, new_fd;     // listen on sock_fd, new connection on new_fd
    struct sockaddr_in my_addr;     // my address information
    struct sockaddr_in their_addr;  // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes = 1;
    int n = 0;

    Item store;
    store.storeInitializer();
    store.printStore();

    srand((unsigned) time(NULL));

    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1){
        perror("Error opening socket");
        exit(1);
    }
    if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
        perror("setsockopt failed");
        exit(1);
    }

    my_addr.sin_family = AF_INET;         // host byte order
    my_addr.sin_port = htons(MYPORT);     // short, network byte order
    my_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
    memset(&(my_addr.sin_zero), '\0', 8); // zero the rest of the struct

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }


    while(1) {  // main accept() loop
        sin_size = sizeof(struct sockaddr_in);
        if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1) {
            perror("accept");
            continue;
        }

        printf("server: got connection from %s\n", inet_ntoa(their_addr.sin_addr));
    
        while(!fork()) { // this is the child process
            close(sockfd); // child doesn't need the listener
            if (send(new_fd, "hello", 50, 0) == -1)
                perror("send");
            close(new_fd);
            exit(0);
        }
        n++;
    }
    printf("Finished");
    return 0;
}
