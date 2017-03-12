#pragma once
#include <QVector>
#include <QPointF>
#include "itemsview.h"

// Reads from file, generates, scales and renders marker images
class MarkersView: public ItemsView
{
Q_OBJECT

protected:
 QVector<QPointF> data;  // markers coordinates
 
public:
 // constructor, set given color for drawing tools
 MarkersView(QObject* parent, QColor color);

 // load coordinates from given file
 void load(const QString& fileName);

 // recalculate values for markers parameters
 void update();

 // paint items with given QPainter
 void paint(QPainter* painter);
};
