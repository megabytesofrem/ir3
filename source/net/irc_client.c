#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include "irc_client.h"
#include "irc_cmd.h"

struct IrcMessage parseMessage(char *str)
{
    char *tmp;
    struct IrcMessage msg;
    int i;

    tmp = strsep(&str, " ");
    if (*tmp == ':')
    {
        msg.origin = tmp + 1;
        msg.command = strsep(&str, " ");
    }
    else
        msg.command = tmp;

    for (i = 0; *str != ':' && str != NULL;)
    {
        tmp = strsep(&str, " ");
        msg.params[i++] = tmp;
        if (*str == ':')
            msg.params[i++] = str + 1;
    }

    msg.nparams = i;
    return msg;
}

struct IrcClient *clientCreate(const char *nick)
{
    struct IrcClient *client = malloc(sizeof(struct IrcClient));
    client->sock_buffer = NULL;
    client->authed = false;
    client->nick = nick;
    client->channel = "";

    return client;
}

bool initSocketSvc()
{
    // initialize the 3DS's socket service
    int buffer = (u32 *)memalign(SOCK_ALIGN, SOCK_BUFFERSIZE);
    if (buffer == NULL)
        return false;

    if (socInit(buffer, SOCK_BUFFERSIZE) != 0)
        return false;

    return true;
}

enum ClientStatus resolveHost(const char *host, char *buffer)
{
    int s;
    bool done = false;
    struct addrinfo *result;

    struct addrinfo hints = {
        .ai_flags = 0,
        .ai_family = AF_UNSPEC,
        .ai_socktype = SOCK_STREAM,
        .ai_protocol = IPPROTO_TCP};

    // address we return
    if ((s = getaddrinfo(host, NULL, &hints, &result)) != 0)
        return RESOLVE_ERROR;

    for (struct addrinfo *rp = result; rp != NULL; rp = rp->ai_next)
    {
        s = getnameinfo(rp->ai_addr, rp->ai_addrlen, buffer, 1024, NULL, 0, NI_NUMERICHOST);

        if (s != 0)
            continue;

        return RESOLVE_SUCCESS;
        break;
    }

    freeaddrinfo(result);
    return RESOLVE_SUCCESS;
}

enum ClientStatus clientConnect(struct IrcClient *client, const char *host, u16 port)
{
    initSocketSvc();

    int nread, ret = 0;

    // buffer to read into
    int data[CLIENT_BUFFERSIZE] = {0};

    struct sockaddr_in addr_in;
    memset(&addr_in, 0, sizeof(struct sockaddr_in));

    // lookup the hostname
    char address[1024];
    resolveHost(host, &address);
    printf("trying to conn to %s:%d\n", address, port);

    printf("1\n");

    // create a connection
    client->fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    addr_in.sin_family = AF_INET;
    addr_in.sin_port = htons(port);

    if (inet_pton(AF_INET, address, &addr_in.sin_addr) <= 0)
    {
        return CLIENT_INVALIDADDR;
    }

    if (connect(client->fd, (struct sockaddr *)&addr_in, sizeof(addr_in)) < 0)
    {
        return CLIENT_ERROR;
    }

    // we are connected
    client->alive = true;

    // send USER and NICK commands
    char userCmd[128];
    sprintf(userCmd, "USER %s %s %s :irthree IRC client\r\n", client->nick, client->nick, client->nick);
    clientCmd(client, userCmd);
    clientNick(client, client->nick);

    return CLIENT_SUCCESS;
}

/**
 * Process a single socket event, intended to be called from `aptMainLoop()`
 * continuously in a loop.
 */
void clientProcess(struct IrcClient *client)
{
    int selected;
    char buffer[1024];
    memset(&buffer, 0, sizeof(buffer));

    struct timeval tv = {
        .tv_sec = 1,
        .tv_usec = 0,
    };

    fd_set readfds;

    FD_ZERO(&readfds);
    FD_SET(client->fd, &readfds);
    selected = select(client->fd + 1, &readfds, NULL, NULL, &tv);
    switch (selected)
    {
    case 0:
        // timeout
        // printf("[E] socket timeout\n");
        break;
    case -1:
        // error
        printf("[E] select(): error occured\n");
        break;
    default:
        if (recv(client->fd, &buffer, sizeof(buffer), 0) < 0)
        {
            // failed to recv data
            printf("[E] select(): failed to recv from remote\n");
            break;
        }
        else
        {
            // Decode any incoming IRC messages
            struct IrcMessage recvMsg = parseMessage(strdup(buffer));
            if (strcmp(recvMsg.command, "PING") == 0)
            {
                printf("REPLYING with %s\n", recvMsg.params[0]);
                clientPong(client, recvMsg.params[0]);
            }
            printf("%s\n", buffer);
        }
        break;
    }
}

void clientClose(struct IrcClient *client)
{
    if (!client->alive)
    {
        return;
    }

    client->alive = false;
    close(client->fd);
    clientFree(client);
}

void clientFree(struct IrcClient *client)
{
    free(client);
}
