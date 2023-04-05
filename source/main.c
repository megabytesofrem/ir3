#include <3ds.h>
#include <stdio.h>
#include <fcntl.h>
#include <citro2d.h>

#include <sys/select.h>
#include "net/irc_client.h"

int main(int argc, char **argv)
{
    gfxInitDefault();

    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();

    consoleInit(GFX_TOP, NULL);
    printf("IRThree v0.1 (c) Rem. Licensed under MIT.\n");

    // client
    struct IrcClient *client = clientCreate("rem");
    enum ClientStatus status = clientConnect(client, "irc.efnet.nl", 6667);

    clientJoin(client, "#linux");

    atexit(gfxExit);
    atexit(socExit);

    char buffer[1024] = {0};

    // select loop
    int selected;
    struct timeval tv = {
        .tv_sec = 1,
        .tv_usec = 1,
    };

    fd_set readfds;

    while (aptMainLoop())
    {
        hidScanInput();
        clientProcess(client);

        if (hidKeysDown() & KEY_START)
            break;
    }

    clientClose(client);
    C2D_Fini();
    C3D_Fini();
    return 0;
}