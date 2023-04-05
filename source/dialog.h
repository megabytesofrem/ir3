#ifndef _UI_DIALOG_
#define _UI_DIALOG_

#include <3ds.h>
#include <citro2d.h>

#define BT_SCREEN_WIDTH 320
#define BT_SCREEN_HEIGHT 240

#define clrDialog_BG C2D_Color32(0x28, 0x28, 0x28, 0XFF)
#define clrDialog_FG C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF)
#define clrDialog_Active C2D_Color32(0xC2, 0x30, 0x30, 0xFF)

struct Dialog
{
    char *title;
    char *text;

    C2D_TextBuf textBuf;
};

struct Dialog *createDialog(const char *title, const char *text);
void drawDialog(struct Dialog *msgbox, C3D_RenderTarget *target);

#endif