/********************************************************************************
** Form generated from reading ui file 'FormScriptEditor.ui'
**
** Created: Fri Oct 30 00:37:05 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_FORMSCRIPTEDITOR_H
#define UI_FORMSCRIPTEDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FormScriptEditor
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer;
    QTextEdit *textEdit;

    void setupUi(QWidget *FormScriptEditor)
    {
        if (FormScriptEditor->objectName().isEmpty())
            FormScriptEditor->setObjectName(QString::fromUtf8("FormScriptEditor"));
        FormScriptEditor->resize(539, 559);
        verticalLayout_2 = new QVBoxLayout(FormScriptEditor);
        verticalLayout_2->setMargin(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        frame = new QFrame(FormScriptEditor);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(frame);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        pushButton = new QPushButton(frame);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout_2->addWidget(pushButton);

        horizontalSpacer = new QSpacerItem(425, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout->addWidget(frame);

        textEdit = new QTextEdit(FormScriptEditor);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));

        verticalLayout->addWidget(textEdit);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(FormScriptEditor);

        QMetaObject::connectSlotsByName(FormScriptEditor);
    } // setupUi

    void retranslateUi(QWidget *FormScriptEditor)
    {
        FormScriptEditor->setWindowTitle(QApplication::translate("FormScriptEditor", "Script Editor", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("FormScriptEditor", "Save", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(FormScriptEditor);
    } // retranslateUi

};

namespace Ui {
    class FormScriptEditor: public Ui_FormScriptEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMSCRIPTEDITOR_H
