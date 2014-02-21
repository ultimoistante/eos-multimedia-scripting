
#include "View.h"

// -----------------------------------------------------------------------------

bool View::instanceFlag = false;
View* View::instance = NULL;

// -----------------------------------------------------------------------------

View::View()
    {
    currentWindow = NULL;
    }

// -----------------------------------------------------------------------------

View::~View()
    {
    // delete surfaces
    std::vector<ViewSurface *>::iterator itv;
    for(itv = surfaces.begin(); itv != surfaces.end(); itv++)
        delete *itv;
    surfaces.clear();

    // delete windows
    std::map<ulong, ViewWindow *>::iterator itm;
    for(itm = windows.begin(); itm != windows.end(); itm++)
        delete itm->second;
    windows.clear();
    }

// -----------------------------------------------------------------------------

View* View::getInstance()
    {
    if(!instanceFlag)
        {
        instance = new View();
        instanceFlag = true;
        }
    return instance;
    }

// -----------------------------------------------------------------------------

void View::shutdown()
    {
    if(instance)
        delete instance;

    instanceFlag = false;
    }


// -----------------------------------------------------------------------------

uint View::getScreenCount()
    {
    return ViewPlatformSupport::getScreenCount();
    }

// -----------------------------------------------------------------------------

bool View::getScreenInfo(uint num, ViewScreenInfo* si)
    {
    return false; // TODO: implement multi-monitor support
    }

// -----------------------------------------------------------------------------

ulong View::createWindow(uint display, uint width, uint height, uint bpp, uint antialiasing, bool fullscreen)
    {
    if(this->windows.size() > 0)
        {
        LOGWARNING("[eos.view.windowmanager.createWindow()] Multiple windows not supported.");
        return -1;
        }
    //
    ViewWindow* newWindow = new ViewWindow(display, width, height, bpp, antialiasing, fullscreen);
    if(newWindow != NULL)
        {
        newWindow->open();
        windows.insert(std::pair<ulong, ViewWindow *>(newWindow->getId(), newWindow));
        char windowCaption[65];
        snprintf(windowCaption, 64, "EOS Window - id: %d", this->windows.size());
        newWindow->setCaption(windowCaption);
        //
        return newWindow->getId();
        }
    return 0;
    }

// -----------------------------------------------------------------------------

bool View::updateWindow(ulong windowId)
    {
    ViewWindow *wnd = getWindowFromId(windowId);
    if(wnd != NULL)
        return wnd->update();

    return false;
    }

// -----------------------------------------------------------------------------

ViewWindow* View::getWindowFromId(ulong windowId)
    {
    std::map<ulong, ViewWindow*>::iterator it;
    it = windows.find(windowId);
    if(it != windows.end())
        return it->second;
    else
        return NULL;
    }

// -----------------------------------------------------------------------------

void View::setCurrentWindow(ulong windowId)
    {
    ViewWindow *wnd = getWindowFromId(windowId);
    if(wnd != NULL)
        currentWindow = wnd;
    }

// -----------------------------------------------------------------------------

ViewWindow* View::getCurrentWindow()
    {
    if(currentWindow == NULL)
        {
        std::map<ulong, ViewWindow*>::iterator it;
        it = windows.begin();
        if(it != windows.end())
            currentWindow = it->second;
        }

    return currentWindow;
    }

// -----------------------------------------------------------------------------

uint View::createSurface()
    {
    // create a subsurface
    uint surfaceId = createSubSurface();
    // register in the current window
    if(surfaceId > 0)
        {
        ViewWindow *wnd = getCurrentWindow();
        wnd->registerSurface(surfaceId);
        }

    return surfaceId;
    }

// -----------------------------------------------------------------------------

uint View::createSubSurface()
    {
    uint surfaceId = 0;

    ViewSurface* newSurface = new ViewSurface();
    if(newSurface != NULL)
        {
        // take the window context
        ViewWindow *wnd = getCurrentWindow();
        if(wnd == NULL) return 0;
        wnd->takeContext();

        // create surface
        if(newSurface->create(wnd->getWidth(), wnd->getHeight()))
            {
            this->surfaces.push_back(newSurface);
            //
            surfaceId = this->surfaces.size();
            newSurface->setId(surfaceId);
            }
        else
            delete newSurface;

        // release the window context
        wnd->releaseContext();
        }

    return surfaceId;
    }

// -----------------------------------------------------------------------------

ViewSurface* View::getSurfaceFromId(uint surfaceId)
    {
    if(surfaceId <= surfaces.size())
        return surfaces[surfaceId - 1];
    else
        return NULL;
    }
