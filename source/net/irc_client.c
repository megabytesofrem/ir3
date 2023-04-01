#include "irc_client.h"
#include "irc_parse.h"

#include <string.h>
#include <stdlib.h>

struct IRCClient *clientCreate()
{
    struct IRCClient *client = malloc(sizeof(struct IRCClient));
    client->sock_buffer = NULL;
    client->channel = "";

    return client;
}

enum ClientStatus clientConnect(struct IRCClient *client, const char *addr, u16 port)
{
    int nread, ret = 0;

    // buffer to read into
    int data[CLIENT_BUFFERSIZE] = {0};

    // initialize the 3DS' socket service
    int sock_buffer = (u32 *)memalign(SOCK_ALIGN, SOCK_BUFFERSIZE);
    if (sock_buffer == NULL)
    {
        printf("memalign: failed to alloc\n");
        return CLIENT_ERROR;
    }

    if ((ret = socInit(sock_buffer, SOCK_BUFFERSIZE)) != 0)
    {
        printf("socInit: failed\n");
        return CLIENT_ERROR;
    }

    atexit(gfxExit);
    atexit(socExit);

    // netcode
    struct sockaddr_in addr_in;
    memset(&addr_in, 0, sizeof(struct sockaddr_in));

    // create a connection
    client->fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    addr_in.sin_family = AF_INET;
    addr_in.sin_port = htons(port);

    if (inet_pton(AF_INET, addr, &addr_in.sin_addr) <= 0)
    {
        printf("connect: invalid address %s\n", addr);
        return CLIENT_INVALIDADDR;
    }

    if (connect(client->fd, (struct sockaddr *)&addr_in, sizeof(addr_in)) < 0)
    {
        printf("connect: connect() failed\n");
        return CLIENT_ERROR;
    }

    // we are connected
    client->alive = true;
    return CLIENT_SUCCESS;
}

void clientNick(struct IRCClient *client, const char *nick)
{
    char command[32] = {0};
    sprintf(command, "NICK %s\r\n", client->nick);
    send(client->fd, command, strlen(command), 0);
}

void clientJoin(struct IRCClient *client, const char *chan)
{
    char command[32] = {0};
    sprintf(command, "JOIN %s\r\n", chan);
    send(client->fd, command, strlen(command), 0);
}

void clientClose(struct IRCClient *client)
{
    if (!client->alive)
    {
        printf("error: no connection open\n");
        return;
    }

    client->alive = false;
    close(client->fd);
    clientFree(client);
}

void clientFree(struct IRCClient *client)
{
    free(client);
}
