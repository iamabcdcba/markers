#include "readthread.h"
#include <QFile>
#include <QPainter>
#include <QMutexLocker>
#include <limits>

const int ReadThread::BufferSize = 4000;

ReadThread::ReadThread(QObject* parent): QThread(parent)
{
  startingPos = 0;
  maxBytes = 16000 << 2;
  data = QVector<uint16_t>(BufferSize >> 1);
  brush.setStyle(Qt::SolidPattern);  
}

int ReadThread::maximumBytes()
{
  return (maxBytes >> 2);
}

void ReadThread::setColor(const QColor& color)
{
  pen.setColor(color);
  brush.setColor(color);
}

void ReadThread::setFile(const QString& fileName)
{
  if (fileName.isNull())
    return;  
  this->fileName = fileName;
  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly))
    return;
  emit limitsChanged(maximumStartingPoint(), file.size() >> 2);    
}

QMutex& ReadThread::mutex()
{
  return m_mutex;
}

QWaitCondition& ReadThread::condition()
{
  return m_condition;
}

void ReadThread::setSize(const QSizeF& size)
{
  this->size = size;
}
  
QImage ReadThread::image(const QSizeF& newSize, qreal offset)
{
  if ((buffer.isNull()) || ((offset == 1.) && (this->size == newSize)))
    return buffer;
  else 
    return buffer.scaled(newSize.width()  * offset,
                  	 newSize.height() * offset, Qt::IgnoreAspectRatio);
}

void ReadThread::setStartingPoint(int startingPoint)
{
  QMutexLocker locker(&m_mutex);
  startingPos = startingPoint << 2;
  reset = true;
  if (!isRunning())
    start();
}

void ReadThread::setTotalPoints(int totalPoints)
{
  QMutexLocker locker(&m_mutex);
  maxBytes = totalPoints << 2;
  reset = true;
  if (!isRunning())
    start();
}

void ReadThread::stop()
{
  QMutexLocker locker(&m_mutex);
  abort = true;
  m_condition.wakeOne();
}

int ReadThread::maximumStartingPoint()
{
  int m = QFile(fileName).size() - maxBytes;
  if (m > 0)
    return m >> 2;
  else
    return 0;
}

void ReadThread::run()
{
  if (fileName.isNull())
    return;

  do {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
      return;
    file.seek(startingPos);
 
    int usedBytes = 0;

    width = height = 0;
    reset = false;
    abort = false;
    buffer = QImage();

    if (!maxBytes)
      maxBytes = file.size();

    while ((!abort) && (!reset) && (usedBytes < maxBytes) && (!file.atEnd())) {
      QMutexLocker locker(&m_mutex);
      readBytes = file.read((char*)data.data(), qMin(BufferSize, maxBytes - usedBytes));
      usedBytes += readBytes;
      emit progressValueChanged(100 * usedBytes / maxBytes);
      update();
      emit readyRead();
      m_condition.wait(&m_mutex);
    }
    if ((usedBytes >= maxBytes) || (file.atEnd()))
      emit progressValueChanged(100);
    file.close();
  } while (reset);
}

void ReadThread::update()
{
  int iReadBytes = readBytes >> 1;
  uint16_t maxX, maxY;
  maxX = maxY = std::numeric_limits<uint16_t>::min();  
  for(int i=0; i<iReadBytes; i++) {
    if (data[i] > maxX)
      maxX = data[i];
    i++;
    if (data[i] > maxY)
      maxY = data[i];
  }
  qreal cx, cy;
  if (buffer.isNull()) {
    width  = maxX;
    height = maxY;
    
    buffer = QImage(size.width(), size.height(), QImage::Format_ARGB32);
    buffer.fill(qRgba(0, 0, 0, 0));    
  } else {
    if ((maxX > width) || (maxY > height)) {
      if (maxX > width) {
	cx = maxX / (qreal)width;
	width = maxX;        
      }
      if (maxY > height) {
	cy = maxY / (qreal)height;
	height = maxY;
      }
      QImage leader(size.width(), size.height(), QImage::Format_ARGB32);
      leader.fill(qRgba(0, 0, 0, 0));

      QPainter p(&leader);
      buffer = buffer.scaled(cx, cy, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
      p.drawImage(QPointF(0, 0), buffer);
      buffer = leader;
    }
  }

  QPainter p(&buffer);
  p.setPen(pen);
  p.setBrush(brush);

  qreal x, y;
  cx = size.width() / width;
  cy = size.height() / height;

  for(int i=0; i < iReadBytes; i++) {
    x = cx * data[i++];
    y = cy * data[i];
    p.drawEllipse(x, y, 1, 1);
  }
}
