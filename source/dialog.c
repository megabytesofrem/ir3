#include "dialog.h"

struct Dialog *createDialog(const char *title, const char *text)
{
    struct Dialog *dialog = malloc(sizeof(struct Dialog));
    dialog->title = title;
    dialog->text = text;
    dialog->textBuf = C2D_TextBufNew(4096);

    return dialog;
}

void drawDialog(struct Dialog *dialog, C3D_RenderTarget *target)
{
    float pad = 50;

    // center the dialog
    float x = BT_SCREEN_WIDTH / 2 - (BT_SCREEN_WIDTH - pad) / 2;
    float y = BT_SCREEN_HEIGHT / 2 - (BT_SCREEN_HEIGHT - pad) / 2;

    C2D_TextBufClear(dialog->textBuf);

    char buf[256];
    C2D_Text title, text;
    snprintf(buf, sizeof(buf), dialog->title);
    C2D_TextParse(&title, dialog->textBuf, buf);
    C2D_TextOptimize(&title);

    snprintf(buf, sizeof(buf), dialog->text);
    C2D_TextParse(&text, dialog->textBuf, buf);
    C2D_TextOptimize(&text);

    // draw the dialog
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_SceneBegin(target);
    {
        C2D_DrawRectSolid(0, 0, 0.0f, BT_SCREEN_WIDTH, BT_SCREEN_HEIGHT, clrDialog_FG);
        C2D_DrawRectSolid(x, y, 0.0f, BT_SCREEN_WIDTH - pad, BT_SCREEN_HEIGHT - pad, clrDialog_BG);

        C2D_DrawText(&title, C2D_AlignLeft | C2D_WithColor, 100, 30, 0.0f, 0.7f, 0.7f, clrDialog_FG);
        C2D_DrawText(&text, C2D_AlignCenter | C2D_WithColor, 100, 60, 0.0f, 0.5f, 0.5f, clrDialog_FG);
    };
    C3D_FrameEnd(0);
}