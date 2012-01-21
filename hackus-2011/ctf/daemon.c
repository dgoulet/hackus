#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/socket.h>
#include <string.h>

static struct sockaddr_in listen_addr;

#define BUFSIZE 16

#define EXPLOIT "/tmp/exploit"
#define PASSWORD "toutoune"

static void manage_client(char **argv)
{
	FILE *fd;
	char line[1024];
	char cmd[1024];

	snprintf(cmd, 1024, "%s %s %s", EXPLOIT, argv[0], argv[1]);

	fd = popen(cmd, "r");

	while (fgets(line, sizeof(line), fd)) {
	}

	if (!strncmp(line, "Granted", 7)) {
		printf("YAHOO\n");
	}
	pclose(fd);
	/*
	u_long diff;
	int len_user = 0;
	int auth = 0;
	char pass[BUFSIZE];
	char *user = (char *)malloc(BUFSIZE);
	char *password = (char *)malloc(BUFSIZE);

	len_user = strlen(argv[1]);

	strcpy(pass, argv[0]);

	diff = (u_long)password - (u_long)user;
	printf("user = %p, password = %p, diff = 0x%x bytes\n", user, password, diff);

	strncpy(password, argv[0], strlen(argv[0]));

	if (strncmp(password, PASSWORD, strlen(PASSWORD))) {
		exit(EXIT_FAILURE);
	}

	printf("before overflow: password = %s\n", password);
	strncpy(user, argv[1], strlen(argv[1]));
	printf("after overflow: password = %s, user = %s\n", password, user);

	if (strlen(user) == strlen(password)) {
		exit(EXIT_FAILURE);
	}

	printf("auth : %d\n", auth);
	if (auth < 0 && !strncmp(user, password, strlen(password))) {
		printf("Granted\n");
	}
	*/
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
	listen_addr.sin_port = htons(4242);
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

static void parse_options(int argc, char **argv)
{
	int i, c;

	while ((c = getopt(argc, argv, "hd")) != -1) {
		switch (c) {
			case 'd':
				daemonize();
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

static void recv_data(int sock, char **buf)
{
	int nbytes;
	char *pch;
	int count = 0;
	char data[1024];
	char *ptr;

	nbytes = recv(sock, data, sizeof(data), 0);
	if (nbytes < 1) {
		shutdown(sock, SHUT_RDWR);
		exit(EXIT_FAILURE);
	}

	pch = strtok_r(data, " ", &ptr);
	while (pch != NULL) {
		if (count == 0) {
			buf[0] = pch;
			count++;
		} else {
			buf[1] = pch;
			break;
		}
		pch = strtok_r(NULL, " ", &ptr);
	}
}

int main(int argc, char **argv)
{
	int ret;
	int sock;
	int new_sock;
	pid_t pid;
	char *buf[2];

	parse_options(argc, argv);

	sock = create_sock();

	while (1) {
		new_sock = accept_request(sock);
		if (new_sock < 1) {
			break;
		}
		pid = client_fork();
		if (pid == 0) {
			printf("Client spawned\n");
			recv_data(new_sock, buf);
			manage_client(buf);
		}
		shutdown(new_sock, SHUT_RDWR);
	}

	return 0;
}
