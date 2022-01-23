/********************************************************************************
** Form generated from reading UI file 'qtdesigncal.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTDESIGNCAL_H
#define UI_QTDESIGNCAL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtDesignCalClass
{
public:

    void setupUi(QWidget *QtDesignCalClass)
    {
        if (QtDesignCalClass->objectName().isEmpty())
            QtDesignCalClass->setObjectName(QString::fromUtf8("QtDesignCalClass"));
        QtDesignCalClass->resize(600, 400);

        retranslateUi(QtDesignCalClass);

        QMetaObject::connectSlotsByName(QtDesignCalClass);
    } // setupUi

    void retranslateUi(QWidget *QtDesignCalClass)
    {
        QtDesignCalClass->setWindowTitle(QCoreApplication::translate("QtDesignCalClass", "QtDesignCal", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtDesignCalClass: public Ui_QtDesignCalClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTDESIGNCAL_H
