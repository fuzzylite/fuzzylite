/********************************************************************************
** Form generated from reading UI file 'Preferences.ui'
**
** Created: Sat Feb 12 19:33:24 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREFERENCES_H
#define UI_PREFERENCES_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QFormLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Preferences
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *grx_fuzzy_operator;
    QVBoxLayout *verticalLayout_2;
    QFormLayout *formLayout;
    QLabel *lbl_aggregation;
    QComboBox *cbx_aggregation;
    QLabel *lbl_defuzzifier;
    QComboBox *cbx_defuzzifier;
    QLabel *lbl_modulation;
    QComboBox *cbx_modulation;
    QComboBox *cbx_snorm;
    QLabel *lbl_snorm;
    QComboBox *cbx_tnorm;
    QLabel *lbl_tnorm;
    QLabel *lbl_sample_size;
    QSpinBox *isx_sample_size;
    QLabel *lbl_algorithm;
    QComboBox *cbx_algorithm;

    void setupUi(QDialog *Preferences)
    {
        if (Preferences->objectName().isEmpty())
            Preferences->setObjectName(QString::fromUtf8("Preferences"));
        Preferences->resize(320, 281);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Preferences->sizePolicy().hasHeightForWidth());
        Preferences->setSizePolicy(sizePolicy);
        Preferences->setSizeGripEnabled(false);
        verticalLayout = new QVBoxLayout(Preferences);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        grx_fuzzy_operator = new QGroupBox(Preferences);
        grx_fuzzy_operator->setObjectName(QString::fromUtf8("grx_fuzzy_operator"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(grx_fuzzy_operator->sizePolicy().hasHeightForWidth());
        grx_fuzzy_operator->setSizePolicy(sizePolicy1);
        verticalLayout_2 = new QVBoxLayout(grx_fuzzy_operator);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        lbl_aggregation = new QLabel(grx_fuzzy_operator);
        lbl_aggregation->setObjectName(QString::fromUtf8("lbl_aggregation"));

        formLayout->setWidget(5, QFormLayout::LabelRole, lbl_aggregation);

        cbx_aggregation = new QComboBox(grx_fuzzy_operator);
        cbx_aggregation->setObjectName(QString::fromUtf8("cbx_aggregation"));
        cbx_aggregation->setMinimumSize(QSize(170, 0));

        formLayout->setWidget(5, QFormLayout::FieldRole, cbx_aggregation);

        lbl_defuzzifier = new QLabel(grx_fuzzy_operator);
        lbl_defuzzifier->setObjectName(QString::fromUtf8("lbl_defuzzifier"));

        formLayout->setWidget(4, QFormLayout::LabelRole, lbl_defuzzifier);

        cbx_defuzzifier = new QComboBox(grx_fuzzy_operator);
        cbx_defuzzifier->setObjectName(QString::fromUtf8("cbx_defuzzifier"));
        cbx_defuzzifier->setMinimumSize(QSize(170, 0));

        formLayout->setWidget(4, QFormLayout::FieldRole, cbx_defuzzifier);

        lbl_modulation = new QLabel(grx_fuzzy_operator);
        lbl_modulation->setObjectName(QString::fromUtf8("lbl_modulation"));

        formLayout->setWidget(2, QFormLayout::LabelRole, lbl_modulation);

        cbx_modulation = new QComboBox(grx_fuzzy_operator);
        cbx_modulation->setObjectName(QString::fromUtf8("cbx_modulation"));
        cbx_modulation->setMinimumSize(QSize(170, 0));

        formLayout->setWidget(2, QFormLayout::FieldRole, cbx_modulation);

        cbx_snorm = new QComboBox(grx_fuzzy_operator);
        cbx_snorm->setObjectName(QString::fromUtf8("cbx_snorm"));
        cbx_snorm->setMinimumSize(QSize(170, 0));

        formLayout->setWidget(1, QFormLayout::FieldRole, cbx_snorm);

        lbl_snorm = new QLabel(grx_fuzzy_operator);
        lbl_snorm->setObjectName(QString::fromUtf8("lbl_snorm"));

        formLayout->setWidget(1, QFormLayout::LabelRole, lbl_snorm);

        cbx_tnorm = new QComboBox(grx_fuzzy_operator);
        cbx_tnorm->setObjectName(QString::fromUtf8("cbx_tnorm"));
        cbx_tnorm->setMinimumSize(QSize(170, 0));

        formLayout->setWidget(0, QFormLayout::FieldRole, cbx_tnorm);

        lbl_tnorm = new QLabel(grx_fuzzy_operator);
        lbl_tnorm->setObjectName(QString::fromUtf8("lbl_tnorm"));

        formLayout->setWidget(0, QFormLayout::LabelRole, lbl_tnorm);

        lbl_sample_size = new QLabel(grx_fuzzy_operator);
        lbl_sample_size->setObjectName(QString::fromUtf8("lbl_sample_size"));

        formLayout->setWidget(6, QFormLayout::LabelRole, lbl_sample_size);

        isx_sample_size = new QSpinBox(grx_fuzzy_operator);
        isx_sample_size->setObjectName(QString::fromUtf8("isx_sample_size"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(isx_sample_size->sizePolicy().hasHeightForWidth());
        isx_sample_size->setSizePolicy(sizePolicy2);
        isx_sample_size->setAlignment(Qt::AlignCenter);
        isx_sample_size->setMinimum(10);
        isx_sample_size->setMaximum(9999999);
        isx_sample_size->setSingleStep(50);
        isx_sample_size->setValue(100);

        formLayout->setWidget(6, QFormLayout::FieldRole, isx_sample_size);

        lbl_algorithm = new QLabel(grx_fuzzy_operator);
        lbl_algorithm->setObjectName(QString::fromUtf8("lbl_algorithm"));

        formLayout->setWidget(3, QFormLayout::LabelRole, lbl_algorithm);

        cbx_algorithm = new QComboBox(grx_fuzzy_operator);
        cbx_algorithm->setObjectName(QString::fromUtf8("cbx_algorithm"));
        sizePolicy2.setHeightForWidth(cbx_algorithm->sizePolicy().hasHeightForWidth());
        cbx_algorithm->setSizePolicy(sizePolicy2);
        cbx_algorithm->setMinimumSize(QSize(130, 0));

        formLayout->setWidget(3, QFormLayout::FieldRole, cbx_algorithm);


        verticalLayout_2->addLayout(formLayout);


        verticalLayout->addWidget(grx_fuzzy_operator);


        retranslateUi(Preferences);

        cbx_snorm->setCurrentIndex(0);
        cbx_tnorm->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Preferences);
    } // setupUi

    void retranslateUi(QDialog *Preferences)
    {
        Preferences->setWindowTitle(QApplication::translate("Preferences", "Preferences", 0, QApplication::UnicodeUTF8));
        grx_fuzzy_operator->setTitle(QApplication::translate("Preferences", "Fuzzy Operator", 0, QApplication::UnicodeUTF8));
        lbl_aggregation->setText(QApplication::translate("Preferences", "Aggregation", 0, QApplication::UnicodeUTF8));
        cbx_aggregation->clear();
        cbx_aggregation->insertItems(0, QStringList()
         << QApplication::translate("Preferences", "Maximum", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Preferences", "Sum", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Preferences", "Bounded Sum", 0, QApplication::UnicodeUTF8)
        );
        lbl_defuzzifier->setText(QApplication::translate("Preferences", "Defuzzifier", 0, QApplication::UnicodeUTF8));
        cbx_defuzzifier->clear();
        cbx_defuzzifier->insertItems(0, QStringList()
         << QApplication::translate("Preferences", "Centroid", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Preferences", "TSK", 0, QApplication::UnicodeUTF8)
        );
        lbl_modulation->setText(QApplication::translate("Preferences", "Modulation", 0, QApplication::UnicodeUTF8));
        cbx_modulation->clear();
        cbx_modulation->insertItems(0, QStringList()
         << QApplication::translate("Preferences", "Clipping", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Preferences", "Scaling", 0, QApplication::UnicodeUTF8)
        );
        cbx_snorm->clear();
        cbx_snorm->insertItems(0, QStringList()
         << QApplication::translate("Preferences", "Maximum", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Preferences", "Sum", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Preferences", "Bounded Sum", 0, QApplication::UnicodeUTF8)
        );
        lbl_snorm->setText(QApplication::translate("Preferences", "S-Norm", 0, QApplication::UnicodeUTF8));
        cbx_tnorm->clear();
        cbx_tnorm->insertItems(0, QStringList()
         << QApplication::translate("Preferences", "Minimum", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Preferences", "Product", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Preferences", "Bounded Difference", 0, QApplication::UnicodeUTF8)
        );
        lbl_tnorm->setText(QApplication::translate("Preferences", "T-Norm", 0, QApplication::UnicodeUTF8));
        lbl_sample_size->setText(QApplication::translate("Preferences", "Sample Size", 0, QApplication::UnicodeUTF8));
        lbl_algorithm->setText(QApplication::translate("Preferences", "Algorithm", 0, QApplication::UnicodeUTF8));
        cbx_algorithm->clear();
        cbx_algorithm->insertItems(0, QStringList()
         << QApplication::translate("Preferences", "Triangulation", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Preferences", "Trapezoidal", 0, QApplication::UnicodeUTF8)
        );
    } // retranslateUi

};

namespace Ui {
    class Preferences: public Ui_Preferences {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREFERENCES_H
