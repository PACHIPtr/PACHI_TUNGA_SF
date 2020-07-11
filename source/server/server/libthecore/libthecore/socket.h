#pragma once
#define INVALID_SOCKET -1

extern int socket_read(socket_t desc, char* read_point, size_t space_left);
extern int socket_write(socket_t desc, const char* data, size_t length);

extern int socket_udp_read(socket_t desc, char* read_point, size_t space_left, struct sockaddr* from, socklen_t* fromlen);
extern int socket_tcp_bind(const char* ip, int port);
extern int socket_udp_bind(const char* ip, int port);

extern socket_t socket_accept(socket_t s, struct sockaddr_in* peer);
extern void socket_close(socket_t s);
extern socket_t socket_connect(const char* host, WORD port);

extern void socket_nonblock(socket_t s);
extern void socket_block(socket_t s);
extern void socket_dontroute(socket_t s);
extern void socket_lingeroff(socket_t s);
extern void socket_lingeron(socket_t s);

extern void socket_sndbuf(socket_t s, unsigned int opt);
extern void socket_rcvbuf(socket_t s, unsigned int opt);