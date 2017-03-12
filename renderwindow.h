#pragma once
#include <QWidget>
#include <QWheelEvent>
#include <QResizeEvent>
#include <QPaintEvent>
#include "itemsview.h"

// Realizes window for rendering markers & points
class RenderWindow : public QWidget
{
Q_OBJECT
 
private:
 QVector<ItemsView*> views;  // list of objects that control the items(markers, points)
 
 qreal offset;               // additive zoom factor is managed by mousewheel 
 qreal delta;                // sensivity of additive zoom factor
 
 qreal minOffset;            // additive zoom factor's minimum
 qreal maxOffset;            // additive zoom factor's maximum
  
public:
 RenderWindow(QWidget* parent = 0); // constructor
 void addSource(ItemsView* view);   // set new items controller

protected:
 // standard events overriding
 #ifndef QT_NO_WHEELEVENT
 void wheelEvent(QWheelEvent *) override;   
 #endif
 void resizeEvent(QResizeEvent *) override; 
 void paintEvent(QPaintEvent * ) override;
};
 
 
