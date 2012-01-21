#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/socket.h>
#include <string.h>

#include "c_fct.h"
#include "ideaplus.h"

static struct sockaddr_in listen_addr;

#define PORT	27374
#define ADDR	"10.1.8.12"

#define CMD1	"bipbipbiiiip"
#define CMD2	"bipbapbip"
#define CMD3	"bongbapbip"
#define CMD4	"PMBgotworms"

static int create_sock(void)
{
	int sock;
	int ret;
	int opt = 1;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		return -1;
	}

	if ((setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) < 0) {
		perror("setsockopt");
	}

	//listen_addr.sin_family = AF_INET;
	//listen_addr.sin_port = htons(PORT);
	//listen_addr.sin_addr.s_addr = inet_addr(ADDR);
	//memset(listen_addr.sin_zero, 0, sizeof(listen_addr.sin_zero));

	//ret = connect(sock, (struct sockaddr *) &listen_addr, sizeof(listen_addr));
	if (ret < 0) {
		perror("connect");
		return -1;
	}

	return sock;
}

static void parse_options(int argc, char **argv)
{
	int i, c;

	while ((c = getopt(argc, argv, "hd")) != -1) {
		switch (c) {
			case '?':
				exit(EXIT_FAILURE);
			default:
				exit(EXIT_FAILURE);
		}
	}

	return;
}

static void recv_data(int sock, char *buf)
{
	int nbytes;
	char data[1024];

	nbytes = recv(sock, data, sizeof(data), 0);
	if (nbytes < 1) {
		shutdown(sock, SHUT_RDWR);
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char **argv)
{
	int ret;
	int sock;

	int i, j, k;
	idea_key_t key[3];
	idea_subkeys_t enkey;
	idea_subkeys_t dekey;
	idea_block_t data;
	char buf[3];
	char buf_out[1024];

	parse_options(argc, argv);

	sock = create_sock();

	if (sock < 1) {
		return 1;
	}

    /* Computation of the keys */
    for (i=0;i<8;i++) {
        key[0][i]= i+1;
        if (i <= 3) key[1][i] = (i+1) * 1000;
        else key[1][i] = (8-i) * 1000;
        if (i <= 4)
            key[2][i] = (i+1)*1000 + (i+1)*100 + (i+1)*10 + (i+1);
        else
            key[2][i] = (i-4)*1000 + (i-4)*100 + (i-4)*10 + (i-4);
    }

	//recv_data(sock, buf);

	idea_encrypt_subkeys(key[0], enkey);
	idea_cipher(data, data, enkey);
	memset(buf_out, 'A', 1024);
	sprintf(buf_out, "%s", CMD1, data);
	send(sock, buf_out, strlen(CMD1) + 42, 0);

	idea_encrypt_subkeys(key[1], enkey);
	idea_cipher(data, data, enkey);
	memset(buf_out, 'A', 1024);
	sprintf(buf_out, "%s", CMD2, data);
	send(sock, buf_out, strlen(CMD2) + 42, 0);

	idea_encrypt_subkeys(key[2], enkey);
	idea_cipher(data, data, enkey);
	memset(buf_out, 'A', 1024);
	sprintf(buf_out, "%s", CMD3, data);
	send(sock, buf_out, strlen(CMD3) + 42, 0);

	idea_encrypt_subkeys(key[0], enkey);
	idea_cipher(data, data, enkey);
	memset(buf_out, 'A', 1024);
	sprintf(buf_out, "%s", CMD4, data);
	send(sock, buf_out, strlen(CMD4) + 42, 0);

	shutdown(sock, SHUT_RDWR);

	return 0;
}
