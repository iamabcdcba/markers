#include "xmlparser.h"
#include <QDebug>

XmlParser::XmlParser(QVector<QPointF>* data): QXmlDefaultHandler(), data(data)
{
}

bool XmlParser::startElement(const QString&,
				const QString&,
				const QString& localName,
				const QXmlAttributes& attrs
				)
{
    int attrIndex;      // position of element's attribute
    QString attrValue;  // value of element's attribute
    // read the expected number of points
    if (localName == "Plot")
    {
      // get value of corresponding attribute
      attrIndex = attrs.index("Caption");      
      if (attrIndex != -1)
      {
	attrValue = attrs.value(attrIndex);
	// get count of points
	QRegExp rx("\\w*(\\d+)\\s*Points");	
	if (rx.indexIn(attrValue) > -1)
	{
	  // check result
	  bool ok = false;
	  int count = rx.cap(1).toInt(&ok, 10);	  	  
	  if (ok && (count > 0) && (count < 16000))
	    data->reserve(count);
	}	
      }
    }
    // get (x, y) coordinates of point    
    else if (localName == "Point")
    {
      double x = 0, y = 0;
      bool xOk = false, yOk = false;
      // get x coordinate of point
      attrIndex = attrs.index("X");
      if (attrIndex != -1)
      {
	attrValue = attrs.value(attrIndex);
	// check result
	x = attrValue.toDouble(&xOk);
      }
      // get y coordinate of point
      attrIndex = attrs.index("Y");
      if (attrIndex != -1)
      {
	attrValue = attrs.value(attrIndex);
	// check result
	y = attrValue.toDouble(&yOk);
      }
      // check x & y summary values
      if (xOk && yOk)
      {
	data->push_back(QPointF(x, y));
      }
    }
    return true;
}

bool XmlParser::fatalError(const QXmlParseException& exception)
{
  errorMessage = QString("%1. Line: %2, Column: %3.").arg(exception.lineNumber())
    .arg(exception.columnNumber()).arg(exception.message());
  return false;
}

QString XmlParser::getErrorMessage()
{
  return errorMessage;
}
