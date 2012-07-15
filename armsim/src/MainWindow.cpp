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

#include "MainWindow.hpp"

#include <QMessageBox>

MainWindow::MainWindow() : QMainWindow() {
	ui.setupUi(this);
}

void MainWindow::about() {
	QMessageBox::about(this, "armsim", "Simple ARM Simulator");
}

void MainWindow::closeEvent(QCloseEvent *event) {
}

void MainWindow::newConfig() {
	QMessageBox::critical(this, "Error", "Not implemented.");
	// TODO
}
void MainWindow::open() {
	QMessageBox::critical(this, "Error", "Not implemented.");
	// TODO
}
void MainWindow::save() {
	QMessageBox::critical(this, "Error", "Not implemented.");
	// TODO
}
void MainWindow::saveAs() {
	QMessageBox::critical(this, "Error", "Not implemented.");
	// TODO
}

void MainWindow::selectProcessor(int processor) {
	QMessageBox::critical(this, "Error", "Not implemented.");
	// TODO
}
void MainWindow::changeRegister() {
	QMessageBox::critical(this, "Error", "Not implemented.");
	// TODO
}

void MainWindow::addDevice() {
	QMessageBox::critical(this, "Error", "Not implemented.");
	// TODO
}
void MainWindow::removeDevice() {
	QMessageBox::critical(this, "Error", "Not implemented.");
	// TODO
}
void MainWindow::changeDevice() {
	QMessageBox::critical(this, "Error", "Not implemented.");
	// TODO
}

void MainWindow::addMemoryContent() {
	QMessageBox::critical(this, "Error", "Not implemented.");
	// TODO
}
void MainWindow::removeMemoryContent() {
	QMessageBox::critical(this, "Error", "Not implemented.");
	// TODO
}
void MainWindow::changeMemoryContent() {
	QMessageBox::critical(this, "Error", "Not implemented.");
	// TODO
}

void MainWindow::refreshLog() {
	QMessageBox::critical(this, "Error", "Not implemented.");
	// TODO
}
void MainWindow::clearLog() {
	QMessageBox::critical(this, "Error", "Not implemented.");
	// TODO
}
void MainWindow::saveLog() {
	QMessageBox::critical(this, "Error", "Not implemented.");
	// TODO
}

void MainWindow::refreshProcessorState() {
	QMessageBox::critical(this, "Error", "Not implemented.");
	// TODO
}

void MainWindow::start() {
	QMessageBox::critical(this, "Error", "Not implemented.");
	// TODO
}
void MainWindow::pause() {
	QMessageBox::critical(this, "Error", "Not implemented.");
	// TODO
}
void MainWindow::stop() {
	QMessageBox::critical(this, "Error", "Not implemented.");
	// TODO
}
