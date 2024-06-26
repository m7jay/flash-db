#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static void die(const char *msg)
{
    int err = errno;
    fprintf(stderr, "[%d] %s\n", err, msg);
    abort();
}

static void msg(const char *s)
{
    fprintf(stderr, "%s\n", s);
}

static void do_something(int connfd)
{
    char rbuf[64] = {};
    ssize_t n = read(connfd, rbuf, sizeof(rbuf) - 1);
    if (n < 0)
    {
        msg("read() error");
        return;
    }
    printf("client says: %s\n", rbuf);
    char wbuf[] = "world";
    write(connfd, wbuf, strlen(wbuf));
}

int main()
{
    // create socket
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        die("socket()");
    }

    // set socket options
    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    // bind socket
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(1234);
    addr.sin_addr.s_addr = ntohl(0);
    int rv = bind(fd, (const sockaddr *)&addr, sizeof(addr));
    if (rv)
    {
        die("bind()");
    }

    // listen
    rv = listen(fd, SOMAXCONN);
    if (rv)
    {
        die("listen()");
    }

    while (true)
    {
        struct sockaddr_in client_addr = {};
        socklen_t socklen = sizeof(client_addr);
        int connfd = accept(fd, (struct sockaddr *)&client_addr, &socklen);
        if (connfd < 0)
        {
            continue; // error
        }

        do_something(connfd);
        close(connfd);
    }
    return 0;
}