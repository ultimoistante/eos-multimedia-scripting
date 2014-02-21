/* 
 * File:   FormMsgLogInspector.h
 * Author: alex9978
 *
 * Created on October 25, 2009, 8:33 PM
 */

#ifndef _FORMMSGLOGINSPECTOR_H
#define	_FORMMSGLOGINSPECTOR_H

#include "ui_FormMsgLogInspector.h"

class FormMsgLogInspector : public QWidget
{
    Q_OBJECT
public:
    FormMsgLogInspector();
    virtual ~FormMsgLogInspector();

private slots:
    void openLog(bool);
    void updateFilterValues(int index);
    void applyFilter(bool);
    void resetFilter(bool);

private:
    Ui::FormMsgLogInspector widget;
    QStringList lines;

    int filterIndex;
    QString filter;

    void loadFile(QString fileName);
    void fillTable();
};

#endif	/* _FORMMSGLOGINSPECTOR_H */
