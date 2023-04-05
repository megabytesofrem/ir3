#include <stdio.h>
#include <string.h>

#include "irc_cmd.h"

void clientPong(struct IrcClient *client, const char *token)
{
    char command[128] = {0};
    sprintf(command, "PONG %s\r\n", token);
    send(client->fd, command, strlen(command), 0);
}

void clientNick(struct IrcClient *client, const char *nick)
{
    char command[128] = {0};
    sprintf(command, "NICK %s\r\n", nick);
    send(client->fd, command, strlen(command), 0);
}

void clientJoin(struct IrcClient *client, const char *chan)
{
    char command[128] = {0};
    sprintf(command, "JOIN %s\r\n", chan);
    send(client->fd, command, strlen(command), 0);
}

void clientMsg(struct IrcClient *client, const char *target, const char *msg)
{
    char command[128] = {0};
    sprintf(command, "PRIVMSG %s :%s\r\n", target, msg);
    send(client->fd, command, strlen(command), 0);
}

void clientCmd(struct IrcClient *client, const char *cmd)
{
    char command[128] = {0};
    sprintf(command, "%s\r\n", cmd);
    send(client->fd, command, strlen(command), 0);
}