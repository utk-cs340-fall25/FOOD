/********************************************************************************
** Form generated from reading UI file 'Radditemdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RADDITEMDIALOG_H
#define UI_RADDITEMDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_RAddItemDialog
{
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *labelName;
    QLineEdit *nameLineEdit;
    QLabel *labelQty;
    QDoubleSpinBox *qtySpin;
    QLabel *labelUnit;
    QLineEdit *unitLineEdit;
    QLabel *labelTags;
    QLineEdit *tagsLineEdit;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *RAddItemDialog)
    {
        if (RAddItemDialog->objectName().isEmpty())
            RAddItemDialog->setObjectName(QString::fromUtf8("RAddItemDialog"));
        RAddItemDialog->resize(400, 300);
        verticalLayout = new QVBoxLayout(RAddItemDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        labelName = new QLabel(RAddItemDialog);
        labelName->setObjectName(QString::fromUtf8("labelName"));

        formLayout->setWidget(0, QFormLayout::LabelRole, labelName);

        nameLineEdit = new QLineEdit(RAddItemDialog);
        nameLineEdit->setObjectName(QString::fromUtf8("nameLineEdit"));

        formLayout->setWidget(0, QFormLayout::FieldRole, nameLineEdit);

        labelQty = new QLabel(RAddItemDialog);
        labelQty->setObjectName(QString::fromUtf8("labelQty"));

        formLayout->setWidget(1, QFormLayout::LabelRole, labelQty);

        qtySpin = new QDoubleSpinBox(RAddItemDialog);
        qtySpin->setObjectName(QString::fromUtf8("qtySpin"));
        qtySpin->setMaximum(999999.000000000000000);

        formLayout->setWidget(1, QFormLayout::FieldRole, qtySpin);

        labelUnit = new QLabel(RAddItemDialog);
        labelUnit->setObjectName(QString::fromUtf8("labelUnit"));

        formLayout->setWidget(2, QFormLayout::LabelRole, labelUnit);

        unitLineEdit = new QLineEdit(RAddItemDialog);
        unitLineEdit->setObjectName(QString::fromUtf8("unitLineEdit"));

        formLayout->setWidget(2, QFormLayout::FieldRole, unitLineEdit);

        labelTags = new QLabel(RAddItemDialog);
        labelTags->setObjectName(QString::fromUtf8("labelTags"));

        formLayout->setWidget(3, QFormLayout::LabelRole, labelTags);

        tagsLineEdit = new QLineEdit(RAddItemDialog);
        tagsLineEdit->setObjectName(QString::fromUtf8("tagsLineEdit"));

        formLayout->setWidget(3, QFormLayout::FieldRole, tagsLineEdit);


        verticalLayout->addLayout(formLayout);

        buttonBox = new QDialogButtonBox(RAddItemDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(RAddItemDialog);

        QMetaObject::connectSlotsByName(RAddItemDialog);
    } // setupUi

    void retranslateUi(QDialog *RAddItemDialog)
    {
        RAddItemDialog->setWindowTitle(QCoreApplication::translate("RAddItemDialog", "Add Item", nullptr));
        labelName->setText(QCoreApplication::translate("RAddItemDialog", "Name", nullptr));
        labelQty->setText(QCoreApplication::translate("RAddItemDialog", "Quantity", nullptr));
        labelUnit->setText(QCoreApplication::translate("RAddItemDialog", "Unit", nullptr));
        labelTags->setText(QCoreApplication::translate("RAddItemDialog", "Tags", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RAddItemDialog: public Ui_RAddItemDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RADDITEMDIALOG_H
