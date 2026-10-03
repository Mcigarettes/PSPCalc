#include <pspkernel.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <pspgu.h>
#include <pspdebug.h>

#include <stdlib.h>

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

/*
 * Cursor
 */
static float cursor_x = 240.0f;
static float cursor_y = 136.0f;

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

    framebuffer0 =
        guGetStaticVramBuffer(
            BUFFER_WIDTH,
            SCREEN_HEIGHT,
            GU_PSM_8888
        );

    framebuffer1 =
        guGetStaticVramBuffer(
            BUFFER_WIDTH,
            SCREEN_HEIGHT,
            GU_PSM_8888
        );

    depthbuffer =
        guGetStaticVramBuffer(
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
    SceCtrlData pad;

    setup_callbacks();

    /*
     * Initialize controller.
     */
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

    /*
     * Initialize graphics.
     */
    init_gu();

    while (running)
    {
        /*
         * Read controller.
         */
        sceCtrlPeekBufferPositive(
            &pad,
            1
        );

        /*
         * START exits the program.
         */
        if (pad.Buttons & PSP_CTRL_START)
        {
            running = 0;
        }

        /*
         * Analog stick.
         *
         * Center is approximately 128.
         */
        float dx = (float)pad.Lx - 128.0f;
        float dy = (float)pad.Ly - 128.0f;

        /*
         * Dead zone.
         */
        if (dx > -15.0f && dx < 15.0f)
        {
            dx = 0.0f;
        }

        if (dy > -15.0f && dy < 15.0f)
        {
            dy = 0.0f;
        }

        /*
         * Movement speed.
         */
        cursor_x += dx * 0.08f;
        cursor_y += dy * 0.08f;

        /*
         * Keep cursor on screen.
         */
        if (cursor_x < 10.0f)
            cursor_x = 10.0f;

        if (cursor_x > SCREEN_WIDTH - 10.0f)
            cursor_x = SCREEN_WIDTH - 10.0f;

        if (cursor_y < 10.0f)
            cursor_y = 10.0f;

        if (cursor_y > SCREEN_HEIGHT - 10.0f)
            cursor_y = SCREEN_HEIGHT - 10.0f;

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
         * Draw the cursor.
         */
        draw_rectangle(
            (int)cursor_x - 8,
            (int)cursor_y - 8,
            16,
            16,
            0xFF8060FF
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