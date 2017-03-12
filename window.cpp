#include "window.h"
#include <QGraphicsScene>
#include <QFile>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QLabel>

Window::Window(QWidget* parent): QWidget(parent)
{
  setWindowTitle(tr("Markers & points"));
  ////!
  QPushButton* browseMarkersFile = new QPushButton(tr("Load markers"));
  connect(browseMarkersFile, SIGNAL(clicked()), this, SLOT(slotBrowseMarkersFile()));
  QPushButton* browsePointsFile = new QPushButton(tr("Load points"));
  connect(browsePointsFile, SIGNAL(clicked()), this, SLOT(slotBrowsePointsFile()));
  QVBoxLayout* browseButtonsLayout = new QVBoxLayout();
  browseButtonsLayout->addWidget(browseMarkersFile);
  browseButtonsLayout->addWidget(browsePointsFile);
  browseButtonsLayout->addStretch(1);

  ////!!
  QLabel* startingPointLabel = new QLabel(tr("Starting point:"));
  startingPointSlider = new QSlider(Qt::Horizontal);
  startingPointSlider->setDisabled(true);
  startingPointSpinBox = new QSpinBox();
  startingPointSpinBox->setDisabled(true);

  QLabel* totalPointsLabelHead = new QLabel(tr("Show up to:"));
  totalPointsSlider = new QSlider(Qt::Horizontal);
  totalPointsSlider->setDisabled(true);  
  totalPointsSpinBox = new QSpinBox();
  totalPointsSpinBox->setDisabled(true);

  QLabel* readThreadLabel = new QLabel(tr("Reading progress:"));
  readThreadProgressBar = new QProgressBar();
  readThreadProgressBar->setMinimum(0);
  readThreadProgressBar->setMaximum(100);
  readThreadProgressBar->setValue(0);

  connect(startingPointSpinBox, SIGNAL(valueChanged(int)),startingPointSlider, SLOT(setValue(int)));
  connect(startingPointSlider, SIGNAL(valueChanged(int)), startingPointSpinBox, SLOT(setValue(int)));
  connect(startingPointSpinBox,SIGNAL(valueChanged(int)), this, SLOT(slotChangeStartingPoint(int)));

  connect(totalPointsSpinBox, SIGNAL(valueChanged(int)), totalPointsSlider, SLOT(setValue(int)));
  connect(totalPointsSlider, SIGNAL(valueChanged(int)), totalPointsSpinBox, SLOT(setValue(int)));
  connect(totalPointsSpinBox, SIGNAL(valueChanged(int)), this, SLOT(slotChangeTotalPoints(int)));

  QGridLayout* pointsControlsLayout = new QGridLayout();
  pointsControlsLayout->addWidget(startingPointLabel, 0, 0, Qt::AlignRight);
  pointsControlsLayout->addWidget(startingPointSlider, 0, 1);
  pointsControlsLayout->addWidget(startingPointSpinBox, 0, 2);
  pointsControlsLayout->addWidget(totalPointsLabelHead, 1, 0, Qt::AlignRight);
  pointsControlsLayout->addWidget(totalPointsSlider, 1, 1);
  pointsControlsLayout->addWidget(totalPointsSpinBox, 1, 2);
  pointsControlsLayout->addWidget(readThreadLabel, 2, 0);
  pointsControlsLayout->addWidget(readThreadProgressBar, 2, 1);
				 
  QHBoxLayout* topLayout = new QHBoxLayout();
  topLayout->addLayout(browseButtonsLayout);
  topLayout->addLayout(pointsControlsLayout);

  ////!!!
  markersView = new MarkersView(this, Qt::blue);
  pointsView = new PointsView(this, Qt::red);

  connect(pointsView->thread(), SIGNAL(limitsChanged(int, int)), this,
	  SLOT(slotChangeLimits(int, int)));
  connect(pointsView->thread(), SIGNAL(progressValueChanged(int)), readThreadProgressBar,
	  SLOT(setValue(int)));

  totalPointsSlider->setMinimum(pointsView->thread()->maximumBytes());
  totalPointsSpinBox->setMinimum(totalPointsSlider->minimum());  
  
  renderWindow = new RenderWindow();
  renderWindow->addSource(pointsView);
  renderWindow->addSource(markersView);
  
  QVBoxLayout* mainLayout = new QVBoxLayout();
  mainLayout->addLayout(topLayout, 1);
  mainLayout->addWidget(renderWindow, 100);
  setLayout(mainLayout); 
}

void Window::slotChangeLimits(int maxStartingPoint, int maxTotalPoints)
{
  qDebug() << "maxTotalPoints = " << maxTotalPoints;
  startingPointSlider->setMaximum(maxStartingPoint);
  startingPointSpinBox->setMaximum(maxStartingPoint);
  totalPointsSpinBox->setMaximum(maxTotalPoints);
  totalPointsSlider->setMaximum(maxTotalPoints);
  startingPointSlider->setEnabled(true);
  startingPointSpinBox->setEnabled(true);
  totalPointsSpinBox->setEnabled(true);
  totalPointsSlider->setEnabled(true);
}

void Window::slotChangeTotalPoints(int value)
{
  pointsView->thread()->setTotalPoints(value);
  startingPointSpinBox->setMaximum(pointsView->thread()->maximumStartingPoint());
  startingPointSlider->setMaximum(startingPointSpinBox->maximum());
}

void Window::slotChangeStartingPoint(int value)
{
  pointsView->thread()->setStartingPoint(value);
}

void Window::slotBrowseMarkersFile()
{ 
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open file with crosses data"),
						  ".", tr("XML file (*.xml)"));
  if (!fileName.isNull())
    markersView->load(fileName);
}

void Window::slotBrowsePointsFile()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open file with points data"),
						  ".", tr("Binary file (*.bin)"));
  if (!fileName.isNull()) {
    pointsView->load(fileName);
    pointsView->thread()->setTotalPoints(totalPointsSpinBox->value());
  }
}

void Window::errorMessage(QString title, QString message)
{
  QMessageBox::critical(this, title, message);
}
