/********************************************************************************
** Form generated from reading ui file 'FormMsgLogInspector.ui'
**
** Created: Fri Oct 30 00:37:05 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_FORMMSGLOGINSPECTOR_H
#define UI_FORMMSGLOGINSPECTOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FormMsgLogInspector
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QPushButton *pbOpenLogFile;
    QFrame *line_2;
    QLabel *label;
    QComboBox *cbFilter;
    QComboBox *cbValue;
    QPushButton *pbApply;
    QFrame *line;
    QPushButton *pbReset;
    QProgressBar *progressBar;
    QSpacerItem *horizontalSpacer;
    QTableWidget *tableWidget;

    void setupUi(QWidget *FormMsgLogInspector)
    {
        if (FormMsgLogInspector->objectName().isEmpty())
            FormMsgLogInspector->setObjectName(QString::fromUtf8("FormMsgLogInspector"));
        FormMsgLogInspector->resize(796, 243);
        verticalLayout = new QVBoxLayout(FormMsgLogInspector);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        frame = new QFrame(FormMsgLogInspector);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame);
        horizontalLayout->setMargin(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pbOpenLogFile = new QPushButton(frame);
        pbOpenLogFile->setObjectName(QString::fromUtf8("pbOpenLogFile"));

        horizontalLayout->addWidget(pbOpenLogFile);

        line_2 = new QFrame(frame);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(line_2);

        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        cbFilter = new QComboBox(frame);
        cbFilter->setObjectName(QString::fromUtf8("cbFilter"));
        cbFilter->setMinimumSize(QSize(120, 0));

        horizontalLayout->addWidget(cbFilter);

        cbValue = new QComboBox(frame);
        cbValue->setObjectName(QString::fromUtf8("cbValue"));
        cbValue->setMinimumSize(QSize(120, 0));

        horizontalLayout->addWidget(cbValue);

        pbApply = new QPushButton(frame);
        pbApply->setObjectName(QString::fromUtf8("pbApply"));

        horizontalLayout->addWidget(pbApply);

        line = new QFrame(frame);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(line);

        pbReset = new QPushButton(frame);
        pbReset->setObjectName(QString::fromUtf8("pbReset"));

        horizontalLayout->addWidget(pbReset);

        progressBar = new QProgressBar(frame);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(0);

        horizontalLayout->addWidget(progressBar);

        horizontalSpacer = new QSpacerItem(91, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addWidget(frame);

        tableWidget = new QTableWidget(FormMsgLogInspector);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));

        verticalLayout->addWidget(tableWidget);


        retranslateUi(FormMsgLogInspector);

        QMetaObject::connectSlotsByName(FormMsgLogInspector);
    } // setupUi

    void retranslateUi(QWidget *FormMsgLogInspector)
    {
        FormMsgLogInspector->setWindowTitle(QApplication::translate("FormMsgLogInspector", "Messages Log Inspector", 0, QApplication::UnicodeUTF8));
        pbOpenLogFile->setText(QApplication::translate("FormMsgLogInspector", "Open Log File...", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("FormMsgLogInspector", "Filter by", 0, QApplication::UnicodeUTF8));
        cbFilter->clear();
        cbFilter->insertItems(0, QStringList()
         << QApplication::translate("FormMsgLogInspector", "None", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FormMsgLogInspector", "Sender", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FormMsgLogInspector", "Target", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FormMsgLogInspector", "Message class", 0, QApplication::UnicodeUTF8)
        );
        pbApply->setText(QApplication::translate("FormMsgLogInspector", "Apply", 0, QApplication::UnicodeUTF8));
        pbReset->setText(QApplication::translate("FormMsgLogInspector", "Reset Filter", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(FormMsgLogInspector);
    } // retranslateUi

};

namespace Ui {
    class FormMsgLogInspector: public Ui_FormMsgLogInspector {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMMSGLOGINSPECTOR_H
