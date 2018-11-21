#include <QFileDialog>
#include <QString>
#include <QDebug>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QSpinBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "parameterestimationutils.h"
#include "drawing.h"


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	mParam1(0), 
	mParam2(0),
	mData(),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::slotFileOpen()
{
	auto fileName = QFileDialog::getOpenFileName(this, "Load Protocol");
	loadFile(fileName);
}

void MainWindow::loadFile(QString & filename)
{
	if (filename.isEmpty())
	{
		slotFileOpen();
		return;
	}

	qDebug() << "FileName: " << filename;

	// set onto status bar
	this->setStatusTip(QString("Loaded: %1").arg(filename));

	// parse the result
	mData = ResultParser::fromFile(filename.toStdString());
	drawResults(mData);

}

void MainWindow::drawResults(std::vector<ResultData*>& results)
{
	auto* scene = ui->graphicsView->scene();

	// create a new scene if we don't have one
	if (scene == NULL)
	{
		scene = new QGraphicsScene(ui->graphicsView);
	}

	// clear it
	scene->clear();

	// construct message to set into the scene
	//QString message = QString("num results: %1").arg(results.size());

	for (auto& result : results)
	{
		//qDebug() << "NumFittingItems: " << result->numFittingItems();
		//qDebug() << "NumCheckPoints:  " << result->numCheckPoints();


		// we go through all the items print their settings and all checkpoints
		int index = 0;
		//vector definieren für x/y koordianten
		std::vector< std::vector<double> > par(result->numFittingItems());
		std::vector<double> bestValue;
		std::vector<double> max;
		std::vector<double> min;

		for (auto& item : result->mCheckPoints)
		{
			bestValue.push_back(item->mBestValue);
		}
		
		for (auto& item : result->mFittingItems)
		{
			min.push_back(item->mLowerBound);
			max.push_back(item->mUpperBound);
		}

		for (auto& item : result->mFittingItems)
		{
			//qDebug() << "Name:  " << QString::fromStdString(item->mName);
			//qDebug() << "lower: " << item->mLowerBound;
			//qDebug() << "upper: " << item->mUpperBound;
			//qDebug() << "start: " << item->mStartValue;


			// go through all the checkpoints (the values found for the current parameter)

			auto& current = par[index];
			
			for (auto& checkpoint : result->mCheckPoints)
			{
				current.push_back(checkpoint->mParameters[index]);
			}

			// increment index, so we access the next parameter on the following loop
			++index;
		}

		//determine max BestValue:
		double current_maxBestValue = bestValue[0], maxBestValue = bestValue[0], minBestValue;

		for (int i = 0; i < result->numCheckPoints(); i++) {
			current_maxBestValue = bestValue[i];
			if (current_maxBestValue > maxBestValue) {
				maxBestValue = current_maxBestValue;
			}
		}

		minBestValue = *std::min_element(bestValue.begin(),bestValue.end());
		
		drawing(maxBestValue, minBestValue, par, bestValue, max, min, result, scene, mParam1, mParam2);
	} // all results
	
	ui->graphicsView->setScene(scene);
	results;
}

void MainWindow::slotUpdate()
{
	mParam1 = ui->mpParameter1->value();
	mParam2 = ui->mpParmeter2->value();

	drawResults(mData);
}
