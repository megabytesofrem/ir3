#ifndef _IRTHREE_CLIENT_
#define _IRTHREE_CLIENT_

#include <3ds.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "irc_parse.h"

#define SOCK_ALIGN 0x1000
#define SOCK_BUFFERSIZE 0x100000

#define CLIENT_BUFFERSIZE 1024

/**
 * An active connection to an IRC network
 */
struct IRCClient
{
    u32 *sock_buffer;
    bool alive;
    int fd;

    char *nick;
    char *channel;
};

enum ClientStatus
{
    CLIENT_SUCCESS,
    CLIENT_INVALIDADDR,
    CLIENT_ERROR,
};

struct IRCClient *clientCreate();
enum ClientStatus clientConnect(struct IRCClient *client, const char *addr, u16 port);
void clientNick(struct IRCClient *client, const char *nick);
void clientJoin(struct IRCClient *client, const char *chan);

void clientClose(struct IRCClient *client);
void clientFree(struct IRCClient *client);

#endif