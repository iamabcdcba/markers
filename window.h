#pragma once
#include <QWidget>
#include <QSlider>
#include <QSpinBox>
#include <QProgressBar>
#include "renderwindow.h"
#include "markersview.h"
#include "pointsview.h"

// main window class
class Window: public QWidget
{  
Q_OBJECT

private:
 MarkersView* markersView;           // object manages markerks
 PointsView* pointsView;             // object manages points
 RenderWindow* renderWindow;         // area for rendering markers & points

 QSlider* startingPointSlider;       // controls for starting
 QSpinBox* startingPointSpinBox;     // point value managing
 
 QSlider* totalPointsSlider;         // controls for total points
 QSpinBox* totalPointsSpinBox;       // value managing
 
 QProgressBar* readThreadProgressBar; // thread reading progress indicator
 
public: 
 Window(QWidget* parent=0);   // constructor

private slots:
 void errorMessage(QString title, QString message);  // open error message box
 
 void slotBrowseMarkersFile();  // open dialog to choose markers data file
 void slotBrowsePointsFile();   // open dialog to choose points data file
 
 void slotChangeStartingPoint(int);  // execute when starting point value changed
 void slotChangeLimits(int, int);    // execute when starting point & total points
                                     // maximums changed
 
 void slotChangeTotalPoints(int);    // execute when total points value changed
};
 
 
