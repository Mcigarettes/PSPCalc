#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspgu.h>
#include <pspdebug.h>

#include <stdlib.h>

#include "input.h"

/*
 * PSP application information
 */
PSP_MODULE_INFO("PSPCalc", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

/*
 * Screen
 */
#define SCREEN_WIDTH  480
#define SCREEN_HEIGHT 272

/*
 * GU framebuffer width.
 * PSP VRAM is normally arranged with a 512-pixel buffer width.
 */
#define BUFFER_WIDTH 512

/*
 * GU display list
 */
static unsigned int __attribute__((aligned(16))) list[262144];

/*
 * Application state
 */
static volatile int running = 1;
static int focus_index = 0;

/* v0.2.4: activation feedback */
static int activated_index = -1;
static int activated_frames = 0;

/* v0.2.5: cursor mode + hover hit test */
static int cursor_mode = 0;      /* 1 = last input was analog cursor */
static int hovered_index = -1;   /* element under cursor, or -1 */

/*
 * Test button layout (shared by hit test and drawing)
 */
static const int button_x[6] = { 100, 200, 300, 100, 200, 300 };
static const int button_y[6] = {  80,  80,  80, 160, 160, 160 };
static const int button_w = 60;
static const int button_h = 40;
static const int button_count = 6;

/*
 * ---------------------------------------------------------
 * Exit callback
 * ---------------------------------------------------------
 */

static int exit_callback(int arg1, int arg2, void *common)
{
    running = 0;
    return 0;
}

static int callback_thread(SceSize args, void *argp)
{
    int callback_id;

    callback_id = sceKernelCreateCallback(
        "Exit Callback",
        exit_callback,
        NULL
    );

    if (callback_id >= 0)
    {
        sceKernelRegisterExitCallback(callback_id);
    }

    sceKernelSleepThreadCB();

    return 0;
}

static int setup_callbacks(void)
{
    int thread_id;

    thread_id = sceKernelCreateThread(
        "CallbackThread",
        callback_thread,
        0x11,
        0xFA0,
        0,
        NULL
    );

    if (thread_id >= 0)
    {
        sceKernelStartThread(thread_id, 0, NULL);
    }

    return thread_id;
}

/*
 * ---------------------------------------------------------
 * Graphics initialization
 * ---------------------------------------------------------
 */

static void init_gu(void)
{
    void *framebuffer0;
    void *framebuffer1;
    void *depthbuffer;

    framebuffer0 = guGetStaticVramBuffer(
        BUFFER_WIDTH,
        SCREEN_HEIGHT,
        GU_PSM_8888
    );

    framebuffer1 = guGetStaticVramBuffer(
        BUFFER_WIDTH,
        SCREEN_HEIGHT,
        GU_PSM_8888
    );

    depthbuffer = guGetStaticVramBuffer(
        BUFFER_WIDTH,
        SCREEN_HEIGHT,
        GU_PSM_4444
    );

    sceGuInit();

    sceGuStart(GU_DIRECT, list);

    sceGuDrawBuffer(
        GU_PSM_8888,
        framebuffer0,
        BUFFER_WIDTH
    );

    sceGuDispBuffer(
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        framebuffer1,
        BUFFER_WIDTH
    );

    sceGuDepthBuffer(
        depthbuffer,
        BUFFER_WIDTH
    );

    /*
     * Set up a 480x272 coordinate system.
     */
    sceGuOffset(
        2048 - SCREEN_WIDTH / 2,
        2048 - SCREEN_HEIGHT / 2
    );

    sceGuViewport(
        2048,
        2048,
        SCREEN_WIDTH,
        SCREEN_HEIGHT
    );

    sceGuDepthRange(
        65535,
        0
    );

    sceGuScissor(
        0,
        0,
        SCREEN_WIDTH,
        SCREEN_HEIGHT
    );

    sceGuEnable(GU_SCISSOR_TEST);

    sceGuDisable(GU_DEPTH_TEST);

    sceGuFinish();

    sceGuSync(
        GU_SYNC_FINISH,
        GU_SYNC_WHAT_DONE
    );

    sceDisplayWaitVblankStart();

    sceGuDisplay(GU_TRUE);
}

/*
 * ---------------------------------------------------------
 * Draw rectangle
 * ---------------------------------------------------------
 */

typedef struct
{
    unsigned int color;
    short x;
    short y;
    short z;
} Vertex;

static void draw_rectangle(
    int x,
    int y,
    int width,
    int height,
    unsigned int color
)
{
    Vertex *vertices;

    vertices = (Vertex *)sceGuGetMemory(
        2 * sizeof(Vertex)
    );

    vertices[0].color = color;
    vertices[0].x = x;
    vertices[0].y = y;
    vertices[0].z = 0;

    vertices[1].color = color;
    vertices[1].x = x + width;
    vertices[1].y = y + height;
    vertices[1].z = 0;

    sceGuDrawArray(
        GU_SPRITES,
        GU_COLOR_8888 |
        GU_VERTEX_16BIT |
        GU_TRANSFORM_2D,
        2,
        NULL,
        vertices
    );
}

/*
 * ---------------------------------------------------------
 * Main
 * ---------------------------------------------------------
 */

int main(void)
{
    setup_callbacks();

    input_init();

    /*
     * Initialize graphics.
     */
    init_gu();

    while (running)
    {
        UIEvent event = input_update();

        /*
         * Exit
         */
        if (event.type == UI_EVENT_EXIT)
        {
            running = 0;
        }

        /*
         * Focus navigation (v0.2.3) + activate (v0.2.4/v0.2.5)
         */
        if (event.type == UI_EVENT_UP)
        {
            if (focus_index >= 3)
            {
                focus_index -= 3;
            }
            cursor_mode = 0;
        }
        else if (event.type == UI_EVENT_DOWN)
        {
            if (focus_index < 3)
            {
                focus_index += 3;
            }
            cursor_mode = 0;
        }
        else if (event.type == UI_EVENT_LEFT)
        {
            if (focus_index % 3 != 0)
            {
                focus_index--;
            }
            cursor_mode = 0;
        }
        else if (event.type == UI_EVENT_RIGHT)
        {
            if (focus_index % 3 != 2)
            {
                focus_index++;
            }
            cursor_mode = 0;
        }
        else if (event.type == UI_EVENT_ACTIVATE)
        {
            /* v0.2.5: unified activation */
            int target;

            if (cursor_mode && hovered_index >= 0)
            {
                target = hovered_index;
            }
            else
            {
                target = focus_index;
            }

            activated_index = target;
            activated_frames = 15;
        }

        /*
         * v0.2.5: analog stick switches active input to cursor mode
         */
        if (event.cursor_moved)
        {
            cursor_mode = 1;
        }

        /*
         * v0.2.5: hit test cursor against buttons
         */
        hovered_index = -1;

        for (int i = 0; i < button_count; i++)
        {
            if (event.cursor_x >= button_x[i] &&
                event.cursor_x <  button_x[i] + button_w &&
                event.cursor_y >= button_y[i] &&
                event.cursor_y <  button_y[i] + button_h)
            {
                hovered_index = i;
                break;
            }
        }

        /*
         * v0.2.4: tick activation flash timer
         */
        if (activated_frames > 0)
        {
            activated_frames--;

            if (activated_frames == 0)
            {
                activated_index = -1;
            }
        }

        /*
         * Start a new frame.
         */
        sceGuStart(
            GU_DIRECT,
            list
        );

        /*
         * Background.
         *
         * RGBA:
         * 0xFF101828
         */
        sceGuClearColor(
            0xFF101828
        );

        sceGuClear(
            GU_COLOR_BUFFER_BIT
        );

        /*
         * Draw a test panel.
         */
        draw_rectangle(
            40,
            35,
            400,
            200,
            0xFF18243A
        );

        /*
         * v0.2.5: active element = hovered (cursor mode) or focused (d-pad mode)
         */
        int active_index;

        if (cursor_mode && hovered_index >= 0)
        {
            active_index = hovered_index;
        }
        else
        {
            active_index = focus_index;
        }

        for (int i = 0; i < button_count; i++)
        {
            unsigned int button_color;

            if (i == activated_index)
            {
                button_color = 0xFF00FF00;   /* activated: green flash */
            }
            else if (i == active_index)
            {
                button_color = 0xFF8060FF;   /* active: purple */
            }
            else
            {
                button_color = 0xFF30405A;   /* normal */
            }

            draw_rectangle(
                button_x[i],
                button_y[i],
                button_w,
                button_h,
                button_color
            );
        }

        /*
         * Draw the cursor.
         */
        unsigned int cursor_color = 0xFF8060FF;

        if (event.type == UI_EVENT_ACTIVATE)
        {
            cursor_color = 0xFF00FF00;
        }
        else if (event.type == UI_EVENT_BACK)
        {
            cursor_color = 0xFFFF0000;
        }

        draw_rectangle(
            (int)event.cursor_x - 8,
            (int)event.cursor_y - 8,
            16,
            16,
            cursor_color
        );

        /*
         * Finish frame.
         */
        sceGuFinish();

        sceGuSync(
            GU_SYNC_FINISH,
            GU_SYNC_WHAT_DONE
        );

        /*
         * Wait for vertical blank.
         */
        sceDisplayWaitVblankStart();

        /*
         * Swap buffers.
         */
        sceGuSwapBuffers();
    }

    sceGuTerm();

    sceKernelExitGame();

    return 0;
}