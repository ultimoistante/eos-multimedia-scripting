/*
 * File:   FormMain.cpp
 * Author: alex9978
 *
 * Created on October 25, 2009, 8:19 PM
 */

#include "FormMain.h"
#include "FormMsgLogInspector.h"
#include "FormScriptEditor.h"
#include <QMdiSubWindow>

FormMain::FormMain()
{
    widget.setupUi(this);

    // connect signals
    connect(widget.action_Quit, SIGNAL(triggered()), this, SLOT(quit()));
    connect(widget.actionMessages_Log_Inspector, SIGNAL(triggered()), this, SLOT(startMessagesLogInspector()));
    connect(widget.actionScript_Editor, SIGNAL(triggered()), this, SLOT(startScriptEditor()));
}

FormMain::~FormMain()
{
}

void FormMain::quit()
{
    exit(0);
}

void FormMain::startScriptEditor()
{
    if(!checkSubWindowOpened("Script Editor"))
    {
        QMdiSubWindow *subWindow = widget.mdiArea->addSubWindow(new FormScriptEditor);
        subWindow->setAttribute(Qt::WA_DeleteOnClose);
        subWindow->showMaximized();
    }
}

void FormMain::startMessagesLogInspector()
{
    if(!checkSubWindowOpened("Messages Log Inspector"))
    {
        QMdiSubWindow *subWindow = widget.mdiArea->addSubWindow(new FormMsgLogInspector);
        subWindow->setAttribute(Qt::WA_DeleteOnClose);
        subWindow->showMaximized();
    }    
}

bool FormMain::checkSubWindowOpened(QString name)
{
    QList<QMdiSubWindow *> wndList = widget.mdiArea->subWindowList();

    for(int i=0; i<wndList.size(); ++i)
        if(wndList.at(i)->windowTitle() == name)
            return true;

    return false;
}