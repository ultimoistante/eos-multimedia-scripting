/*
 * File:   FormMsgLogInspector.cpp
 * Author: alex9978
 *
 * Created on October 25, 2009, 8:33 PM
 */

#include "FormMsgLogInspector.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

FormMsgLogInspector::FormMsgLogInspector()
{
    widget.setupUi(this);
    //
    connect(widget.pbOpenLogFile, SIGNAL(clicked(bool)), this, SLOT(openLog(bool)));
    connect(widget.cbFilter, SIGNAL(currentIndexChanged(int)), this, SLOT(updateFilterValues(int)));
    connect(widget.pbApply, SIGNAL(clicked(bool)), this, SLOT(applyFilter(bool)));
    connect(widget.pbReset, SIGNAL(clicked(bool)), this, SLOT(resetFilter(bool)));
    //
    filterIndex = 0;
}

FormMsgLogInspector::~FormMsgLogInspector()
{
}

void FormMsgLogInspector::openLog(bool)
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if(!fileName.isEmpty())
    {
        loadFile(fileName);
    }
}

void FormMsgLogInspector::updateFilterValues(int index)
{
    widget.cbValue->clear();
    if(index == 0)
        return;

    QStringList values;
    for(int i=0; i<widget.tableWidget->rowCount(); ++i)
    {
        QString value = widget.tableWidget->item(i, index)->text();
        if(!values.contains(value))
            values << value;
    }    
    widget.cbValue->addItems(values);
}

void FormMsgLogInspector::applyFilter(bool)
{
    filterIndex = widget.cbFilter->currentIndex();
    if(filterIndex > 0)
    {
        filter = widget.cbValue->currentText();
        fillTable();
    }
}

void FormMsgLogInspector::resetFilter(bool)
{
    if(filterIndex > 0)
    {
        filterIndex = 0;
        filter = "";
        fillTable();
    }
}

void FormMsgLogInspector::loadFile(QString fileName)
{
    // open file
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Application"), tr("Cannot read file %1:\n%2.")
                                                      .arg(fileName)
                                                      .arg(file.errorString()));
        return;
    }

    // open text stream
    QTextStream in(&file);

    // set wait mouse cursor
    QApplication::setOverrideCursor(Qt::WaitCursor);

    // read lines
    while(!in.atEnd())
    {
        QString line = in.readLine();
        lines.append(line);
    }

    // fill table
    fillTable();

    // set normal mouse cursor
    QApplication::restoreOverrideCursor();
}

void FormMsgLogInspector::fillTable()
{
    // clear table
    widget.tableWidget->clear();
    widget.tableWidget->setColumnCount(0);
    widget.tableWidget->setRowCount(lines.size());

    // populate table
    int usedRows = 0;
    widget.progressBar->setMaximum(lines.size()-1);
    //
    for(int i=0; i<lines.size(); ++i)
    {
        QStringList list = lines.at(i).split("\t");

        if(filterIndex == 0 || list.at(filterIndex) == filter)
        {
            // increase column count if necessary
            if(widget.tableWidget->columnCount() < list.size())
                widget.tableWidget->setColumnCount(list.size());

            for(int j=0; j<list.size(); ++j)
            {
                 QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg(list.at(j)));
                 widget.tableWidget->setItem(usedRows, j, newItem);
            }

            usedRows++;
        }
        widget.progressBar->setValue(i);
    }

    widget.tableWidget->setRowCount(usedRows);

    // set header
    QStringList labels;
    labels << "TimeStamp" << "Sender" << "Target" << "MessageClass";
    for(int i=4; i<widget.tableWidget->columnCount(); ++i)
        labels << tr("Param %1").arg(i-3);

    widget.tableWidget->setHorizontalHeaderLabels(labels);
}