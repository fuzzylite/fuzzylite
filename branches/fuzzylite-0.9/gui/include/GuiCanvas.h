/*
 * GuiCanvas.h
 *
 *  Created on: Dec 18, 2009
 *      Author: jcrada
 */

#ifndef _GUICANVAS_H_
#define _GUICANVAS_H_

#include <QtGui/QGraphicsView>
#include <QtGui/QResizeEvent>
#include <fuzzylite/FuzzyLite.h>
#include <QtGui/QColor>

namespace fl_gui {

	class GuiCanvas: public QGraphicsView {
	Q_OBJECT
	private:
		fl::flScalar _minimum, _maximum;
	protected:
		void resizeEvent(QResizeEvent *event);
		virtual void ensureVisible();
	public:
		GuiCanvas(QWidget* parent = NULL);
		virtual ~GuiCanvas();

		virtual fl::flScalar minimumLVar();
		virtual void setMinimumLVar(fl::flScalar min);
		virtual fl::flScalar maximumLVar();
		virtual void setMaximumLVar(fl::flScalar max);

		virtual QRect drawingRect() const;
		virtual void drawVariable(const fl::LinguisticVariable& var, const QColor& from =
			QColor(255, 255, 0, 75), const QColor& to = QColor(255, 0, 0, 175));
		virtual void drawTerm(const fl::LinguisticTerm& term, const QColor& color =
			QColor(0, 204, 0, 230));
		virtual void drawGuide(fl::flScalar x, fl::flScalar y, const QColor& color =
			QColor(0, 210, 0, 255));
		virtual void drawCentroid(fl::flScalar x, fl::flScalar y);

		virtual void clear();
	};

} // namespace fl_gui
#endif /* _GUICANVAS_H_ */
