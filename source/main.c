#include <3ds.h>
#include <citro2d.h>
#include <stdio.h>

void drawColorOnScreen(C3D_RenderTarget* screen, u32 color)
{
    C2D_TargetClear(screen, color);
    C2D_SceneBegin(screen);
}

int main(int argc, char **argv)
{
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();

    C3D_RenderTarget* top    = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

    //Initialize console on top screen. Using NULL as the second argument tells the console library to use the internal console structure as current one
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
        //Scan all the inputs. This should be done once for each frame
        hidScanInput();

        //hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
        u32 kDown = hidKeysDown();

        if (kDown & KEY_START)
        {
            // Exit services
            gfxExit();
            return 0;
        }

        u32 kHeld = hidKeysHeld();

        if (kHeld & KEY_TOUCH) break;
    }

    gfxExit();
    gfxInitDefault();

    int color = 0;

    u32 black = C2D_Color32(0x00, 0x00, 0x00, 0xFF);
    u32 red   = C2D_Color32(0xFF, 0x00, 0x00, 0xFF);
    u32 green = C2D_Color32(0x00, 0xFF, 0x00, 0xFF);
    u32 blue  = C2D_Color32(0x00, 0x00, 0xFF, 0xFF);

    // Main loop
    while (aptMainLoop())
    {
        //Scan all the inputs. This should be done once for each frame
        hidScanInput();

        //hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
        u32 kDown = hidKeysDown();

        if (kDown & KEY_START) break; // break in order to return to hbmenu

        u32 kHeld = hidKeysHeld();

        // begin the frame
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

        // draw either black or cycle colors
        if (kHeld & KEY_TOUCH)
        {
            // display black
            drawColorOnScreen(top, black);
            drawColorOnScreen(bottom, black);
        }
        else
        {
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

        // end the frame
        C3D_FrameEnd(0);
    }

    // Exit services
    C2D_Fini();
    C3D_Fini();
    gfxExit();
    return 0;
}
