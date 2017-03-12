#include <QApplication>
#include <QDesktopWidget>
#include <QRect>
#include "window.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  Window window;

  // adjust our window size and pos
  QRect geo = QDesktopWidget().geometry();		
  window.setGeometry(geo.width() / 4, geo.height() / 4, geo.width() / 2 , geo.height() / 2);
  window.show();
  return a.exec();
}
