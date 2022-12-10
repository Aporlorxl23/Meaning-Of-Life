/*
gcc socket.c -o socket.so -fPIC -shared -D_GNU_SOURCE
*/
#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <sys/mman.h>

int socket(int domain, int type, int protocol)
{
    int (*new_socket)(int domain, int type, int protocol);
    int result;
    new_socket = dlsym(RTLD_NEXT, "socket");
    printf("[*] Domain %d & Type: %d & Protocol: %d\n", domain, type, protocol);
    return new_socket(domain, type, protocol);
}

ssize_t send(int sockfd, const void *buf, size_t len, int flags) {
    int (*new_send)(int sockfd, const void *buf, size_t len, int flags);
    int result;
    new_send = dlsym(RTLD_NEXT, "send");
    printf("[*] Sockfd: %d & Buf: %p & Len: %lu & Flags: %d\n", sockfd, &buf, len, flags);
    return new_send(sockfd, &buf, len, flags);
}

int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res) {
    int (*new_getaddrinfo)(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);
    new_getaddrinfo = dlsym(RTLD_NEXT, "getaddrinfo");
    printf("[*] Address: %s & Port: %s\n", node, service);
    return new_getaddrinfo(node, service, hints, res);
}
