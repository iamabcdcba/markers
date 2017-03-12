#include "pointsview.h"
#include <QFile>
#include <cmath>
#include <QDebug>
#include <QMutexLocker>
#include <QWaitCondition>

PointsView::PointsView(QObject* parent, QColor color): ItemsView(parent, color)
{
  m_thread.setColor(color);
  connect(&m_thread, SIGNAL(readyRead()), this, SIGNAL(updated()), Qt::QueuedConnection);

  moveToThread(&m_thread);
  connect(&m_thread, SIGNAL(started()), this, SLOT(load()));
  connect(this, SIGNAL(updated()), this, SLOT(pendingLoad()));
  connect(this, SIGNAL(finished()), &m_thread, SLOT(quit()));  
}

void PointsView::load(const QString& fileName)
{
  m_abort = false;
  forever {
    QMutexLocker locker(&m_mutex);
    if (m_abort || readFile(fileName))
      break;
    m_condition.wait(&m_mutex);
  }
  emit finished();
}

void PointsView::paint(QPainter* painter)
{
  QMutexLocker locker(&m_thread.mutex());
  painter->drawImage(0, 0, m_thread.image(size, offset));
  m_thread.condition().wakeOne();
}

PointsView::~PointsView() {
  m_thread.stop();
  m_thread.wait(300);
}
