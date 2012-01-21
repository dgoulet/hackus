#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/socket.h>
#include <string.h>

#define FMT1 "%s -- %d\n"
#define FMT2 "%s [%d]\n"
#define FMT3 "%s : %d\n"
#define FMT4 "%s\t%d\n"
#define FMT5 "%s (%d)\n"
#define ENC_TYPE "rc2-40-cbc"
#define MAX_RECV 128

static struct sockaddr_in listen_addr;

static ssize_t send_data(int, char*, size_t);
static ssize_t recv_data(int, char*, size_t);

static char menu[] = "\n\
g) Get a product\n\
l) Login\n\
p) Print product list\n\
r) Remove product\n\
q) Quit\n\
\n\
Command : ";

static char product_title[] = "\nProduct List :\n=====\n";

static char *item_list[20] = {
	"Orangina",
	"Butterfly in a jar",
	"Canned Unicorn Meat",
	"Acrobots",
	"Alien Facehugger Plush",
	"42 Utility Towel",
	"USB Pet Rock",
	"Tux",
	"Bacon Soap",
	"USB Rocket Launcher",
	"SNES Controller",
	"Giant Paper Clip",
	"Settlers of Catan",
	"Bacon Salt",
	"Bacon Gravy",
	"Bacon Lip Balm",
	"Bacon Mints",
	"Awesome Sauce",
	"Big T-Shirt",
	"Coffee",
};

static int qty[20] = {5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5};

static int num_items = 20;

static char format_menu[] = "\n\
1)Product -- Qty\n\
2)Product [Qty]\n\
3)Product : Qty\n\
4)Product	Qty\n\
5)Product (Qty)\n\
\n\
Choose : ";

static int sock;
static char login_menu[] = "\nUser : ";
static char pass_menu[] = "Pass : ";
static char recv_buf[MAX_RECV];

static void send_menu(void)
{
	send_data(sock, menu, sizeof(menu));
}

static void send_list(char *fmt)
{
	int i;
	char **ptr_c;

	send_data(sock, product_title, strlen(product_title));
	for (i = 0;i < num_items;i++) {
		asprintf(ptr_c, fmt, item_list[i], qty[i]);
		send_data(sock, *ptr_c, strlen(*ptr_c));
		free(*ptr_c);
	}
}

static void manage_client(char *pass, char *filename)
{
	int canari = 42;
	char c_on[] = "off";
	char quit[] = "no";
	char user[] = "monique";
	char buf[4];
	char fmt[32];
	char line[128];
	int i = 0;
	FILE *fd;
	long fsize;
	ssize_t ret;

	while (1) {
		if (strstr(quit, "no") == NULL) {
			goto end;
		}
		send_menu();

		ret = recv_data(sock, recv_buf, MAX_RECV);
		if (ret < 0) {
			goto end;
		}

		recv_buf[ret - 1] = '\0';

		memcpy(buf, recv_buf, ret - 1);

		if (buf[0] == 'q') {
			goto end;
		} else if (buf[0] == 'g') {
			send_data(sock, "N/A\n\n", 5);
		} else if (buf[0] == 'r') {
			/* FIXME: Need some intern for that function */
		} else if (buf[0] == 'p') {
			send_data(sock, format_menu, sizeof(format_menu));
			ret = recv_data(sock, recv_buf, MAX_RECV);
			if (ret < 0) {
				break;
			}
			memcpy(buf, recv_buf, ret - 1);

			switch (atoi(&buf[0])) {
				case 1:
					strcpy(fmt, FMT1);
					break;
				case 2:
					strcpy(fmt, FMT2);
					break;
				case 3:
					strcpy(fmt, FMT3);
					break;
				case 4:
					strcpy(fmt, FMT4);
					break;
				case 5:
					strcpy(fmt, FMT5);
					break;
				default:
					strcpy(fmt, FMT3);
					break;
			}

			if (atoi(&buf[0]) == 6) {
				if (strstr(c_on, "on") != NULL) {
					send_data(sock, "Pretty Print : ", 15);
					i = recv_data(sock, fmt, MAX_RECV);
					if (i < 0) {
						break;
					}
				}
				send_data(sock, product_title, strlen(product_title));
				for (i = 0;i < num_items;i++) {
					printf(fmt, item_list[i], qty[i]);
					fflush(stdout);
				}
			} else {
				send_list(fmt);
			}

		} else if (buf[0] == 'l') {
			send_data(sock, login_menu, sizeof(login_menu));
			if (recv_data(sock, line, MAX_RECV) < 0) {
				break;
			}
			if (strncmp(user, line, strlen(user)) == 0) {
				send_data(sock, pass_menu, sizeof(pass_menu));
				if (recv_data(sock, line, MAX_RECV) < 0) {
					break;
				}

				if (strncmp(pass, line, strlen(pass)) == 0) {
					fd = fopen(filename, "rb");
					fseek(fd, 0, SEEK_END);
					fsize = ftell(fd);
					rewind(fd);
					fread(line, 1, fsize, fd);
					send_data(sock, line, fsize);
					fclose(fd);
				}
			}
		}
	}

end:
	shutdown(sock, SHUT_RDWR);
}

static ssize_t send_data(int sock, char *buf, size_t len)
{
	int ret;
	ret = send(sock, buf, len, 0);
	if (ret < 0) {
		perror("send");
	}

	return ret;
}

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

	listen_addr.sin_family = AF_INET;
	listen_addr.sin_port = htons(8712);
	listen_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(listen_addr.sin_zero, 0, sizeof(listen_addr.sin_zero));

	ret = bind(sock, (struct sockaddr *) &listen_addr, sizeof(listen_addr));
	if (ret < 0) {
		perror("bind");
		return -1;
	}

	ret = listen(sock, 50);
	if (ret < 0) {
		perror("listen");
		return -1;
	}

	return sock;
}

static int accept_request(int sock)
{
	int sockfd;
	int opt;
	socklen_t s_size;

	s_size = sizeof(listen_addr);
	sockfd = accept(sock, (struct sockaddr *) &listen_addr, &s_size);
	if (sockfd < 0) {
		perror("accept");
		return -1;
	}

	if ((setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) < 0) {
		perror("setsockopt");
		return -1;
	}

	return sockfd;
}


static void daemonize(void)
{
	pid_t pid, sid;

	if ((pid = fork()) < 0) {
		perror("fork");
		exit(EXIT_FAILURE);
	}

	/* Parent process */
	if (pid > 0) {
		exit(EXIT_SUCCESS);
	}

    /* Change the file mode mask */
    umask(0);

    /* Create a new SID for the child process */
    if ((sid = setsid()) < 0) {
        perror("setsid");
        exit(EXIT_FAILURE);
    }

	/* Change the current working directory */
    if ((chdir("/tmp")) < 0) {
        perror("chdir");
        exit(EXIT_FAILURE);
    }

    /* Close out the standard file descriptors */
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}

static void parse_options(int argc, char **argv, char **secret)
{
	int i, c;

	while ((c = getopt(argc, argv, "hd" "s:")) != -1) {
		switch (c) {
			case 'd':
				daemonize();
				break;
			case 's':
				*secret = strdup(optarg);
				break;
			case '?':
				exit(EXIT_FAILURE);
			default:
				exit(EXIT_FAILURE);
		}
	}

	return;
}

static pid_t client_fork(void)
{
	pid_t pid;

	if ((pid = fork()) < 0) {
		perror("fork");
		exit(EXIT_FAILURE);
	}

	return pid;
}

static ssize_t recv_data(int sock, char *buf, size_t len)
{
	ssize_t nbytes;

	nbytes = recv(sock, buf, len, 0);
	if (nbytes < 1) {
		perror("recv");
		shutdown(sock, SHUT_RDWR);
	}

	return nbytes;
}

int main(int argc, char **argv)
{
	int new_sock;
	pid_t pid = 0;
	int opt = 1;
	char *pass;

	parse_options(argc, argv, &pass);

	sock = create_sock();

	while (1) {
		new_sock = accept_request(sock);
		if (new_sock < 1) {
			break;
		}

		if ((setsockopt(new_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) < 0) {
			perror("setsockopt");
		}

		//pid = client_fork();
		if (pid == 0) {
			printf("Client spawned\n");
			sock = new_sock;
			dup2(sock, 0);
			dup2(sock, 1);
			manage_client(pass, "price_list.txt");
		}
		close(new_sock);
	}

	return 0;
}
