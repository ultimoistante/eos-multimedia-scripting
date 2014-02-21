/********************************************************************************
** Form generated from reading ui file 'FormMain.ui'
**
** Created: Fri Oct 30 00:37:05 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_FORMMAIN_H
#define UI_FORMMAIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMdiArea>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QToolBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FormMain
{
public:
    QAction *action_Quit;
    QAction *actionMessages_Log_Inspector;
    QAction *actionScript_Editor;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QMdiArea *mdiArea;
    QMenuBar *menubar;
    QMenu *menu_File;
    QMenu *menu_Debugger;
    QMenu *menuEdit;
    QStatusBar *statusbar;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout_2;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_3;
    QToolBox *toolBox;
    QWidget *page;
    QVBoxLayout *verticalLayout_4;
    QListWidget *listWidget;
    QWidget *page_2;
    QWidget *tab_2;

    void setupUi(QMainWindow *FormMain)
    {
        if (FormMain->objectName().isEmpty())
            FormMain->setObjectName(QString::fromUtf8("FormMain"));
        FormMain->resize(524, 407);
        action_Quit = new QAction(FormMain);
        action_Quit->setObjectName(QString::fromUtf8("action_Quit"));
        actionMessages_Log_Inspector = new QAction(FormMain);
        actionMessages_Log_Inspector->setObjectName(QString::fromUtf8("actionMessages_Log_Inspector"));
        actionScript_Editor = new QAction(FormMain);
        actionScript_Editor->setObjectName(QString::fromUtf8("actionScript_Editor"));
        centralwidget = new QWidget(FormMain);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setMargin(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        mdiArea = new QMdiArea(centralwidget);
        mdiArea->setObjectName(QString::fromUtf8("mdiArea"));
        mdiArea->setFrameShape(QFrame::WinPanel);
        mdiArea->setFrameShadow(QFrame::Sunken);
        mdiArea->setLineWidth(1);
        mdiArea->setMidLineWidth(0);
        QBrush brush(QColor(198, 194, 189, 255));
        brush.setStyle(Qt::SolidPattern);
        mdiArea->setBackground(brush);
        mdiArea->setViewMode(QMdiArea::TabbedView);
        mdiArea->setDocumentMode(true);
        mdiArea->setTabShape(QTabWidget::Rounded);
        mdiArea->setTabPosition(QTabWidget::North);

        verticalLayout->addWidget(mdiArea);

        FormMain->setCentralWidget(centralwidget);
        menubar = new QMenuBar(FormMain);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 524, 22));
        menu_File = new QMenu(menubar);
        menu_File->setObjectName(QString::fromUtf8("menu_File"));
        menu_Debugger = new QMenu(menubar);
        menu_Debugger->setObjectName(QString::fromUtf8("menu_Debugger"));
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        FormMain->setMenuBar(menubar);
        statusbar = new QStatusBar(FormMain);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        FormMain->setStatusBar(statusbar);
        dockWidget = new QDockWidget(FormMain);
        dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
        dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        verticalLayout_2 = new QVBoxLayout(dockWidgetContents);
        verticalLayout_2->setMargin(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        tabWidget = new QTabWidget(dockWidgetContents);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout_3 = new QVBoxLayout(tab);
        verticalLayout_3->setMargin(4);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        toolBox = new QToolBox(tab);
        toolBox->setObjectName(QString::fromUtf8("toolBox"));
        toolBox->setFrameShape(QFrame::NoFrame);
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        page->setGeometry(QRect(0, 0, 256, 248));
        verticalLayout_4 = new QVBoxLayout(page);
        verticalLayout_4->setMargin(0);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        listWidget = new QListWidget(page);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        verticalLayout_4->addWidget(listWidget);

        toolBox->addItem(page, QString::fromUtf8("Scripts"));
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        page_2->setGeometry(QRect(0, 0, 96, 26));
        toolBox->addItem(page_2, QString::fromUtf8("Page 2"));

        verticalLayout_3->addWidget(toolBox);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        tabWidget->addTab(tab_2, QString());

        verticalLayout_2->addWidget(tabWidget);

        dockWidget->setWidget(dockWidgetContents);
        FormMain->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget);

        menubar->addAction(menu_File->menuAction());
        menubar->addAction(menuEdit->menuAction());
        menubar->addAction(menu_Debugger->menuAction());
        menu_File->addAction(action_Quit);
        menu_Debugger->addAction(actionMessages_Log_Inspector);
        menuEdit->addAction(actionScript_Editor);

        retranslateUi(FormMain);

        tabWidget->setCurrentIndex(0);
        toolBox->setCurrentIndex(0);
        toolBox->layout()->setSpacing(3);


        QMetaObject::connectSlotsByName(FormMain);
    } // setupUi

    void retranslateUi(QMainWindow *FormMain)
    {
        FormMain->setWindowTitle(QApplication::translate("FormMain", "eos.toolkit", 0, QApplication::UnicodeUTF8));
        action_Quit->setText(QApplication::translate("FormMain", "&Quit", 0, QApplication::UnicodeUTF8));
        actionMessages_Log_Inspector->setText(QApplication::translate("FormMain", "Messages Log Inspector", 0, QApplication::UnicodeUTF8));
        actionScript_Editor->setText(QApplication::translate("FormMain", "Script Editor", 0, QApplication::UnicodeUTF8));
        menu_File->setTitle(QApplication::translate("FormMain", "&File", 0, QApplication::UnicodeUTF8));
        menu_Debugger->setTitle(QApplication::translate("FormMain", "&Debug", 0, QApplication::UnicodeUTF8));
        menuEdit->setTitle(QApplication::translate("FormMain", "&Edit", 0, QApplication::UnicodeUTF8));
        dockWidget->setWindowTitle(QApplication::translate("FormMain", "Application", 0, QApplication::UnicodeUTF8));
        toolBox->setItemText(toolBox->indexOf(page), QApplication::translate("FormMain", "Scripts", 0, QApplication::UnicodeUTF8));
        toolBox->setItemText(toolBox->indexOf(page_2), QApplication::translate("FormMain", "Page 2", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("FormMain", "Resources", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("FormMain", "Properties", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class FormMain: public Ui_FormMain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMMAIN_H
