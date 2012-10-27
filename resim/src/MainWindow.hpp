/*
Copyright (C) 2012, Mathias Gottschlag

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef ARMSIM_MAINWINDOW_HPP_INCLUDED
#define ARMSIM_MAINWINDOW_HPP_INCLUDED

#include "SimulationThread.hpp"
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow {
	Q_OBJECT
public:
	MainWindow();
public slots:
	void about();

	void newConfig();
	void open();
	void save();
	void saveAs();

	void selectProcessor(int processor);
	void changeRegister();

	void addDevice();
	void removeDevice();
	void changeDevice();

	void addMemoryContent();
	void removeMemoryContent();
	void changeMemoryContent();

	void refreshLog();
	void clearLog();
	void saveLog();

	void refreshProcessorState();

	void start();
	void pause();
	void stop();
private slots:
	void updateStartPauseStop();
protected:
	void closeEvent(QCloseEvent *event);
private:
	void updateMemoryContentList();

	Ui::MainWindow ui;

	SimulationThread simulation;

	bool simulationStarted;
};

#endif
