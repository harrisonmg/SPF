#include <3ds.h>
#include <citro2d.h>
#include <stdio.h>

// draw a single color on an entire render target
void drawColorOnScreen(C3D_RenderTarget* screen, u32 color)
{
    // just need to clear the screen to the target color and begin the scene
    C2D_TargetClear(screen, color);
    C2D_SceneBegin(screen);
}

int main(int argc, char **argv)
{
    // graphics initialization
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();

    // top and bottom screen render targets
    C3D_RenderTarget* top    = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

    // initialize console on top screen and print out intro message
    consoleInit(GFX_TOP, NULL);
    printf("\n");
    printf("Welcome to the stuck pixel fixer!\n");
    printf("\n");
    printf("\n");
    printf("TOUCH the screen to begin fixing.\n");
    printf("\n");
    printf("\n");
    printf("Leave the screens flashing for an extended period of time (up to a few hours).\n");
    printf("\n");
    printf("\n");
    printf("HOLD down on the screen to display all black and\nfind stuck pixels.\n");
    printf("\n");
    printf("\n");
    printf("Press START to return to the homebrew menu.\n");
    printf("\n");
    printf("\n");
    printf("\t\t\t\t\t\tMade by Harrison Gieraltowski.");

    // wait for initial touch or exit
    while (aptMainLoop())
    {
        // scan all inputs
        hidScanInput();

        // get key presses
        u32 kDown = hidKeysDown();

        // exit to homebrew menu on start
        if (kDown & KEY_START)
        {
            gfxExit();
            return 0;
        }

        // exit to rest of program on touch
        if (kDown & KEY_TOUCH) break;
    }

    // exit and re-init graphics, only way to remove console that I could find
    gfxExit();
    gfxInitDefault();

    // track current display color
    int color = 0;

    // predefined colors in u32 form
    u32 black = C2D_Color32(0x00, 0x00, 0x00, 0xFF);
    u32 red   = C2D_Color32(0xFF, 0x00, 0x00, 0xFF);
    u32 green = C2D_Color32(0x00, 0xFF, 0x00, 0xFF);
    u32 blue  = C2D_Color32(0x00, 0x00, 0xFF, 0xFF);

    // main loop
    while (aptMainLoop())
    {
        // scan all inputs
        hidScanInput();

        // get key presses
        u32 kDown = hidKeysDown();

        // break on start to return to homebrew menu
        if (kDown & KEY_START) break;

        // get held keys
        u32 kHeld = hidKeysHeld();

        // begin the drawing frame
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

        // draw either black or cycle colors, depending on touch
        if (kHeld & KEY_TOUCH)
        {
            // display black
            drawColorOnScreen(top, black);
            drawColorOnScreen(bottom, black);
        }
        else
        {
            // display for 4 frames each, visually seems to be a good cycle speed
            switch (color)
            {
                case 0 ... 3:
                    // display red
                    drawColorOnScreen(top, red);
                    drawColorOnScreen(bottom, red);
                    ++color;
                    break;
                case 4 ... 7:
                    // display green
                    drawColorOnScreen(top, green);
                    drawColorOnScreen(bottom, green);
                    ++color;
                    break;
                case 8 ... 11:
                    // display blue
                    drawColorOnScreen(top, blue);
                    drawColorOnScreen(bottom, blue);
                    ++color;
                    break;
                default:
                    color = 0;
                    break;
            }
        }
        // end the drawing frame
        C3D_FrameEnd(0);
    }
    // exit services
    C2D_Fini();
    C3D_Fini();
    gfxExit();
    return 0;
}
