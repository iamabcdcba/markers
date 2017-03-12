#pragma once
#include "itemsview.h"
#include <QVector>
#include <QRectF>
#include "readthread.h"

// Reads from file, generates, scales and renders point images
class PointsView: public ItemsView
{
Q_OBJECT

protected:
 ReadThread m_thread; // thread for reading coordinates & render images

public:
 // constructor, set given color for drawing tools
 PointsView(QObject* parent, QColor color);
 ~PointsView();

  // load coordinates from given file
 void load(const QString& fileName);

  // paint point with given QPainter
 void paint(QPainter* painter);

 // get reading thread   
 ReadThread* thread();                
};
