#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspgu.h>
#include <pspdebug.h>

#include <stdlib.h>

#include "input.h"
#include "ui.h"

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

/* v0.2.4: activation feedback */
static int activated_index = -1;
static int activated_frames = 0;

/* v0.2.5: cursor mode + hover hit test */
static int cursor_mode = 0;
static int hovered_index = -1;

/*
 * v0.2.6: test UI elements
 * v0.2.7: focus state owned by UIContainer
 */
#define ELEMENT_COUNT 6
#define ELEMENT_COLS  3

static UIElement elements[ELEMENT_COUNT];

static UIContainer container;

static void on_element_activate(UIElement *element)
{
    activated_index = element->id;
    activated_frames = 15;
}

static void init_elements(void)
{
    int positions[ELEMENT_COUNT][2] = {
        { 100,  80 },
        { 200,  80 },
        { 300,  80 },
        { 100, 160 },
        { 200, 160 },
        { 300, 160 }
    };

    for (int i = 0; i < ELEMENT_COUNT; i++)
    {
        elements[i].x = positions[i][0];
        elements[i].y = positions[i][1];
        elements[i].width = 60;
        elements[i].height = 40;

        elements[i].id = i;
        elements[i].enabled = 1;
        elements[i].focused = 0;

        elements[i].normal_color = 0xFF30405A;
        elements[i].focused_color = 0xFF8060FF;
        elements[i].activated_color = 0xFF00FF00;

        elements[i].on_activate = on_element_activate;
        elements[i].user_data = NULL;
    }
}

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
 * Draw rectangle (background / panel / cursor)
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

    vertices = (Vertex *)sceGuGetMemory(2 * sizeof(Vertex));

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

    init_elements();

    ui_container_init(
        &container,
        elements,
        ELEMENT_COUNT,
        ELEMENT_COLS
    );

    init_gu();

    while (running)
    {
        UIEvent event = input_update();

        if (event.type == UI_EVENT_EXIT)
        {
            running = 0;
        }

        /*
         * v0.2.7: Focus navigation delegated to UIContainer.
         */
        if (event.type == UI_EVENT_UP)
        {
            ui_container_move_focus(&container, UI_DIR_UP);
            cursor_mode = 0;
        }
        else if (event.type == UI_EVENT_DOWN)
        {
            ui_container_move_focus(&container, UI_DIR_DOWN);
            cursor_mode = 0;
        }
        else if (event.type == UI_EVENT_LEFT)
        {
            ui_container_move_focus(&container, UI_DIR_LEFT);
            cursor_mode = 0;
        }
        else if (event.type == UI_EVENT_RIGHT)
        {
            ui_container_move_focus(&container, UI_DIR_RIGHT);
            cursor_mode = 0;
        }
        else if (event.type == UI_EVENT_ACTIVATE)
        {
            int target;

            if (cursor_mode && hovered_index >= 0)
            {
                target = hovered_index;
            }
            else
            {
                target = ui_container_get_focus_index(&container);
            }

            if (target >= 0 && target < ELEMENT_COUNT)
            {
                ui_element_activate(&elements[target]);
            }
        }

        /*
         * v0.2.5: analog switches to cursor mode
         */
        if (event.cursor_moved)
        {
            cursor_mode = 1;
        }

        /*
         * v0.2.5: hit test cursor against elements
         */
        hovered_index = -1;

        for (int i = 0; i < ELEMENT_COUNT; i++)
        {
            if (event.cursor_x >= elements[i].x &&
                event.cursor_x <  elements[i].x + elements[i].width &&
                event.cursor_y >= elements[i].y &&
                event.cursor_y <  elements[i].y + elements[i].height)
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
        sceGuStart(GU_DIRECT, list);

        sceGuClearColor(0xFF101828);
        sceGuClear(GU_COLOR_BUFFER_BIT);

        draw_rectangle(
            40,
            35,
            400,
            200,
            0xFF18243A
        );

        /*
         * v0.2.7: active element
         */
        int active_index;

        if (cursor_mode && hovered_index >= 0)
        {
            active_index = hovered_index;
        }
        else
        {
            active_index = ui_container_get_focus_index(&container);
        }

        for (int i = 0; i < ELEMENT_COUNT; i++)
        {
            elements[i].focused = (i == active_index);
        }

        for (int i = 0; i < ELEMENT_COUNT; i++)
        {
            if (i == activated_index)
            {
                draw_rectangle(
                    elements[i].x,
                    elements[i].y,
                    elements[i].width,
                    elements[i].height,
                    elements[i].activated_color
                );
            }
            else
            {
                ui_element_draw(&elements[i]);
            }
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

        sceDisplayWaitVblankStart();

        sceGuSwapBuffers();
    }

    sceGuTerm();

    sceKernelExitGame();

    return 0;
}