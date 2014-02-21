/* 
 * File:   FormMain.h
 * Author: alex9978
 *
 * Created on October 25, 2009, 8:19 PM
 */

#ifndef _FORMMAIN_H
#define	_FORMMAIN_H

#include "ui_FormMain.h"
#include <QString>

class FormMain : public QMainWindow
{
    Q_OBJECT
public:
    FormMain();
    virtual ~FormMain();

private slots:
    void quit();
    void startScriptEditor();
    void startMessagesLogInspector();

private:
    Ui::FormMain widget;

    bool checkSubWindowOpened(QString name);
};

#endif	/* _FORMMAIN_H */
