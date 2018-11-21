#include <QFileDialog>
#include <QString>
#include <QDebug>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsTextItem>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "parameterestimationutils.h"

void drawing(double maxBestValue, double minBestValue, std::vector< std::vector<double> > par, std::vector<double> bestValue, std::vector<double> max, std::vector<double> min,
	ResultData *&result, QGraphicsScene *scene, int p1, int p2) {
	int cSize = 25;
	int wSize = 400;
	int parSetA = p1;
	int parSetB = p2;

	QGraphicsEllipseItem *ellipse;
	QGraphicsLineItem *lineConnect;
	QPen outlineTrack(QColor(0, 0, 255));
	outlineTrack.setWidth(2);

	QBrush redBrush(QColor(0, 0, 0));
	QPen outlinePen(QColor(0, 0, 0));
	outlinePen.setWidth(2);


	std::vector<double> par1(result->numCheckPoints());
	std::vector<double> par2(result->numCheckPoints());


	par1 = par[parSetA];
	par2 = par[parSetB];

	//determine borders

	double maxpar1 = 0, minpar1 = 0, maxpar2 = 0, minpar2 = 0;
	maxpar1 = *std::max_element(par1.begin(), par1.end());
	minpar1 = *std::min_element(par1.begin(), par1.end());
	maxpar2 = *std::max_element(par2.begin(), par2.end());
	minpar2 = *std::min_element(par2.begin(), par2.end());
	QGraphicsLineItem *line1;
	QGraphicsLineItem *line2;
	QGraphicsLineItem *line3;
	QGraphicsLineItem *line4;
	QPen outlinePen1(QColor(0, 0, 0));
	QPen outlinePen2(QColor(0, 0, 0));
	QPen outlinePen3(QColor(0, 0, 0));
	QPen outlinePen4(QColor(0, 0, 0));
	outlinePen1.setWidth(2);
	outlinePen2.setWidth(2);
	outlinePen3.setWidth(2);
	outlinePen4.setWidth(2);

	QString message1 = QString("par1_max: %1").arg(max[parSetA]);
	QString message2 = QString("par1_min: %1").arg(min[parSetA]);
	QString message3 = QString("par2_max: %1").arg(max[parSetB]);
	QString message4 = QString("par2_min: %1").arg(min[parSetB]);

	
	if (5 * maxpar1 < max[parSetA]) {
		max[parSetA] = 2 * maxpar1;
		outlinePen1.setColor(QColor(255, 0, 0));
		message1 += QString("\n(Not shown here!)\n Instead: %1").arg(max[parSetA]);
	}

	if (5 * maxpar2 < max[parSetB]) {
		max[parSetB] = 2 * maxpar2;
		outlinePen3.setColor(QColor(255, 0, 0));
		message3 += QString("\n(Not shown here!)\n Instead: %1").arg(max[parSetB]);
	}
	



	line1 = scene->addLine(wSize, wSize, min[parSetA] / max[parSetA] * wSize, wSize, outlinePen3);
	line2 = scene->addLine(wSize, wSize, wSize, min[parSetB] / max[parSetB] * wSize, outlinePen1);
	line3 = scene->addLine(min[parSetA] / max[parSetA] * wSize, min[parSetB] / max[parSetB] * wSize, min[parSetA] / max[parSetA] * wSize, wSize, outlinePen2);
	line4 = scene->addLine(min[parSetA] / max[parSetA] * wSize, min[parSetB] / max[parSetB] * wSize, wSize, min[parSetB] / max[parSetB] * wSize, outlinePen4);


	//set position of text for min/max
	auto* text1 = scene->addText(message1);
	text1->setPos(wSize, 0);
	auto* text2 = scene->addText(message2);
	text2->setPos(min[parSetA] / max[parSetA] * wSize - 100, 0);
	auto* text3 = scene->addText(message3);
	text3->setPos(wSize / 2, wSize);
	auto* text4 = scene->addText(message4);
	text4->setPos(wSize / 2, min[parSetB] / max[parSetB] * wSize);


	int colorS = 0;


	for (int i = 0; i < result->numCheckPoints(); i++) {

		colorS = (log(bestValue[i])- log(minBestValue)) / log((maxBestValue)- log(minBestValue)) * 765;
		//a[i] = ((a[i] - min) / (max - min)) * 755;
		if (colorS<256) { redBrush.setColor(QColor(colorS, 255, 0)); outlinePen.setColor(QColor(colorS, 255, 0)); }
		if (colorS<510 && colorS>255) { redBrush.setColor(QColor(255, 509 - colorS, 0)); outlinePen.setColor(QColor(255, 509 - colorS, 0)); }
		if (colorS >= 510) { redBrush.setColor(QColor(765 - colorS, 0, 0)); outlinePen.setColor(QColor(765 - colorS, 0, 0)); }

		ellipse = scene->addEllipse(par1[i] / max[parSetA] * wSize - (cSize / 2), par2[i] / max[parSetB] * wSize - (cSize / 2), cSize, cSize, outlinePen, redBrush);
		//- (cSize / 2) so the center of the circle is in the right place
		ellipse->setOpacity(0.75);
		//connecting the circles with a line
		if (i == result->numCheckPoints() - 1) { break; }
		lineConnect = scene->addLine(par1[i] / max[parSetA] * wSize, par2[i] / max[parSetB] * wSize, par1[i + 1] / max[parSetA] * wSize, par2[i + 1] / max[parSetB] * wSize, outlineTrack);
	}

	//show color coding of best value
	QBrush testBrush(QColor(0, 0, 0));
	for (int i = 0; i < 255*3; i++) {
		if(i<256){ testBrush.setColor(QColor(i, 255, 0)); outlinePen.setColor(QColor(i, 255, 0));}
		if (i<510 && i>255){ testBrush.setColor(QColor(255, 509-i, 0)); outlinePen.setColor(QColor(255, 509-i, 0)); }
		if (i>=510) { testBrush.setColor(QColor(765-i, 0, 0)); outlinePen.setColor(QColor(765-i, 0, 0)); }
		ellipse = scene->addEllipse(wSize*0.1 + i*wSize/1000, wSize*1.04, cSize/5, cSize/5, outlinePen, testBrush);
	}

	QString message5 = QString("Low BestValue");
	QString message6 = QString("High BestValue");
	auto* text5 = scene->addText(message5);
	text5->setPos(wSize*0.1 + wSize / 1000, wSize*1.05);
	auto* text6 = scene->addText(message6);
	text6->setPos(wSize*0.1 + 765 * wSize / 1000, wSize*1.05);
}

