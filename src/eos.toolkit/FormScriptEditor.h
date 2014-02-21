/* 
 * File:   FormScriptEditor.h
 * Author: alex9978
 *
 * Created on October 26, 2009, 11:06 PM
 */

#ifndef _FORMSCRIPTEDITOR_H
#define	_FORMSCRIPTEDITOR_H

#include "ui_FormScriptEditor.h"
#include "LuaHighlighter.h"

class FormScriptEditor : public QWidget
{
    Q_OBJECT
public:
    FormScriptEditor();
    virtual ~FormScriptEditor();
private:
    Ui::FormScriptEditor widget;
    LuaHighlighter *luaHighlighter;

};

#endif	/* _FORMSCRIPTEDITOR_H */
