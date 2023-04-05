#ifndef _IRC_CMD_
#define _IRC_CMD_

// for struct IrcClient
#include "irc_client.h"

// Construct and send a PONG reply in response to a PING command
void clientPong(struct IrcClient *client, const char *token);

void clientNick(struct IrcClient *client, const char *nick);
void clientJoin(struct IrcClient *client, const char *chan);
void clientMsg(struct IrcClient *client, const char *target, const char *msg);
void clientCmd(struct IrcClient *client, const char *cmd);

#endif