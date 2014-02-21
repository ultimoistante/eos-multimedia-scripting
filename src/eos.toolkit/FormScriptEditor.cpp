/*
 * File:   FormScriptEditor.cpp
 * Author: alex9978
 *
 * Created on October 26, 2009, 11:06 PM
 */

#include "FormScriptEditor.h"

FormScriptEditor::FormScriptEditor()
{
    widget.setupUi(this);
    luaHighlighter = new LuaHighlighter(widget.textEdit->document());
}

FormScriptEditor::~FormScriptEditor()
{
}
