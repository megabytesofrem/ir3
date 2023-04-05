#ifndef _IRC_CLIENT_
#define _IRC_CLIENT_

#include <3ds.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "irc_cmd.h"

#define SOCK_ALIGN 0x1000
#define SOCK_BUFFERSIZE 0x100000

#define CLIENT_BUFFERSIZE 1024

/**
 * An active connection to an IRC network
 */
struct IrcClient
{
    u32 *sock_buffer;
    bool alive, authed;
    int fd;

    char *nick;
    char *channel;
};

struct IrcMessage
{
    // optional note of where the message came from (prefixed with :)
    char *origin;
    char *command;
    char *params[15];
    int nparams;
};

enum ClientStatus
{
    CLIENT_SUCCESS,
    CLIENT_INVALIDADDR,
    CLIENT_ERROR,

    RESOLVE_ERROR,
    RESOLVE_SUCCESS,
};

struct IrcMessage parseMessage(char *str);

bool initSocketSvc();
enum ClientStatus resolveHost(const char *host, char *buffer);

struct IrcClient *clientCreate(const char *nick);
enum ClientStatus clientConnect(struct IrcClient *client, const char *host, u16 port);
void clientProcess(struct IrcClient *client);
void clientClose(struct IrcClient *client);
void clientFree(struct IrcClient *client);

#endif