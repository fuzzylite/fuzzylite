/********************************************************************************
** Form generated from reading UI file 'LinguisticVariableWizard.ui'
**
** Created: Sat Feb 12 19:33:24 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LINGUISTICVARIABLEWIZARD_H
#define UI_LINGUISTICVARIABLEWIZARD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QSpinBox>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_LinguisticVariableWizard
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *grx_wizard;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_4;
    QLabel *lbl_min;
    QDoubleSpinBox *dsx_min;
    QVBoxLayout *verticalLayout_5;
    QLabel *lbl_max;
    QDoubleSpinBox *dsx_max;
    QVBoxLayout *verticalLayout_6;
    QLabel *lbl_terms;
    QSpinBox *spx_terms;
    QHBoxLayout *horizontalLayout;
    QComboBox *cbx_lterm;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_5;
    QLabel *lbl_term_labels;
    QToolButton *btn_create_labels;
    QPlainTextEdit *ptx_term_labels;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *LinguisticVariableWizard)
    {
        if (LinguisticVariableWizard->objectName().isEmpty())
            LinguisticVariableWizard->setObjectName(QString::fromUtf8("LinguisticVariableWizard"));
        LinguisticVariableWizard->setWindowModality(Qt::WindowModal);
        LinguisticVariableWizard->resize(377, 221);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(LinguisticVariableWizard->sizePolicy().hasHeightForWidth());
        LinguisticVariableWizard->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(LinguisticVariableWizard);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        grx_wizard = new QGroupBox(LinguisticVariableWizard);
        grx_wizard->setObjectName(QString::fromUtf8("grx_wizard"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(grx_wizard->sizePolicy().hasHeightForWidth());
        grx_wizard->setSizePolicy(sizePolicy1);
        horizontalLayout_4 = new QHBoxLayout(grx_wizard);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        lbl_min = new QLabel(grx_wizard);
        lbl_min->setObjectName(QString::fromUtf8("lbl_min"));
        lbl_min->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(lbl_min);

        dsx_min = new QDoubleSpinBox(grx_wizard);
        dsx_min->setObjectName(QString::fromUtf8("dsx_min"));
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(dsx_min->sizePolicy().hasHeightForWidth());
        dsx_min->setSizePolicy(sizePolicy2);
        dsx_min->setAlignment(Qt::AlignCenter);
        dsx_min->setMinimum(-1e+09);
        dsx_min->setMaximum(1e+09);

        verticalLayout_4->addWidget(dsx_min);


        horizontalLayout_3->addLayout(verticalLayout_4);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        lbl_max = new QLabel(grx_wizard);
        lbl_max->setObjectName(QString::fromUtf8("lbl_max"));
        lbl_max->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(lbl_max);

        dsx_max = new QDoubleSpinBox(grx_wizard);
        dsx_max->setObjectName(QString::fromUtf8("dsx_max"));
        sizePolicy2.setHeightForWidth(dsx_max->sizePolicy().hasHeightForWidth());
        dsx_max->setSizePolicy(sizePolicy2);
        dsx_max->setAlignment(Qt::AlignCenter);
        dsx_max->setMinimum(-1e+09);
        dsx_max->setMaximum(1e+09);

        verticalLayout_5->addWidget(dsx_max);


        horizontalLayout_3->addLayout(verticalLayout_5);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        lbl_terms = new QLabel(grx_wizard);
        lbl_terms->setObjectName(QString::fromUtf8("lbl_terms"));
        lbl_terms->setAlignment(Qt::AlignCenter);

        verticalLayout_6->addWidget(lbl_terms);

        spx_terms = new QSpinBox(grx_wizard);
        spx_terms->setObjectName(QString::fromUtf8("spx_terms"));
        sizePolicy2.setHeightForWidth(spx_terms->sizePolicy().hasHeightForWidth());
        spx_terms->setSizePolicy(sizePolicy2);
        spx_terms->setAlignment(Qt::AlignCenter);
        spx_terms->setMinimum(1);
        spx_terms->setMaximum(99);
        spx_terms->setValue(5);

        verticalLayout_6->addWidget(spx_terms);


        horizontalLayout_3->addLayout(verticalLayout_6);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        cbx_lterm = new QComboBox(grx_wizard);
        cbx_lterm->setObjectName(QString::fromUtf8("cbx_lterm"));

        horizontalLayout->addWidget(cbx_lterm);


        verticalLayout_2->addLayout(horizontalLayout);


        horizontalLayout_4->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        lbl_term_labels = new QLabel(grx_wizard);
        lbl_term_labels->setObjectName(QString::fromUtf8("lbl_term_labels"));
        lbl_term_labels->setAlignment(Qt::AlignCenter);

        horizontalLayout_5->addWidget(lbl_term_labels);

        btn_create_labels = new QToolButton(grx_wizard);
        btn_create_labels->setObjectName(QString::fromUtf8("btn_create_labels"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icons/wizard.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_create_labels->setIcon(icon);

        horizontalLayout_5->addWidget(btn_create_labels);


        verticalLayout_3->addLayout(horizontalLayout_5);

        ptx_term_labels = new QPlainTextEdit(grx_wizard);
        ptx_term_labels->setObjectName(QString::fromUtf8("ptx_term_labels"));

        verticalLayout_3->addWidget(ptx_term_labels);


        horizontalLayout_4->addLayout(verticalLayout_3);


        verticalLayout->addWidget(grx_wizard);

        buttonBox = new QDialogButtonBox(LinguisticVariableWizard);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(LinguisticVariableWizard);
        QObject::connect(buttonBox, SIGNAL(accepted()), LinguisticVariableWizard, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), LinguisticVariableWizard, SLOT(reject()));

        QMetaObject::connectSlotsByName(LinguisticVariableWizard);
    } // setupUi

    void retranslateUi(QDialog *LinguisticVariableWizard)
    {
        LinguisticVariableWizard->setWindowTitle(QApplication::translate("LinguisticVariableWizard", "Linguistic Variable Wizard", 0, QApplication::UnicodeUTF8));
        grx_wizard->setTitle(QApplication::translate("LinguisticVariableWizard", "Wizard", 0, QApplication::UnicodeUTF8));
        lbl_min->setText(QApplication::translate("LinguisticVariableWizard", "Min.", 0, QApplication::UnicodeUTF8));
        lbl_max->setText(QApplication::translate("LinguisticVariableWizard", "Max.", 0, QApplication::UnicodeUTF8));
        lbl_terms->setText(QApplication::translate("LinguisticVariableWizard", "Terms", 0, QApplication::UnicodeUTF8));
        cbx_lterm->clear();
        cbx_lterm->insertItems(0, QStringList()
         << QApplication::translate("LinguisticVariableWizard", "Triangular", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("LinguisticVariableWizard", "Triangular+Shoulder", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("LinguisticVariableWizard", "Trapezoidal", 0, QApplication::UnicodeUTF8)
        );
        lbl_term_labels->setText(QApplication::translate("LinguisticVariableWizard", "Term Labels", 0, QApplication::UnicodeUTF8));
        btn_create_labels->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class LinguisticVariableWizard: public Ui_LinguisticVariableWizard {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LINGUISTICVARIABLEWIZARD_H
