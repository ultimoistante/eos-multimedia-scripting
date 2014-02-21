/*
-----------------------------------------------------------------------------
This source file is part of EOS Multimedia Scripting System
For the latest info, see https://github.com/ultimoistante/eos-multimedia-scripting

The MIT License (MIT)

Copyright (c) 2014-2019 of Salvatore Carotenuto aka ultimoistante of StartupSolutions
                          (mailto: ultimoistante@gmail.com | www.startupsolutions.it)
                       and Alessandro Rasulo (mailto: alex9978@gmail.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#include "EOSWeb.h"

// -----------------------------------------------------------------------------

bool EOSWeb::instanceFlag = false;
EOSWeb* EOSWeb::instance = NULL;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

EOSWeb* EOSWeb::getInstance()
    {
    if (!instanceFlag)
        {
        instance = new EOSWeb();
        instanceFlag = true;
        }
    return instance;
    }

// -----------------------------------------------------------------------------

bool EOSWeb::initialize()
    {
    printf("initialize >>>\n");
    int argc = 1;
    gtk_init(&argc, NULL);
    //
    //if (!this->offscreenWindow)
    this->offscreenWindow = create_offscreen_window(512, 512);
    // Create a WebView, set it transparent, add it to the window
    this->webView = WEBKIT_WEB_VIEW(webkit_web_view_new());
    //this->webView = webkit_web_view_new();
    gtk_container_add(GTK_CONTAINER(offscreenWindow), GTK_WIDGET(webView));
    //gtk_container_add (this->offscreenWindow, GTK_WIDGET(this->webView));
    // Attach a load status listener to a web view
    g_signal_connect(webView, "notify::load-status", G_CALLBACK(loadStatusCb), this->offscreenWindow);

    gtk_widget_show(offscreenWindow);
    //gtk_widget_queue_draw(GTK_WIDGET(this->webView)); 
    //GdkWindow* window = gtk_widget_get_window(GTK_WIDGET(this->webView)); 
    //gdk_window_process_updates(window, true); 

    printf("initialize offscrren ok\n");
    //
    buffer = NULL;
    initialized = true;
    printf("initialize <<<\n");
    }

// -----------------------------------------------------------------------------

void EOSWeb::update()
    {
    if (!initialized)
        return;

    //printf("*** u1\n");
    //gtk_widget_realize(offscreenWindow);
    gtk_widget_realize(GTK_WIDGET(this->webView));
    gtk_widget_queue_draw(GTK_WIDGET(this->webView));
    GdkWindow* window = gtk_widget_get_window(GTK_WIDGET(this->webView));
    gdk_window_process_updates(window, true);
    //printf("window: %p\n", window);

    gtk_main_iteration();
    //gtk_main();
    //
    //sharedImagedata.bufferUpdated = true;
    //convertBuffer(pixbuffer);
    //printf("*** u2\n");
    }

// -----------------------------------------------------------------------------

EOSObject* EOSWeb::load_webpage(const char *uri)
    {
    // Initialize module
    if (!initialized)
        initialize();

    // Create offscreen window
    //GtkWidget* offscreenWindow = create_offscreen_window(512, 512);
    //GtkWidget *scrolledWindow = gtk_scrolled_window_new(NULL, NULL);
    //GtkWidget *webView = webkit_web_view_new();

    // Create a WebView, set it transparent, add it to the window
    // WebKitWebView* webView = WEBKIT_WEB_VIEW(webkit_web_view_new());
    //webkit_web_view_set_transparent(webView, TRUE);
    //gtk_container_add (GTK_CONTAINER(offscreenWindow), GTK_WIDGET(webView));

    //gtk_container_add(GTK_CONTAINER(scrolledWindow), webView);
    //gtk_container_add(GTK_CONTAINER(offscreenWindow), scrolledWindow);


    // Load a web page into the browser instance
    //webkit_web_view_load_uri(WEBKIT_WEB_VIEW(webView), uri);
    webkit_web_view_open(WEBKIT_WEB_VIEW(webView), uri);
    // Load a default page
    //webkit_web_view_load_uri(this->webView, "http://www.google.it");

    // Make sure that when the browser area becomes visible, it will get mouse and keyboard events
    gtk_widget_grab_focus(GTK_WIDGET(webView));

    gtk_widget_realize(offscreenWindow);
    gtk_widget_show_all(offscreenWindow);

    pixbuffer = gtk_offscreen_window_get_pixbuf((GtkOffscreenWindow *) offscreenWindow);
    //cairo_surface_t *cairosrf = gtk_offscreen_window_get_surface((GtkOffscreenWindow *)offscreenWindow);

    
    //tthread::thread* _thread = new tthread::thread(threadRun, NULL);
    //createSessionObject(pixbuffer);
    //gtk_main();
    //return 0;
    return createSessionObject(pixbuffer);


    /* Create the widgets 
    GtkWidget *main_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    GtkWidget *scrolled_window = gtk_scrolled_window_new (NULL, NULL);
    GtkWidget *web_view = webkit_web_view_new ();

    // Attach a load status listener to a web view
    g_signal_connect(web_view, "notify::load-status", G_CALLBACK(loadStatusCb), scrolled_window);
    
    /* Place the WebKitWebView in the GtkScrolledWindow 
    gtk_container_add (GTK_CONTAINER (scrolled_window), web_view);
    gtk_container_add (GTK_CONTAINER (main_window), scrolled_window);

    /* Open a webpage 
    webkit_web_view_load_uri (WEBKIT_WEB_VIEW (web_view), "http://acid3.acidtests.org/");

    /* Show the result 
    gtk_window_set_default_size (GTK_WINDOW (main_window), 800, 600);
    gtk_widget_show_all (main_window);       
    
    return NULL;*/
    }

// -----------------------------------------------------------------------------

// Implement the listener, handling each possible load status

void EOSWeb::loadStatusCb(WebKitWebView *web_view, GParamSpec *pspec, void* context)
    {
    WebKitLoadStatus status = webkit_web_view_get_load_status(web_view);
    GObject *object = G_OBJECT(web_view);
    //g_assert(web_view == context->m_WebView);
    g_object_freeze_notify(object);
    switch (status)
        {
        case WEBKIT_LOAD_FINISHED:
            printf("WEBKIT_LOAD_FINISHED");
            //handleLoadFinished(web_view, context);
            break;
        case WEBKIT_LOAD_PROVISIONAL:
            printf("WEBKIT_LOAD_PROVISIONAL");
            //handleLoadProvisional(web_view, context);
            break;
        case WEBKIT_LOAD_COMMITTED:
            printf("WEBKIT_LOAD_COMMITTED");
            //handleLoadCommitted(web_view, context);
            break;
        case WEBKIT_LOAD_FIRST_VISUALLY_NON_EMPTY_LAYOUT:
            printf("WEBKIT_LOAD_FIRST_VISUALLY_NON_EMPTY_LAYOUT");
            //handleLoadNonEmptyLayout(web_view, context);
            break;
        case WEBKIT_LOAD_FAILED:
            printf("WEBKIT_LOAD_FAILED");
            //handleLoadFailed(web_view, context);
            break;
        default:
            break;
        }
    g_object_thaw_notify(object);
    //
    gtk_widget_realize(EOSWeb::getInstance()->offscreenWindow);
    gtk_widget_queue_draw(GTK_WIDGET(EOSWeb::getInstance()->webView));
    GdkWindow* window = gtk_widget_get_window(GTK_WIDGET(EOSWeb::getInstance()->webView));
    gdk_window_process_updates(window, true);

    EOSWeb::getInstance()->pixbuffer = gtk_offscreen_window_get_pixbuf((GtkOffscreenWindow *) EOSWeb::getInstance()->offscreenWindow);
    EOSWeb::getInstance()->convertBuffer(EOSWeb::getInstance()->pixbuffer);
    }

// --------------------------------------------------------------------

GtkWidget* EOSWeb::create_offscreen_window(int width, int height)
    {
    GtkWidget *offscr = gtk_offscreen_window_new();
    gtk_window_set_default_size((GtkWindow *) offscr, width, height);
    return offscr;
    }

// --------------------------------------------------------------------

EOSObject* EOSWeb::createSessionObject(GdkPixbuf *pixbuf)
    {
    printf("*** x\n");
    EOSObject* sessionObject = NULL;
    uint length = 0;
    if (pixbuf != NULL)
        {
        printf("*** y\n");
        convertBuffer(pixbuf);

        //int width = cairo_image_surface_get_width(cairosrf);
        //int height = cairo_image_surface_get_height(cairosrf);
        //unsigned char* data = cairo_image_surface_get_data(cairosrf);

        int width = gdk_pixbuf_get_width(pixbuf);
        int height = gdk_pixbuf_get_height(pixbuf);

        this->sharedImagedata.imageType = EOS_IMAGETYPE_RGBA_BUFFER;
        this->sharedImagedata.width = width;
        this->sharedImagedata.height = height;
        this->sharedImagedata.buffer = buffer;
        this->sharedImagedata.bufferSize = width * height * 4;
        this->sharedImagedata.bufferStatic = false;
        this->sharedImagedata.glTextureHandle = 0;
        //        
        sessionObject = new EOSObject();
        sessionObject->setUserData(&sharedImagedata);
        sessionObject->setUserDataSize(sizeof (eos_imagedata));
        //
        //this->sessionObjects.push_back(sessionObject);
        }
    printf("*** z\n");
    return sessionObject;
    }

// --------------------------------------------------------------------

void EOSWeb::convertBuffer(GdkPixbuf* pixbuf)
    {
    int width = gdk_pixbuf_get_width(pixbuf);
    int height = gdk_pixbuf_get_height(pixbuf);
    int rowstride = gdk_pixbuf_get_rowstride(pixbuf);
    unsigned char *pixels = gdk_pixbuf_get_pixels(pixbuf);

    if (buffer == NULL)
        buffer = new unsigned char[width * height * 4];

    for (int y = 0; y < height; y++)
        {
        unsigned char *src = pixels + (y * rowstride);
        unsigned char *dst = buffer + (y * width * 4);
        int xoff = 0;
        for (int x = 0; x < width; x++)
            {
            dst[x * 4 + 0] = src[x * 3 + 0];
            dst[x * 4 + 1] = src[x * 3 + 1];
            dst[x * 4 + 2] = src[x * 3 + 2];
            dst[x * 4 + 3] = 255;
            }
        }

    this->sharedImagedata.bufferUpdated = true;
    }


// --------------------------------------------------------------------
// --------------------------------------------------------------------

void EOSWeb::threadRun(void* params)
    {
    gtk_main();
    //tthread::this_thread::sleep_for(tthread::chrono::milliseconds(16));
    }
