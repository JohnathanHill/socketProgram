#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h> //used for sockets
#include<sys/wait.h>   //used for WNOHANG and waitpid
#include<netinet/in.h> //used for internet domain addresses
#include<arpa/inet.h>  //contains inet_  structures
#include<signal.h>     //contains sig structures
#include<mutex>
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

mutex mtx;

int size;

int q = 0;
int count = 1;

struct info{
    int new_fd;
    std::string id;
    int data;
    int exit;
};
/*
void *worker(void *data){
    struct info *info = (struct info*) data;
    int temp;
    if(info->data != 0){
        temp = rand() % info->data;
    }
    while(1){
        if(info->exit){
            printf("Nothing to bid for\n");
            break;
        }
        while(1){
            mtx.lock();

            info->data = size;
            info->exit = q;
            if(item.empty()){
                break;
            }
            if(item[temp].getQuantity() == 0){
                size--;
                item.erase(item.begin() + temp);
                if(item.empty()){
                    q = 1;
                }
            }
            mtx.unlock();
            break;

            item[temp].setCurrBid(item[temp].getCurrBid() + 1);
            printf("Bid count: %d\n", count++);
            //printf("Thread %ld bidding on %s.\t Currend bid is %d\n", (long) pthread_self(), item[temp].getDescription().c_str(), item[temp].getCurrBid());

            //if thread wins bid
            if( item[temp].getCurrBid() == item[temp].getMaxBid()){
                item[temp].setQuantity( item[temp].getQuantity()-1);
                item[temp].setCurrBid(0);

                std::stringstream ss;
                ss<<item[temp].getQuantity();
                //output message to winner
                std::string temp2 = "Winning bid console \tnumber of " + item[temp].getDescription() + " left: "+ ss.str()+"\n";
                //sends message to client who won
                if (send(info->new_fd, temp2.c_str(), temp2.size(), 0) == -1){
                    perror("send");
                    exit(1);
                }

                //printf("winning client id: %ld bought %s\n", (long) pthread_self(), item[temp].getDescription().c_str());
                // pthread_mutex_unlock(&mutex);
                mtx.unlock();
                break;
            }
            //if thread loses
            if( item[temp].getMaxBid() < item[temp].getCurrBid()){
                mtx.unlock();
                continue;
            }
            mtx.unlock();
            //this_thread::sleep_for(chrono::seconds(rand() % 3));
        }
        if(info->data != 0){
            temp = rand() % info->data;
        }
    }
    if(send(info->new_fd, "q", 4, 0) == -1){
        perror("send");
        exit(1);
    }
    close(info->new_fd);
    printf("Thread dying: %ld\n", (long) pthread_self());
    pthread_exit(0);

}
*/
void sigchld_handler(int s)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

int main(void){
    int sockfd, new_fd;
    struct sockaddr_in my_addr;
    struct sockaddr_in their_addr;
    socklen_t sin_size;
    struct sigaction sa;
    int yes = 1;
    int n = 0;

    Item store;
    store.storeInitializer();


    struct info *myinfo;
    std::stringstream ss;

    srand((unsigned) time(NULL));
/*
    for(int i = 0; i < NUMOFITEMS; i++){ 
        Item *temp;
        ss << i;
        temp = new Item(rand() % 10 + 1, "Item:" + ss.str(), rand() % 3+1);
        item.push_back(*temp);
        ss.str("");
    }
    size = item.size();
*/
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
        // myinfo = new struct info;
        // myinfo->data = item.size();
        // myinfo->new_fd = new_fd;

        printf("server: got connection from %s\n", inet_ntoa(their_addr.sin_addr));
        
        if (!fork()) { // this is the child process
            close(sockfd); // child doesn't need the listener
            if (send(new_fd, "Hello, world!\n", 14, 0) == -1)
                perror("send");
            close(new_fd);
            exit(0);
        }
        n++;
    }
    printf("Finished");
    return 0;
}
