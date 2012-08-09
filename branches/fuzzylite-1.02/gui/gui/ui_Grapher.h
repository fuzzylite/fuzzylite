/********************************************************************************
** Form generated from reading UI file 'Grapher.ui'
**
** Created: Sat Feb 12 19:33:24 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRAPHER_H
#define UI_GRAPHER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QSlider>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "gui/GuiCanvas.h"

QT_BEGIN_NAMESPACE

class Ui_Grapher
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *lbl_name;
    QHBoxLayout *horizontalLayout_9;
    fl_gui::GuiCanvas *gvw_canvas;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *grx_control;
    QVBoxLayout *verticalLayout_5;
    QSlider *hsl_value;
    QHBoxLayout *horizontalLayout;
    QLabel *lbl_x;
    QLineEdit *led_x;
    QLabel *lbl_y;
    QLineEdit *led_y;
    QHBoxLayout *horizontalLayout_2;
    QLabel *lbl_fuzzy;
    QFrame *line_2;
    QGroupBox *grx_term;
    QVBoxLayout *verticalLayout_6;
    QGroupBox *grx_term_range;
    QHBoxLayout *horizontalLayout_6;
    QLabel *lbl_term_range_min;
    QLineEdit *led_term_range_min;
    QLabel *lbl_term_range_max;
    QLineEdit *led_term_range_max;
    QGroupBox *grx_term_centroid;
    QHBoxLayout *horizontalLayout_3;
    QLabel *lbl_x_term_centroid;
    QLineEdit *led_x_term_centroid;
    QLabel *lbl_y_term_centroid;
    QLineEdit *led_y_term_centroid;
    QHBoxLayout *horizontalLayout_5;
    QLabel *lbl_term_area;
    QLineEdit *led_term_area;
    QGroupBox *grx_lvar;
    QVBoxLayout *verticalLayout_7;
    QGroupBox *grx_lvar_range;
    QHBoxLayout *horizontalLayout_7;
    QLabel *lbl_lvar_range_min;
    QLineEdit *led_lvar_range_min;
    QLabel *lbl_lvar_range_max;
    QLineEdit *led_lvar_range_max;
    QGroupBox *grx_lvar_centroid;
    QHBoxLayout *horizontalLayout_4;
    QLabel *lbl_x_lvar_centroid;
    QLineEdit *led_x_lvar_centroid;
    QLabel *lbl_y_lvar_centroid;
    QLineEdit *led_y_lvar_centroid;
    QHBoxLayout *horizontalLayout_8;
    QLabel *lbl_lvar_area;
    QLineEdit *led_lvar_area;
    QHBoxLayout *hly_area_centroid;

    void setupUi(QWidget *Grapher)
    {
        if (Grapher->objectName().isEmpty())
            Grapher->setObjectName(QString::fromUtf8("Grapher"));
        Grapher->resize(199, 497);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Grapher->sizePolicy().hasHeightForWidth());
        Grapher->setSizePolicy(sizePolicy);
        Grapher->setFocusPolicy(Qt::ClickFocus);
        Grapher->setContextMenuPolicy(Qt::DefaultContextMenu);
        verticalLayout = new QVBoxLayout(Grapher);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(2, 2, 2, 2);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        lbl_name = new QLabel(Grapher);
        lbl_name->setObjectName(QString::fromUtf8("lbl_name"));

        verticalLayout->addWidget(lbl_name);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        horizontalLayout_9->setContentsMargins(8, -1, 8, -1);
        gvw_canvas = new fl_gui::GuiCanvas(Grapher);
        gvw_canvas->setObjectName(QString::fromUtf8("gvw_canvas"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(gvw_canvas->sizePolicy().hasHeightForWidth());
        gvw_canvas->setSizePolicy(sizePolicy1);
        gvw_canvas->setFocusPolicy(Qt::NoFocus);
        gvw_canvas->setContextMenuPolicy(Qt::NoContextMenu);
        gvw_canvas->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        gvw_canvas->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        QBrush brush(QColor(0, 0, 0, 0));
        brush.setStyle(Qt::NoBrush);
        gvw_canvas->setBackgroundBrush(brush);
        QBrush brush1(QColor(0, 0, 0, 0));
        brush1.setStyle(Qt::NoBrush);
        gvw_canvas->setForegroundBrush(brush1);
        gvw_canvas->setSceneRect(QRectF(0, 0, 0, 0));
        gvw_canvas->setRenderHints(QPainter::Antialiasing|QPainter::HighQualityAntialiasing|QPainter::SmoothPixmapTransform|QPainter::TextAntialiasing);
        gvw_canvas->setResizeAnchor(QGraphicsView::AnchorViewCenter);
        gvw_canvas->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);

        horizontalLayout_9->addWidget(gvw_canvas);


        verticalLayout->addLayout(horizontalLayout_9);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        grx_control = new QGroupBox(Grapher);
        grx_control->setObjectName(QString::fromUtf8("grx_control"));
        grx_control->setFlat(true);
        verticalLayout_5 = new QVBoxLayout(grx_control);
        verticalLayout_5->setSpacing(0);
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        hsl_value = new QSlider(grx_control);
        hsl_value->setObjectName(QString::fromUtf8("hsl_value"));
        hsl_value->setFocusPolicy(Qt::WheelFocus);
        hsl_value->setContextMenuPolicy(Qt::NoContextMenu);
        hsl_value->setMinimum(-100);
        hsl_value->setMaximum(100);
        hsl_value->setValue(0);
        hsl_value->setOrientation(Qt::Horizontal);
        hsl_value->setInvertedAppearance(false);

        verticalLayout_5->addWidget(hsl_value);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        lbl_x = new QLabel(grx_control);
        lbl_x->setObjectName(QString::fromUtf8("lbl_x"));
        QFont font;
        font.setPointSize(11);
        lbl_x->setFont(font);
        lbl_x->setContextMenuPolicy(Qt::NoContextMenu);

        horizontalLayout->addWidget(lbl_x);

        led_x = new QLineEdit(grx_control);
        led_x->setObjectName(QString::fromUtf8("led_x"));
        led_x->setAlignment(Qt::AlignCenter);
        led_x->setReadOnly(false);

        horizontalLayout->addWidget(led_x);

        lbl_y = new QLabel(grx_control);
        lbl_y->setObjectName(QString::fromUtf8("lbl_y"));
        lbl_y->setFont(font);
        lbl_y->setContextMenuPolicy(Qt::NoContextMenu);
        lbl_y->setTextFormat(Qt::RichText);

        horizontalLayout->addWidget(lbl_y);

        led_y = new QLineEdit(grx_control);
        led_y->setObjectName(QString::fromUtf8("led_y"));
        led_y->setAlignment(Qt::AlignCenter);
        led_y->setReadOnly(true);

        horizontalLayout->addWidget(led_y);


        verticalLayout_5->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setSizeConstraint(QLayout::SetMinAndMaxSize);
        lbl_fuzzy = new QLabel(grx_control);
        lbl_fuzzy->setObjectName(QString::fromUtf8("lbl_fuzzy"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(lbl_fuzzy->sizePolicy().hasHeightForWidth());
        lbl_fuzzy->setSizePolicy(sizePolicy2);
        lbl_fuzzy->setMinimumSize(QSize(0, 30));
        lbl_fuzzy->setFont(font);
        lbl_fuzzy->setContextMenuPolicy(Qt::NoContextMenu);
        lbl_fuzzy->setAlignment(Qt::AlignCenter);
        lbl_fuzzy->setWordWrap(true);

        horizontalLayout_2->addWidget(lbl_fuzzy);


        verticalLayout_5->addLayout(horizontalLayout_2);


        verticalLayout_3->addWidget(grx_control);

        line_2 = new QFrame(Grapher);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout_3->addWidget(line_2);

        grx_term = new QGroupBox(Grapher);
        grx_term->setObjectName(QString::fromUtf8("grx_term"));
        grx_term->setFlat(false);
        verticalLayout_6 = new QVBoxLayout(grx_term);
        verticalLayout_6->setSpacing(2);
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        grx_term_range = new QGroupBox(grx_term);
        grx_term_range->setObjectName(QString::fromUtf8("grx_term_range"));
        grx_term_range->setFlat(true);
        horizontalLayout_6 = new QHBoxLayout(grx_term_range);
#ifndef Q_OS_MAC
        horizontalLayout_6->setSpacing(-1);
#endif
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 3, 0, 0);
        lbl_term_range_min = new QLabel(grx_term_range);
        lbl_term_range_min->setObjectName(QString::fromUtf8("lbl_term_range_min"));
        lbl_term_range_min->setFont(font);

        horizontalLayout_6->addWidget(lbl_term_range_min);

        led_term_range_min = new QLineEdit(grx_term_range);
        led_term_range_min->setObjectName(QString::fromUtf8("led_term_range_min"));
        led_term_range_min->setFont(font);
        led_term_range_min->setAlignment(Qt::AlignCenter);
        led_term_range_min->setReadOnly(true);

        horizontalLayout_6->addWidget(led_term_range_min);

        lbl_term_range_max = new QLabel(grx_term_range);
        lbl_term_range_max->setObjectName(QString::fromUtf8("lbl_term_range_max"));
        lbl_term_range_max->setFont(font);

        horizontalLayout_6->addWidget(lbl_term_range_max);

        led_term_range_max = new QLineEdit(grx_term_range);
        led_term_range_max->setObjectName(QString::fromUtf8("led_term_range_max"));
        led_term_range_max->setFont(font);
        led_term_range_max->setAlignment(Qt::AlignCenter);
        led_term_range_max->setReadOnly(true);

        horizontalLayout_6->addWidget(led_term_range_max);


        verticalLayout_6->addWidget(grx_term_range);

        grx_term_centroid = new QGroupBox(grx_term);
        grx_term_centroid->setObjectName(QString::fromUtf8("grx_term_centroid"));
        sizePolicy.setHeightForWidth(grx_term_centroid->sizePolicy().hasHeightForWidth());
        grx_term_centroid->setSizePolicy(sizePolicy);
        grx_term_centroid->setContextMenuPolicy(Qt::NoContextMenu);
        grx_term_centroid->setFlat(true);
        horizontalLayout_3 = new QHBoxLayout(grx_term_centroid);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 3, 0, 0);
        lbl_x_term_centroid = new QLabel(grx_term_centroid);
        lbl_x_term_centroid->setObjectName(QString::fromUtf8("lbl_x_term_centroid"));
        lbl_x_term_centroid->setFont(font);
        lbl_x_term_centroid->setContextMenuPolicy(Qt::NoContextMenu);

        horizontalLayout_3->addWidget(lbl_x_term_centroid);

        led_x_term_centroid = new QLineEdit(grx_term_centroid);
        led_x_term_centroid->setObjectName(QString::fromUtf8("led_x_term_centroid"));
        led_x_term_centroid->setFont(font);
        led_x_term_centroid->setAlignment(Qt::AlignCenter);
        led_x_term_centroid->setReadOnly(true);

        horizontalLayout_3->addWidget(led_x_term_centroid);

        lbl_y_term_centroid = new QLabel(grx_term_centroid);
        lbl_y_term_centroid->setObjectName(QString::fromUtf8("lbl_y_term_centroid"));
        lbl_y_term_centroid->setFont(font);
        lbl_y_term_centroid->setContextMenuPolicy(Qt::NoContextMenu);
        lbl_y_term_centroid->setTextFormat(Qt::RichText);

        horizontalLayout_3->addWidget(lbl_y_term_centroid);

        led_y_term_centroid = new QLineEdit(grx_term_centroid);
        led_y_term_centroid->setObjectName(QString::fromUtf8("led_y_term_centroid"));
        led_y_term_centroid->setFont(font);
        led_y_term_centroid->setAlignment(Qt::AlignCenter);
        led_y_term_centroid->setReadOnly(true);

        horizontalLayout_3->addWidget(led_y_term_centroid);


        verticalLayout_6->addWidget(grx_term_centroid);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        lbl_term_area = new QLabel(grx_term);
        lbl_term_area->setObjectName(QString::fromUtf8("lbl_term_area"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(lbl_term_area->sizePolicy().hasHeightForWidth());
        lbl_term_area->setSizePolicy(sizePolicy3);
        lbl_term_area->setFont(font);

        horizontalLayout_5->addWidget(lbl_term_area);

        led_term_area = new QLineEdit(grx_term);
        led_term_area->setObjectName(QString::fromUtf8("led_term_area"));
        sizePolicy1.setHeightForWidth(led_term_area->sizePolicy().hasHeightForWidth());
        led_term_area->setSizePolicy(sizePolicy1);
        led_term_area->setMaximumSize(QSize(100, 16777215));
        led_term_area->setFont(font);
        led_term_area->setAlignment(Qt::AlignCenter);
        led_term_area->setReadOnly(true);

        horizontalLayout_5->addWidget(led_term_area);


        verticalLayout_6->addLayout(horizontalLayout_5);


        verticalLayout_3->addWidget(grx_term);

        grx_lvar = new QGroupBox(Grapher);
        grx_lvar->setObjectName(QString::fromUtf8("grx_lvar"));
        verticalLayout_7 = new QVBoxLayout(grx_lvar);
        verticalLayout_7->setSpacing(2);
        verticalLayout_7->setContentsMargins(0, 0, 0, 0);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        grx_lvar_range = new QGroupBox(grx_lvar);
        grx_lvar_range->setObjectName(QString::fromUtf8("grx_lvar_range"));
        grx_lvar_range->setFlat(true);
        horizontalLayout_7 = new QHBoxLayout(grx_lvar_range);
#ifndef Q_OS_MAC
        horizontalLayout_7->setSpacing(-1);
#endif
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(0, 3, 0, 0);
        lbl_lvar_range_min = new QLabel(grx_lvar_range);
        lbl_lvar_range_min->setObjectName(QString::fromUtf8("lbl_lvar_range_min"));
        lbl_lvar_range_min->setFont(font);

        horizontalLayout_7->addWidget(lbl_lvar_range_min);

        led_lvar_range_min = new QLineEdit(grx_lvar_range);
        led_lvar_range_min->setObjectName(QString::fromUtf8("led_lvar_range_min"));
        led_lvar_range_min->setFont(font);
        led_lvar_range_min->setAlignment(Qt::AlignCenter);
        led_lvar_range_min->setReadOnly(true);

        horizontalLayout_7->addWidget(led_lvar_range_min);

        lbl_lvar_range_max = new QLabel(grx_lvar_range);
        lbl_lvar_range_max->setObjectName(QString::fromUtf8("lbl_lvar_range_max"));
        lbl_lvar_range_max->setFont(font);

        horizontalLayout_7->addWidget(lbl_lvar_range_max);

        led_lvar_range_max = new QLineEdit(grx_lvar_range);
        led_lvar_range_max->setObjectName(QString::fromUtf8("led_lvar_range_max"));
        led_lvar_range_max->setFont(font);
        led_lvar_range_max->setAlignment(Qt::AlignCenter);
        led_lvar_range_max->setReadOnly(true);

        horizontalLayout_7->addWidget(led_lvar_range_max);


        verticalLayout_7->addWidget(grx_lvar_range);

        grx_lvar_centroid = new QGroupBox(grx_lvar);
        grx_lvar_centroid->setObjectName(QString::fromUtf8("grx_lvar_centroid"));
        sizePolicy.setHeightForWidth(grx_lvar_centroid->sizePolicy().hasHeightForWidth());
        grx_lvar_centroid->setSizePolicy(sizePolicy);
        grx_lvar_centroid->setContextMenuPolicy(Qt::NoContextMenu);
        grx_lvar_centroid->setFlat(true);
        horizontalLayout_4 = new QHBoxLayout(grx_lvar_centroid);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 3, 0, 0);
        lbl_x_lvar_centroid = new QLabel(grx_lvar_centroid);
        lbl_x_lvar_centroid->setObjectName(QString::fromUtf8("lbl_x_lvar_centroid"));
        lbl_x_lvar_centroid->setFont(font);
        lbl_x_lvar_centroid->setContextMenuPolicy(Qt::NoContextMenu);

        horizontalLayout_4->addWidget(lbl_x_lvar_centroid);

        led_x_lvar_centroid = new QLineEdit(grx_lvar_centroid);
        led_x_lvar_centroid->setObjectName(QString::fromUtf8("led_x_lvar_centroid"));
        led_x_lvar_centroid->setFont(font);
        led_x_lvar_centroid->setAlignment(Qt::AlignCenter);
        led_x_lvar_centroid->setReadOnly(true);

        horizontalLayout_4->addWidget(led_x_lvar_centroid);

        lbl_y_lvar_centroid = new QLabel(grx_lvar_centroid);
        lbl_y_lvar_centroid->setObjectName(QString::fromUtf8("lbl_y_lvar_centroid"));
        lbl_y_lvar_centroid->setFont(font);
        lbl_y_lvar_centroid->setContextMenuPolicy(Qt::NoContextMenu);
        lbl_y_lvar_centroid->setTextFormat(Qt::RichText);

        horizontalLayout_4->addWidget(lbl_y_lvar_centroid);

        led_y_lvar_centroid = new QLineEdit(grx_lvar_centroid);
        led_y_lvar_centroid->setObjectName(QString::fromUtf8("led_y_lvar_centroid"));
        led_y_lvar_centroid->setFont(font);
        led_y_lvar_centroid->setAlignment(Qt::AlignCenter);
        led_y_lvar_centroid->setReadOnly(true);

        horizontalLayout_4->addWidget(led_y_lvar_centroid);


        verticalLayout_7->addWidget(grx_lvar_centroid);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        lbl_lvar_area = new QLabel(grx_lvar);
        lbl_lvar_area->setObjectName(QString::fromUtf8("lbl_lvar_area"));
        sizePolicy.setHeightForWidth(lbl_lvar_area->sizePolicy().hasHeightForWidth());
        lbl_lvar_area->setSizePolicy(sizePolicy);
        lbl_lvar_area->setFont(font);
        lbl_lvar_area->setContextMenuPolicy(Qt::NoContextMenu);

        horizontalLayout_8->addWidget(lbl_lvar_area);

        led_lvar_area = new QLineEdit(grx_lvar);
        led_lvar_area->setObjectName(QString::fromUtf8("led_lvar_area"));
        sizePolicy1.setHeightForWidth(led_lvar_area->sizePolicy().hasHeightForWidth());
        led_lvar_area->setSizePolicy(sizePolicy1);
        led_lvar_area->setMaximumSize(QSize(100, 16777215));
        led_lvar_area->setFont(font);
        led_lvar_area->setAlignment(Qt::AlignCenter);
        led_lvar_area->setReadOnly(true);

        horizontalLayout_8->addWidget(led_lvar_area);


        verticalLayout_7->addLayout(horizontalLayout_8);


        verticalLayout_3->addWidget(grx_lvar);


        verticalLayout_2->addLayout(verticalLayout_3);


        verticalLayout->addLayout(verticalLayout_2);

        hly_area_centroid = new QHBoxLayout();
        hly_area_centroid->setObjectName(QString::fromUtf8("hly_area_centroid"));

        verticalLayout->addLayout(hly_area_centroid);


        retranslateUi(Grapher);

        QMetaObject::connectSlotsByName(Grapher);
    } // setupUi

    void retranslateUi(QWidget *Grapher)
    {
        Grapher->setWindowTitle(QApplication::translate("Grapher", "Form", 0, QApplication::UnicodeUTF8));
        lbl_name->setText(QString());
        grx_control->setTitle(QString());
        lbl_x->setText(QApplication::translate("Grapher", "x=", 0, QApplication::UnicodeUTF8));
        led_x->setText(QApplication::translate("Grapher", "0.0", 0, QApplication::UnicodeUTF8));
        lbl_y->setText(QApplication::translate("Grapher", ", &#956;=", 0, QApplication::UnicodeUTF8));
        led_y->setText(QApplication::translate("Grapher", "0.0", 0, QApplication::UnicodeUTF8));
        lbl_fuzzy->setText(QString());
        grx_term->setTitle(QApplication::translate("Grapher", "Term", 0, QApplication::UnicodeUTF8));
        grx_term_range->setTitle(QApplication::translate("Grapher", "Range", 0, QApplication::UnicodeUTF8));
        lbl_term_range_min->setText(QApplication::translate("Grapher", "Min.", 0, QApplication::UnicodeUTF8));
        led_term_range_min->setText(QApplication::translate("Grapher", "0.0", 0, QApplication::UnicodeUTF8));
        lbl_term_range_max->setText(QApplication::translate("Grapher", "Max.", 0, QApplication::UnicodeUTF8));
        led_term_range_max->setText(QApplication::translate("Grapher", "0.0", 0, QApplication::UnicodeUTF8));
        grx_term_centroid->setTitle(QApplication::translate("Grapher", "Centroid", 0, QApplication::UnicodeUTF8));
        lbl_x_term_centroid->setText(QApplication::translate("Grapher", "x=", 0, QApplication::UnicodeUTF8));
        led_x_term_centroid->setText(QApplication::translate("Grapher", "0.0", 0, QApplication::UnicodeUTF8));
        lbl_y_term_centroid->setText(QApplication::translate("Grapher", "&#956;=", 0, QApplication::UnicodeUTF8));
        led_y_term_centroid->setText(QApplication::translate("Grapher", "0.0", 0, QApplication::UnicodeUTF8));
        lbl_term_area->setText(QApplication::translate("Grapher", "Area", 0, QApplication::UnicodeUTF8));
        led_term_area->setText(QApplication::translate("Grapher", "0.0", 0, QApplication::UnicodeUTF8));
        grx_lvar->setTitle(QApplication::translate("Grapher", "Linguistic Variable", 0, QApplication::UnicodeUTF8));
        grx_lvar_range->setTitle(QApplication::translate("Grapher", "Range", 0, QApplication::UnicodeUTF8));
        lbl_lvar_range_min->setText(QApplication::translate("Grapher", "Min.", 0, QApplication::UnicodeUTF8));
        led_lvar_range_min->setText(QApplication::translate("Grapher", "0.0", 0, QApplication::UnicodeUTF8));
        lbl_lvar_range_max->setText(QApplication::translate("Grapher", "Max.", 0, QApplication::UnicodeUTF8));
        led_lvar_range_max->setText(QApplication::translate("Grapher", "0.0", 0, QApplication::UnicodeUTF8));
        grx_lvar_centroid->setTitle(QApplication::translate("Grapher", "Centroid", 0, QApplication::UnicodeUTF8));
        lbl_x_lvar_centroid->setText(QApplication::translate("Grapher", "x=", 0, QApplication::UnicodeUTF8));
        led_x_lvar_centroid->setText(QApplication::translate("Grapher", "0.0", 0, QApplication::UnicodeUTF8));
        lbl_y_lvar_centroid->setText(QApplication::translate("Grapher", "&#956;=", 0, QApplication::UnicodeUTF8));
        led_y_lvar_centroid->setText(QApplication::translate("Grapher", "0.0", 0, QApplication::UnicodeUTF8));
        lbl_lvar_area->setText(QApplication::translate("Grapher", "Area", 0, QApplication::UnicodeUTF8));
        led_lvar_area->setText(QApplication::translate("Grapher", "0.0", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Grapher: public Ui_Grapher {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRAPHER_H
