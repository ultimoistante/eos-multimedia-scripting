/* 
 * File:   LuaHighlighter.h
 * Author: alex9978
 *
 * Created on October 26, 2009, 11:49 PM
 */

#ifndef _LUAHIGHLIGHTER_H
#define	_LUAHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QHash>
#include <QTextCharFormat>

class QTextDocument;

class LuaHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    LuaHighlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text);

private:
     struct HighlightingRule
     {
         QRegExp pattern;
         QTextCharFormat format;
     };
     QVector<HighlightingRule> highlightingRules;

     QRegExp commentStartExpression;
     QRegExp commentEndExpression;

     QTextCharFormat keywordFormat;
     QTextCharFormat stringFormat;
     QTextCharFormat singleLineCommentFormat;
     QTextCharFormat multiLineCommentFormat;
     QTextCharFormat quotationFormat;
     QTextCharFormat functionFormat;
};

#endif	/* _LUAHIGHLIGHTER_H */

