/*
 * File:   main.cpp
 * Author: alex9978
 *
 * Created on October 25, 2009, 8:18 PM
 */

#include <QtGui/QApplication>
#include "FormMain.h"

int main(int argc, char *argv[])
{
    // Q_INIT_RESOURCE(resfile);

    QApplication app(argc, argv);
    FormMain formMain;
    formMain.showMaximized();
    return app.exec();
}
