#include "markersview.h"
#include <QFile>
#include "xmlparser.h"
#include <cmath>

MarkersView::MarkersView(QObject* parent, QColor color): ItemsView(parent, color)
{
  pen.setWidthF(0);
  brush.setStyle(Qt::NoBrush);
}

void MarkersView::load(const QString& fileName)
{
  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly))  {
    emit errorMessage("XmlFileReadError", "Can't open marker's coordinates file for reading");
  }  else {
    // trying to parse xml document
    QXmlInputSource source(&file);
    QXmlSimpleReader reader;
    data.clear();
    XmlParser handler(&data);
    reader.setContentHandler(&handler);
    if (!reader.parse(source)) {
      file.close();
      emit errorMessage("XMLParseError", handler.getErrorMessage());
    } else {
      update();
      emit updated();
    }
  }
}

void MarkersView::update()
{
  qreal minX, minY, maxX, maxY, minDistance, euclidianDistance;
  minX = minY = minDistance = std::numeric_limits<qreal>::max();
  maxX = maxY = std::numeric_limits<qreal>::min();
  for(int i=0; i<data.size(); i++) {
    if (data[i].x() < minX) 
      minX = data[i].x();
    if (data[i].y() < minY)
      minY = data[i].y();
    if (data[i].x() > maxX)
      maxX = data[i].x();
    if (data[i].y() > maxY)
      maxY = data[i].y();
    
    for(int j=i+1; j<data.size(); j++) {
      euclidianDistance = sqrt(pow(data[i].x() - data[j].x(), 2) +
			       pow(data[i].y() - data[j].y(), 2));
      if (minDistance > euclidianDistance) 
	minDistance = euclidianDistance;      
    }
  }
  sizeFactor = minDistance / 2.5;
  width = maxX - minX + sizeFactor * 2;
  height = maxY - minY + sizeFactor * 2;  
}

void MarkersView::paint(QPainter* painter)
{
  if (data.size() > 0) {
    painter->setPen(pen);
    painter->setBrush(brush);
    
    qreal xRatio = offset * size.width() / width;
    qreal yRatio = offset * size.height() / height;
    qreal x, y;
    qreal xFactor = sizeFactor * xRatio;
    qreal yFactor = sizeFactor * yRatio;
    
    for(int i=0; i<data.size(); i++) {
      x = center.x() + xRatio * data[i].x();
      y = center.y() - yRatio * data[i].y();      
      painter->drawLine(x - xFactor, y, x + xFactor, y);
      painter->drawLine(x, y - yFactor, x, y + yFactor);
    }
  }
}
