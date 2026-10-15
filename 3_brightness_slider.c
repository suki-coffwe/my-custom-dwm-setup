#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#define WIDTH 360
#define HEIGHT 45
#define XPOS 1366 - WIDTH - 8
#define YPOS 0
#define SLIDER_X 24
#define SLIDER_W 312
//#define SLIDER_Y 62
#define SLIDER_Y 30

static Display *dpy;
static Window win;
static GC gc;
static double brightness = 1;
static int dragging = 0;
static int last_step = -1;

static unsigned long black, white;

static void
set_brightness(double value)
{
    char buf[16];

    snprintf(buf, sizeof buf, "%.2f", value);

    if (fork() == 0) {
        execlp("xrandr", "xrandr",
               "--output", "HDMI-2",
               "--brightness", buf,
               (char *)NULL);
        _exit(127);
    }
}

static int
slider_position(void)
{
    return SLIDER_X +
        (int)(((brightness - 0.10) / 0.90) * SLIDER_W);
}

static void
draw(void)
{
    char text[64];
    int x = slider_position();

    XClearWindow(dpy, win);

    XSetForeground(dpy, gc, white);

    snprintf(text, sizeof text, "Brightness: %.2f", brightness);
    XDrawString(dpy, win, gc, 24, 15, text, strlen(text));

    /* Slider track */
    XDrawLine(dpy, win, gc,
              SLIDER_X, SLIDER_Y,
              SLIDER_X + SLIDER_W, SLIDER_Y);

    /* Filled portion */
    XFillRectangle(dpy, win, gc,
                   SLIDER_X, SLIDER_Y - 2,
                   x - SLIDER_X, 5);

    /* Slider handle */
    XFillArc(dpy, win, gc,
             x - 7, SLIDER_Y - 7,
             14, 14, 0, 360 * 64);

    XFlush(dpy);
}

static void
update_slider(int mouse_x)
{
    double value;
    int step;

    if (mouse_x < SLIDER_X)
        mouse_x = SLIDER_X;

    if (mouse_x > SLIDER_X + SLIDER_W)
        mouse_x = SLIDER_X + SLIDER_W;

    value = 0.10 +
        0.90 * (mouse_x - SLIDER_X) / SLIDER_W;

    /* 0.01 increments */
    step = (int)(value * 100.0 + 0.5);

    if (step < 10)
        step = 10;
    if (step > 100)
        step = 100;

    brightness = step / 100.0;

    if (step != last_step) {
        last_step = step;
        set_brightness(brightness);
    }

    draw();
}

int
main(void)
{
    XEvent ev;
    XSizeHints hints;

    signal(SIGCHLD, SIG_IGN);

    dpy = XOpenDisplay(NULL);
    if (!dpy) {
        fprintf(stderr, "Cannot open X display\n");
        return 1;
    }

    int screen = DefaultScreen(dpy);

    black = BlackPixel(dpy, screen);
    white = WhitePixel(dpy, screen);

    win = XCreateSimpleWindow(
        dpy, RootWindow(dpy, screen),
        XPOS, YPOS, WIDTH, HEIGHT, 1,
        white, black
    );

XSetWindowAttributes attr;
attr.override_redirect = True;

XChangeWindowAttributes(
    dpy, win,
    CWOverrideRedirect,
    &attr
);

    XStoreName(dpy, win, "Brightness");

    hints.flags = PMinSize | PMaxSize;
    hints.min_width = hints.max_width = WIDTH;
    hints.min_height = hints.max_height = HEIGHT;
    XSetWMNormalHints(dpy, win, &hints);

    XSelectInput(dpy, win,
        ExposureMask |
        ButtonPressMask |
        ButtonReleaseMask |
        PointerMotionMask |
        KeyPressMask
    );

    gc = XCreateGC(dpy, win, 0, NULL);
    XSetForeground(dpy, gc, white);

//
/* Start hidden */
/* Hide */
XUnmapWindow(dpy, win);

/* Show */
//
//    XMapWindow(dpy, win);

    while (1) {
        XNextEvent(dpy, &ev);

        switch (ev.type) {
        case Expose:
            if (ev.xexpose.count == 0)
                draw();
            break;

        case ButtonPress:
            if (ev.xbutton.button == Button1) {
                dragging = 1;
                update_slider(ev.xbutton.x);
            }
            break;

        case MotionNotify:
            if (dragging)
                update_slider(ev.xmotion.x);
            break;

        case ButtonRelease:
            if (ev.xbutton.button == Button1)
                dragging = 0;
            break;

        case KeyPress:
            if (XLookupKeysym(&ev.xkey, 0) == XK_q) {
                XCloseDisplay(dpy);
                return 0;
            }
            break;
        }
    }

    XCloseDisplay(dpy);
    return 0;
}
