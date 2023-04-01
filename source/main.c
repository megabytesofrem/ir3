#include <3ds.h>
#include <stdio.h>
#include <fcntl.h>
#include <citro2d.h>

#include "net/irc_parse.h"
#include "net/irc_client.h"

int main(int argc, char **argv)
{
    gfxInitDefault();

    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();

    consoleInit(GFX_TOP, NULL);
    printf("IRThree v0.1 (c) Rem. Licensed under MIT.\n");

    struct IRCClient *client = clientCreate();
    enum ClientStatus status = clientConnect(client, "185.100.59.59", 6667);

    char buffer[1024] = {0};

    while (aptMainLoop())
    {
        hidScanInput();

        // TODO: fix so this doesn't spam and hang the main thread

        int recv = read(client->fd, buffer, 1024);
        printf("%s\n", buffer);

        if (hidKeysDown() & KEY_START)
            break;
    }

    clientClose(client);
    C2D_Fini();
    C3D_Fini();
    gfxExit();
    return 0;
}