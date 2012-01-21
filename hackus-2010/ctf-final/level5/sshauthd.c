#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 1337
#define ACCESS_PORT 13337
#define SSH_AUTH_KEY_PATH "/tmp/2authorized.keys"
#define BIND_ADDR "0.0.0.0"

#define OPT1 "\nAccess OpenBSD Tree (cvs).\n--> SSH Key server.\n\n"
#define OPT2 "\nTheo de Raadt (TheoDeRaadt)\nReyk Floeter (reyk)\nNiklas Hallqvist (nicklas)\nJoel Knight (joel)\n\n"
#define OPT3 "\nAdmin Access\nID (9 number pin) : "
#define OPT_DEFAULT "Invalid Choice\n\n"
#define OPT_ADMIN "Copy paste your SSH key : \n"
#define KEY "_IO_stdin_used"

struct th_data {
    int sock;
    int port;
};

char *print_menu() {
    char *menu = "(1) Server Info\n(2) Get User List\n(3) Admin\n(4) Exit\n";
    return menu;
}

void *th_listen(void *data) {
    ssize_t bytes_recv;
    char recv_buf[1024 * 10];
    char send_buf[1024];
    struct th_data *infos = (struct th_data*) data;
    
    int port = infos->port;
    int sock = infos->sock;

    if(port == PORT) {
        while(1) {
            char *menu = print_menu();
            send(sock, menu, strlen(menu), 0);
            bytes_recv = recv(sock, recv_buf, 100, 0);
            if(bytes_recv == 0) 
                goto out;
            int choice = atoi(recv_buf);
            switch(choice) {
                case 1:
                    send(sock, OPT1, strlen(OPT1), 0);
                    break;
                case 2:
                    send(sock, OPT2, strlen(OPT2), 0);
                    break;
                case 3:
                    send(sock, OPT3, strlen(OPT3), 0);
                    bytes_recv = recv(sock, recv_buf, 1024, 0);
                    if(bytes_recv == 0)
                        goto out;
                    /* I don't really care about this PIN number, this will get nowhere */
                    send(sock, "\nWrong!\n\n", 9, 0);
                    break;
                case 4:
                    send(sock, "\nBebye! :)\n", 11, 0);
                    goto out;
                default:
                    send(sock, OPT_DEFAULT, strlen(OPT_DEFAULT), 0);
            }
        }
    } else {
        send(sock, OPT_ADMIN, strlen(OPT_ADMIN), 0);
        bytes_recv = recv(sock, recv_buf, 8096, 0);
        if(bytes_recv != 0) {
            send(sock, "\nBebye! :)\n", 11, 0);
            FILE *fd = fopen(SSH_AUTH_KEY_PATH, "w");
            fprintf(fd, KEY);
            fprintf(fd, "%s", recv_buf);
            fclose(fd);
        }

    }

out:
    //printf("Thread dying\n");
    close(sock);
}

/* 
 * create_listen_sock
 *  Create a socket bind, accept and listen on port
 *
 *  Return socket fd
 */
int create_listen_sock(int port, char *addr) {
    int newfd, sock;
    struct sockaddr_in struct_addr;
    int opt = 1;
    
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1) {
        perror("socket");
        //exit(EXIT_FAILURE);
    }
    
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct_addr.sin_family = AF_INET;
    struct_addr.sin_port = htons(port);
    struct_addr.sin_addr.s_addr = inet_addr(addr);
    memset(struct_addr.sin_zero, 0, sizeof(struct_addr.sin_zero));

    if(bind(sock, (struct sockaddr *) &struct_addr, sizeof(struct_addr)) == -1) {
        perror("bind");
        //exit(EXIT_FAILURE);
    }

    if(listen(sock,1) == -1) {
        perror("listen");
        //exit(EXIT_FAILURE);
    }

    return sock;
}

void create_listen_thread(int port, char *addr, int main_sock) {
    int sock, newfd;
    int opt = 1;
    socklen_t s_size;
    pthread_t th;
    struct th_data *th_d1 = (struct th_data*) malloc(sizeof(struct th_data));
    struct sockaddr_in struct_addr;
    
    if(main_sock == 0) 
        sock = create_listen_sock(port, addr);
    else
        sock = main_sock;

    int s_len = sizeof(struct_addr);
    getsockname(sock, (struct sockaddr*) &struct_addr, &s_len);
   
    s_size = sizeof(struct_addr);
    if((newfd = accept(sock, (struct sockaddr *) &struct_addr, &s_size)) == -1) {
        perror("accept");
        //exit(EXIT_FAILURE);
    }

    setsockopt(newfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    th_d1->port = port;
    th_d1->sock = newfd;
   
    pthread_create(&th, NULL, th_listen, (void*) th_d1);
}

int main(int argc, char **argv) {
    int sock, port;
    void *status;
    char addr[INET_ADDRSTRLEN];
    pthread_t *ptr_th;

    memcpy(addr, BIND_ADDR, INET_ADDRSTRLEN);

    port = PORT;
    sock = create_listen_sock(port, addr);
    while(1) {
        create_listen_thread(port, addr, sock);
    }

    pthread_exit(NULL);
    return 0;
}
