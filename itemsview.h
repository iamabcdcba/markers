#pragma once
#include <QPointF>
#include <QSizeF>
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QColor>

// Read from file, generate, scale and render individual items
// specifying in the derived classes
class ItemsView: public QObject
{
Q_OBJECT
 
protected:
 QPointF center;   // center of rendering window
 QSizeF size;      // size of rendering window
 qreal offset;     // additive zoom factor
  
 qreal width;      // coordinates bounding rectangle width
 qreal height;     // coordinates bounding rectangle height
 qreal sizeFactor; // zoom factor to adjust the item size
  
 QPen pen;         // pen for drawing item
 QBrush brush;     // brush for drawing item
  
public:
 // constructor, set given color for drawing tools
 ItemsView(QObject* parent = 0, QColor color = Qt::blue): QObject(parent)
 {
   // prepare drawing instruments
   pen.setColor(color);
   brush.setColor(color);
 }

 // virtual destructor, to avoid memory leak error
 virtual ~ItemsView() {}

 // set size of rendering window
 inline void setSize(const QSizeF& size)
 {
   this->size = size;
   // adjust center coodrinates
   center = QPointF(size.width() / 2., size.height() / 2.);
 }

 // set additive zoom factor
 inline void setOffset(qreal offset)
 {
   this->offset = offset;
 }

 virtual void load(const QString& fileName) = 0; // load coordinates from given file

 virtual void paint(QPainter* painter) = 0;      // paint items with given QPainter

signals:
 void errorMessage(QString title, QString message); // execute when error occurs
 void updated();                          // execute when need to redraw content
};
