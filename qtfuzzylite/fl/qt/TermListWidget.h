/* 
 * File:   TermListWidget.h
 * Author: jcrada
 *
 * Created on 20 April 2013, 11:22 AM
 */

#ifndef FL_TERMLISTWIDGET_H
#define	FL_TERMLISTWIDGET_H


#include <QtGui/QListWidget>

namespace fl {
    namespace qt {

        class TermListWidget : public QListWidget {
            Q_OBJECT

        protected:
            std::string _variableType;
            void contextMenuEvent(QContextMenuEvent* event);

        public:
            TermListWidget(QWidget* parent = NULL);

            void setVariableType(const std::string& variableType);
            std::string getVariableType() const;

        };
    }
}

#endif	/* TERMLISTWIDGET_H */

