/* 
 * File:   LuaHighlighter.cpp
 * Author: alex9978
 * 
 * Created on October 26, 2009, 11:49 PM
 */

#include <QtGui>

#include "LuaHighlighter.h"

LuaHighlighter::LuaHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "\\band\\b" << "\\bbreak\\b" << "\\bdo\\b"
                    << "\\belse\\b" << "\\belseif\\b" << "\\bend\\b"
                    << "\\bfalse\\b" << "\\bfor\\b" << "\\bfunction\\b"
                    << "\\bif\\b" << "\\bin\\b" << "\\blocal\\b"
                    << "\\bnil\\b" << "\\bnot\\b" << "\\bor\\b"
                    << "\\brepeat\\b" << "\\breturn\\b" << "\\bthen\\b"
                    << "\\btrue\\b" << "\\buntil\\b" << "\\bwhile\\b";

    foreach(const QString &pattern, keywordPatterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    stringFormat.setFontWeight(QFont::Bold);
    stringFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegExp("(\"[^\"]*\"|'[^']*'|\\[\\[[^\\[\\]]*\\]\\])");
    rule.format = stringFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::red);
    rule.pattern = QRegExp("(--\\[\\[([^\\]\\]]*)\\]\\]|(--(.*)))");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::red);



   /*quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("(==|=|<=|>=|~=|%|\\^|\\+|-|\\*|\\/){1}");
    rule.format = quotationFormat;
    highlightingRules.append(rule);*/

 /*   functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);*/

    commentStartExpression = QRegExp("(--\\[\\[([^\\]\\]]*))");
    commentEndExpression = QRegExp("(\\]\\])");
}

void LuaHighlighter::highlightBlock(const QString &text)
{
    foreach(const HighlightingRule &rule, highlightingRules)
    {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0)
        {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = commentStartExpression.indexIn(text);

    while (startIndex >= 0)
    {
        int endIndex = commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1)
        {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        }
        else
        {
            commentLength = endIndex - startIndex + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }
}
