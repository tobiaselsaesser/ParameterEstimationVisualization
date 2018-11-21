#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <vector>

namespace Ui {
class MainWindow;
}

class ResultData;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  void loadFile(QString& filename);

  void drawResults(std::vector<ResultData*>& results);

public slots:
    void slotFileOpen();
	void slotUpdate();

private:
	int mParam1;
	int mParam2;
	std::vector<ResultData*> mData;
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
