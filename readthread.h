#pragma once
#include <QThread>
#include <QVector>
#include <QMutex>
#include <QWaitCondition>
#include <QImage>
#include <QPen>
#include <QBrush>
#include <QSize>

// read coordinates & generate image within the new thread
class ReadThread: public QThread
{
Q_OBJECT

private:
 static const int BufferSize;  // size of read buffer
 QString fileName;             // name of file to read 
 QVector<uint16_t> data;       // vector of coordinates
 int readBytes;                // amount of read bytes
 QMutex m_mutex;               // mutex for protecting data
 QWaitCondition m_condition;   // wait condition for organize communicating with gui
 
 bool abort;                   // flag to abort thread running
 bool reset;                   // flag to reset thread running
 QImage buffer;                // generating result
 
 uint16_t width;               // width of coordinates bounding rect
 uint16_t height;              // height of coordinates bounding rect
 
 QSizeF size;                  // size of rendering area
 QPen pen;                     // pen for drawing points
 QBrush brush;                 // brush for drawing points
 
 int startingPos;              // starting pos of coordinates vector
 int maxBytes;                 // maximum bytes can be read
 
 void update();                // recalculate values
 
public:  
  ReadThread(QObject* parent = 0);
  // set color for drawing tools
  void setColor(const QColor& color);
  // set file for reading
  void setFile(const QString& fileName);
  // stop thread running
  void stop();
  // get mutex
  QMutex& mutex();
  // get wait condition variable
  QWaitCondition& condition();
  // set size of rendering area
  void setSize(const QSizeF& size);
  // set total points value
  void setTotalPoints(int totalPoints);
  // get generated image
  QImage image(const QSizeF& newSize, qreal offset);
  // set starting point value
  void setStartingPoint(int startingPoint);
  // get maximum value of starting point
  int maximumStartingPoint();
  // get maximum bytes to render
  int maximumBytes();

protected:
  // execute when run thread
 void run() override;

  
 signals:
 // execute when next data block was read
 void progressValueChanged(int);
 // execute when file was checked  
 void limitsChanged(int, int);
 // execute when data was generated
 void readyRead();
};  
  
