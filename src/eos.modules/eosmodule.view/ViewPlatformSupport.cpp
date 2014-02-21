
#include "ViewPlatformSupport.h"
#include "../../eos.kernel/EOSKernel.h"

// -----------------------------------------------------------------------------

int ViewPlatformSupport::getScreenCount()
    {
    Display *testDisplay = XOpenDisplay(NULL);
    int count = 0;
    
	if(testDisplay)
        {
        count = XScreenCount(testDisplay);
        XCloseDisplay(testDisplay);
        }
    
    return count;
    }

// -----------------------------------------------------------------------------

bool ViewPlatformSupport::makeWindow(int screenNum, const char *name, int x, int y, int width, int height, bool fullscreen, ViewWindowData *winData)
    {

#if defined linux

    static int attrListSingle[] ={
        GLX_RGBA,
        GLX_DEPTH_SIZE, 16,
        GLX_RED_SIZE, 4,
        GLX_GREEN_SIZE, 4,
        GLX_BLUE_SIZE, 4,
        None
        };

    static int attrListDouble[] ={
        GLX_RGBA,
        GLX_DOUBLEBUFFER,
        GLX_DEPTH_SIZE, 16,
        GLX_RED_SIZE, 4,
        GLX_GREEN_SIZE, 4,
        GLX_BLUE_SIZE, 4,
        None
        };

    XVisualInfo *vi;
    Colormap cmap;
    X11::XF86VidModeModeInfo **modes;
    int numModes;
    int bestMode;
    Atom wmDelete;
    int dpyWidth, dpyHeight;
    int i;
    char displayName[64];

    winData->windowTitle = (char *) name;
    winData->xpos = x;
    winData->ypos = y;
    winData->width = width;
    winData->height = height;
    winData->fullscreen = fullscreen;
    winData->toBeResized = false;
    winData->toBeClosed = false; // TODO: to remove (only for development)

    // set best mode to current
    bestMode = 0;

    // open connection
    snprintf(displayName, 64, ":0.%i", screenNum);    
    winData->display = XOpenDisplay(displayName);
    if(!winData->display)
        {
        LOGERROR("[eos.view.window.create] error: couldn't open display!");
        return False;
        }

    winData->screen = DefaultScreen(winData->display);

    X11::XF86VidModeGetAllModeLines(winData->display, winData->screen, &numModes, &modes);

    // save current desktop resolution
    winData->desktopMode = *modes[0];

    // look for best mode at requested resolution
    LOGINFO("[eos.view.window] List available video modes:");
    for(i = 0; i < numModes; ++i)
        {           
        if((modes[i]->hdisplay == width) && (modes[i]->vdisplay == height))
            {
            LOGINFO("    mode %i - %ix%i  <--  selected", i, modes[i]->hdisplay, modes[i]->vdisplay);
            bestMode = i;            
            }
        else
            LOGINFO("    mode %i - %ix%i", i, modes[i]->hdisplay, modes[i]->vdisplay);
        }        

    // get an appropriate visual
    vi = glXChooseVisual(winData->display, DefaultScreen(winData->display), attrListDouble);
    if(!vi)
        {
        // try to get single buffered vidual
        vi = glXChooseVisual(winData->display, DefaultScreen(winData->display), attrListSingle);
        if(!vi)
            {
            // failed to get both double and single buffered visual
            LOGERROR("[eos.view.window] error: couldn't open visual!");
            XCloseDisplay(winData->display);
            return false;
            }
        else
            {
            LOGWARNING("[eos.view.window] only singlebuffered visual!");
            winData->doubleBuffered = false;
            }
        }
    else
        winData->doubleBuffered = true;


    // create an OpenGL rendering context
    winData->context_pri = glXCreateContext(winData->display, vi, 0, true);
    winData->context_sec = glXCreateContext(winData->display, vi, winData->context_pri, true);
    if(!winData->context_pri || !winData->context_sec)
        {
        LOGERROR("[eos.view.window] error: failed to create a rendering context!");
        XDestroyWindow(winData->display, winData->window);
        XCloseDisplay(winData->display);
        return false;
        }

    // get parent (root) window
    winData->parent = RootWindow(winData->display, vi->screen);

    // create a color map
    cmap = XCreateColormap(winData->display, winData->parent, vi->visual, AllocNone);

    winData->attributes.colormap = cmap;
    winData->attributes.border_pixel = 0;

    if(winData->fullscreen)
        {
        X11::XF86VidModeSwitchToMode(winData->display, winData->screen, modes[bestMode]);
        X11::XF86VidModeSetViewPort(winData->display, winData->screen, 0, 0);
        winData->width = modes[bestMode]->hdisplay;
        winData->height = modes[bestMode]->vdisplay;
        XFree(modes);

        // create a fullscreen window
        winData->attributes.override_redirect = True;
        winData->attributes.event_mask = StructureNotifyMask | VisibilityChangeMask | FocusChangeMask;
        //ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask;

        winData->window = XCreateWindow(winData->display, winData->parent, 0, 0, winData->width, winData->height,
                                        0, vi->depth, InputOutput, vi->visual,
                                        CWBorderPixel | CWColormap | CWEventMask | CWOverrideRedirect,
                                        &winData->attributes);

        XMapRaised(winData->display, winData->window);
        }
    else
        {
        winData->attributes.event_mask = ExposureMask | StructureNotifyMask;

        // create a simple window
        winData->window = XCreateWindow(winData->display, winData->parent, 0, 0, winData->width, winData->height,
                                        0, vi->depth, InputOutput, vi->visual,
                                        CWBorderPixel | CWColormap | CWEventMask,
                                        &winData->attributes);

        // handle wm_delete_events
        wmDelete = XInternAtom(winData->display, "WM_DELETE_WINDOW", true);
        XSetWMProtocols(winData->display, winData->window, &wmDelete, 1);
        XSetStandardProperties(winData->display, winData->window, winData->windowTitle, winData->windowTitle, None, NULL, 0, NULL);
        XMapRaised(winData->display, winData->window);
        }

    // disable screen saver
    XForceScreenSaver(winData->display, ScreenSaverReset);

    // connect the glx context to the window
    if(!glXMakeCurrent(winData->display, winData->window, winData->context_pri))
        {
        LOGERROR("[eos.view] failed to connext glx context to the window!");
        glXDestroyContext(winData->display, winData->context_pri);
        XDestroyWindow(winData->display, winData->window);

        if(winData->fullscreen)
            {
            X11::XF86VidModeSwitchToMode(winData->display, winData->screen, &winData->desktopMode);
            X11::XF86VidModeSetViewPort(winData->display, winData->screen, 0, 0);
            }

        XCloseDisplay(winData->display);
        return false;
        }

    if(!glXIsDirect(winData->display, winData->context_pri))
        LOGWARNING("[eos.view] no direct rendering possible");

    // duplicate context params
    //glXCopyContext(winData->display, winData->context_pri, winData->context_sec, GL_ALL_ATTRIB_BITS);

#elif defined _WIN32

#endif
    return ViewUtility::checkGL("window.create / makeWindow");
    }

// -----------------------------------------------------------------------------

void ViewPlatformSupport::destroyWindow(ViewWindowData* winData)
    {
#if defined linux    
    glXMakeCurrent(winData->display, None, NULL);
    glXDestroyContext(winData->display, winData->context_pri);
    glXDestroyContext(winData->display, winData->context_sec);
    if(winData->fullscreen)
        {
        X11::XF86VidModeSwitchToMode(winData->display, winData->screen, &winData->desktopMode);
        X11::XF86VidModeSetViewPort(winData->display, winData->screen, 0, 0);
        }
    XDestroyWindow(winData->display, winData->window);
    XCloseDisplay(winData->display);
    XForceScreenSaver(winData->display, ScreenSaverActive);

#elif defined _WIN32

#endif
    ViewUtility::checkGL("window.close / destroyWindow");
    }

// -----------------------------------------------------------------------------

void ViewPlatformSupport::processWindowEvents(ViewWindowData* winData)
    {
#if defined linux
    XEvent event;

    while(XPending(winData->display))
        {
        XNextEvent(winData->display, &event);

        switch(event.type)
        {
            case ConfigureNotify:
                if((event.xconfigure.width != winData->width) && (event.xconfigure.height != winData->height))
                    {
                    winData->width = event.xconfigure.width;
                    winData->height = event.xconfigure.height;
                    winData->toBeResized = true;
                    }
                break;

            case ClientMessage:
                if(*XGetAtomName(winData->display, event.xclient.message_type) == *"WM_PROTOCOLS")
                    {
                    winData->toBeClosed = true;
                    }
                break;
        }
        }
#elif defined _WIN32

#endif
    }

// -----------------------------------------------------------------------------

void ViewPlatformSupport::setWindowCaption(ViewWindowData* winData)
    {
#if defined linux
    XTextProperty textproperty;
    XStringListToTextProperty(&winData->windowTitle, 1, &textproperty);
    XSetWMName(winData->display, winData->window, &textproperty);
#elif defined _WIN32

#endif
    }

// -----------------------------------------------------------------------------

void ViewPlatformSupport::swapBuffers(ViewWindowData* winData, bool waitVSync)
    {
#if defined linux

    if(waitVSync && GLEE_GLX_SGI_video_sync)
        {
        uint retraceCount;
        glXGetVideoSyncSGI(&retraceCount);
        glXWaitVideoSyncSGI(2, (retraceCount + 1) & 1, &retraceCount);
        }
    glXSwapBuffers(winData->display, winData->window);

#elif defined _WIN32

#endif
    ViewUtility::checkGL("window.update / swapBuffers");
    }

// -----------------------------------------------------------------------------

void ViewPlatformSupport::setContext(ViewWindowData* winData, int contextId)
    {
    switch(contextId)
    {
        case 1:
#if defined linux            
            glXMakeCurrent(winData->display, winData->window, winData->context_pri);
#elif defined _WIN32
#endif
            break;
        case 2:
#if defined linux
            glXMakeCurrent(winData->display, winData->window, winData->context_sec);
#elif defined _WIN32
#endif
            break;
    }
    ViewUtility::checkGL("window / setContext");
    }

// -----------------------------------------------------------------------------

bool ViewPlatformSupport::makePBO(ViewSurfaceData *srfData)
    {
    if(GLEE_ARB_vertex_buffer_object)
        {
        glGenBuffersARB(2, srfData->pbo_ids);
        glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, srfData->pbo_ids[0]);
        glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, srfData->bufferSize, 0, GL_STREAM_DRAW_ARB);
        glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, srfData->pbo_ids[1]);
        glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, srfData->bufferSize, 0, GL_STREAM_DRAW_ARB);
        glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
        ViewUtility::checkGL("surface.create / makePBO");
        return true;
        }
    return false;
    }

// -----------------------------------------------------------------------------

void ViewPlatformSupport::destroyPBO(ViewSurfaceData *srfData)
    {
    if(GLEE_ARB_vertex_buffer_object)
        glDeleteBuffersARB(2, srfData->pbo_ids);
    ViewUtility::checkGL("surface.delete / destroyPBO");
    }
