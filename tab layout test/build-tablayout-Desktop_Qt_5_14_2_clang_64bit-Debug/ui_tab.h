/********************************************************************************
** Form generated from reading UI file 'tab.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TAB_H
#define UI_TAB_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_tab
{
public:

    void setupUi(QWidget *tab)
    {
        if (tab->objectName().isEmpty())
            tab->setObjectName(QString::fromUtf8("tab"));
        tab->resize(800, 600);

        retranslateUi(tab);

        QMetaObject::connectSlotsByName(tab);
    } // setupUi

    void retranslateUi(QWidget *tab)
    {
        tab->setWindowTitle(QCoreApplication::translate("tab", "tab", nullptr));
    } // retranslateUi

};

namespace Ui {
    class tab: public Ui_tab {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TAB_H
