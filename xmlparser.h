#pragma once
#include <QXmlDefaultHandler>
#include <QVector>
#include <QPointF>

// Parse xml file for coordinates
class XmlParser : public QXmlDefaultHandler
{
private:
  QString errorMessage;   // xml parsing error
  QVector<QPointF>* data; // coordinates

public:
  // constructor, start parsing to fill specifying vector of coordinates
  XmlParser(QVector<QPointF>* data); 

  // process following xml element
  bool startElement(const QString&,
		    const QString&,
		    const QString& localName,
		    const QXmlAttributes& attrs
		    );

  // process raised exception
  bool fatalError(const QXmlParseException& exception);

  // return error message
  QString getErrorMessage();

};

  

	  

