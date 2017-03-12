#include "renderwindow.h"
#include <QPainter>
#include <QPalette>

RenderWindow::RenderWindow(QWidget* parent): QWidget(parent)
{
  // устанавливаем цвет фона
  QPalette palette;
  palette.setColor(QPalette::Background, Qt::white);
  setAutoFillBackground(true);
  setPalette(palette);
  
  offset = 0.92;
  delta = 0.04;
  minOffset = 0.1;
  maxOffset = 2;
}

void RenderWindow::addSource(ItemsView* view)
{
  view->setSize(size());
  view->setOffset(offset);
  views.push_back(view);
  connect(view, SIGNAL(updated()), this, SLOT(update()));
}

#ifndef QT_NO_WHEELEVENT
void RenderWindow::wheelEvent(QWheelEvent* e)
{
  if (views.size() > 0) {
      int numDegrees = e->delta() / 8;
      int steps = numDegrees / 15;    
      qreal dsteps = delta * steps + offset;
      if ((dsteps > minOffset) && (dsteps < maxOffset)) {
	offset = dsteps;
        for(int i=0; i<views.size(); i++)
           views[i]->setOffset(offset);
	update();
      }
  }
  e->accept();
}
#endif

void RenderWindow::resizeEvent(QResizeEvent* e)
{
  for(int i=0; i<views.size(); i++)
    views[i]->setSize(e->size());
  update();
  e->accept();
}

void RenderWindow::paintEvent(QPaintEvent* e)
{
  Q_UNUSED(e);
  QPainter painter(this);
  for(int i=0; i<views.size(); i++)
    views[i]->paint(&painter);
}


