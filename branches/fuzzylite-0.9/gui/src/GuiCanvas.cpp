/*
 * GuiCanvas.cpp
 *
 *  Created on: Dec 20, 2009
 *      Author: jcrada
 */

#include "GuiCanvas.h"
#include "QtGui/QGraphicsPolygonItem"
namespace fl_gui {

	GuiCanvas::GuiCanvas(QWidget* parent) :
		QGraphicsView(new QGraphicsScene, parent), _minimum(-INFINITY),
			_maximum(INFINITY) {
		setRenderHints(renderHints() | QPainter::Antialiasing
			| QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);
		setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	}

	GuiCanvas::~GuiCanvas() {
		delete scene();
	}

	fl::flScalar GuiCanvas::minimumLVar() {
		return _minimum;
	}
	void GuiCanvas::setMinimumLVar(fl::flScalar min) {
		this->_minimum = min;
	}
	fl::flScalar GuiCanvas::maximumLVar() {
		return _maximum;
	}
	void GuiCanvas::setMaximumLVar(fl::flScalar max) {
		this->_maximum = max;
	}

	void GuiCanvas::clear() {
		scene()->clear();
	}

	void GuiCanvas::resizeEvent(QResizeEvent* e) {
		(void) e;
		ensureVisible();
	}

	void GuiCanvas::ensureVisible() {
		fitInView(drawingRect(), Qt::IgnoreAspectRatio);
		centerOn(drawingRect().center());
	}

	QRect GuiCanvas::drawingRect() const {
		int line_width = 2;
		int slider = 15; //Slider ball in mac 10.6
		QRect result = viewport()->rect();
		fl::flScalar sides = 0.0;
		result.translate(-result.width() / 2, -result.height() / 2);
		int min_x_canvas = result.left() + slider / 2 + sides, max_x_canvas =
			result.right() - slider / 2 - sides, //15 is size of slider ball in macosx
			min_y_canvas = 0 + (result.height() * 1 / 2) - line_width, max_y_canvas = 0
				- (result.height() * 1 / 2) + line_width;
		result.setLeft(min_x_canvas);
		result.setRight(max_x_canvas);
		result.setTop(max_y_canvas);
		result.setBottom(min_y_canvas);
		return result;
	}

	void GuiCanvas::drawVariable(const fl::LinguisticVariable& var, const QColor& from,
		const QColor& to) {
		setMinimumLVar(var.minimum());
		setMaximumLVar(var.maximum());
		for (int i = 0; i < var.numberOfTerms(); ++i) {
			int r, g, b, a;
			fl::flScalar degree = (i + 1.0) / (var.numberOfTerms());
			fl::FuzzyOperator::ColorGradient(from.red(), from.green(), from.blue(),
				from.alpha(), to.red(), to.green(), to.blue(), to.alpha(), degree, r, g,
				b, a);
			drawTerm(*var.term(i), QColor(r, g, b, a));
		}
	}

	void GuiCanvas::drawTerm(const fl::LinguisticTerm& term, const QColor& color) {
		int line_width = 1;
		QRect rect = drawingRect();

		std::vector<fl::flScalar> samples_x, samples_y;
		term.samples(samples_x, samples_y, term.fuzzyOperator().samples(), 2);
		QPolygon polygon;
		polygon.append(QPoint(rect.left(), rect.bottom()));
		for (size_t j = 0; j < samples_x.size(); ++j) {
			fl::flScalar x = fl::FuzzyOperator::Scale(minimumLVar(), maximumLVar(),
				samples_x[j], rect.left(), rect.right());
			fl::flScalar y = fl::FuzzyOperator::Scale(0, 1, samples_y[j], rect.bottom(),
				rect.top());
			polygon.append(QPoint(x, y));
		}
		polygon.append(QPoint(rect.right(), rect.bottom()));

		QPen pen;
		pen.setWidth(line_width);
		pen.setColor(QColor(Qt::black));
		pen.setStyle(Qt::SolidLine);
		QBrush brush;
		brush.setColor(color);
		brush.setStyle(Qt::SolidPattern);
		QGraphicsPolygonItem* item = scene()->addPolygon(polygon, pen, brush);
		static qreal zvalue = INT_MIN; //each figure will be on top.
		item->setZValue(++zvalue);

		fl::flScalar centroid_x, centroid_y;
		term.centroid(centroid_x, centroid_y);
		drawCentroid(centroid_x,centroid_y);
		ensureVisible();
	}

	void GuiCanvas::drawGuide(fl::flScalar x, fl::flScalar y, const QColor& color) {
		QRect rect = drawingRect();
		x = fl::FuzzyOperator::Scale(minimumLVar(), maximumLVar(), x, rect.left(),
			rect.right());
		y = fl::FuzzyOperator::Scale(0, 1, y, rect.bottom(), rect.top());
		QPen pen;
		pen.setColor(color);
		pen.setStyle(Qt::DashLine);
		pen.setWidth(1);
		scene()->addLine(x, rect.bottom(), x, y, pen);
	}

	void GuiCanvas::drawCentroid(fl::flScalar x, fl::flScalar y) {
		QRect rect = drawingRect();
		x = fl::FuzzyOperator::Scale(minimumLVar(), maximumLVar(), x, rect.left(),
			rect.right());
		y = fl::FuzzyOperator::Scale(0, 1, y, rect.bottom(), rect.top());
		QPen pen;
		pen.setColor(QColor(Qt::blue));
		scene()->addEllipse(x, y, 3, 3, pen);
	}

} // namespace fl_gui
