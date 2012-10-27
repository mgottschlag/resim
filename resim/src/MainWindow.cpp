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
#include "Processor.hpp"
#include "Device.hpp"

#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <cassert>

MainWindow::MainWindow() : QMainWindow(), simulationStarted(false) {
	ui.setupUi(this);
	updateStartPauseStop();
	connect(&simulation, SIGNAL(runningChanged()), this,
			SLOT(updateStartPauseStop()));
	// Initialize the window content
	auto processors = Processor::getProcessorTypes();
	assert(processors.size() > 0);
	for (unsigned int i = 0; i < processors.size(); i++) {
		ui.cpuModel->addItem(processors[i]->getName().c_str());
	}
	ui.cpuModel->setCurrentIndex(0);
	// Initialize the processor
	selectProcessor(0);
}

void MainWindow::about() {
	QMessageBox::about(this, "resim", "Simple System Simulator");
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
	// Replace the current processor with a copy of the selected one
	const Processor *prototype = Processor::getProcessorTypes()[processor];
	Processor *current = simulation.getProcessor();
	if (current != NULL && prototype->getName() == current->getName()) {
		return;
	}
	current = prototype->clone();
	simulation.setProcessor(current);
	// The register list needs to be updated
	ui.initialRegisterList->clear();
	auto registers = current->getRegisterList();
	for (auto reg = registers.begin(); reg != registers.end(); reg++) {
		int size = current->getRegisterSize(*reg);
		uint64_t value = current->getRegister(*reg);
		QString line = QString("%1: 0x%2")
				.arg(reg->c_str())
				.arg(value, size * 2, 16, QLatin1Char('0'));
		ui.initialRegisterList->addItem(line);
	}
}
void MainWindow::changeRegister() {
	QMessageBox::critical(this, "Error", "Not implemented.");
	// TODO
}

void MainWindow::addDevice() {
	QInputDialog dialog(this);
	auto devices = Device::getDeviceTypes();
	assert(devices.size() > 0);
	QStringList deviceNames;
	for (unsigned int i = 0; i < devices.size(); i++) {
		deviceNames.append(devices[i]->getName());
	}
	dialog.setLabelText("Select a device type:");
	dialog.setComboBoxItems(deviceNames);
	if (dialog.exec() != QDialog::Accepted) {
		return;
	}
	int selected = deviceNames.indexOf(dialog.textValue());
	assert(selected != -1);
	simulation.addDevice(devices[selected]->clone());
	// TODO: Proper device description
	ui.deviceList->addItem(deviceNames[selected]);
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
	QString fileName =
			QFileDialog::getOpenFileName(this, tr("Open Memory Image"));
	if (fileName.isNull()) {
		return;
	}
	// Read the complete file
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly)) {
		QMessageBox::critical(this, "Error",
				"Could not open the selected file");
		return;
	}
	QByteArray content = file.readAll();
	file.close();
	MemoryContent *memoryContent = new MemoryContent();
	memoryContent->name = QFileInfo(fileName).fileName();
	memoryContent->data = content;
	memoryContent->baseAddress = 0;
	simulation.addMemoryContent(memoryContent);
	// Update the memory content list
	updateMemoryContentList();
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
	if (simulationStarted == false) {
		simulation.reset();
		simulationStarted = true;
	}
	simulation.start();
}
void MainWindow::pause() {
	simulation.stop();
}
void MainWindow::stop() {
	simulation.stop();
	simulationStarted = false;
	updateStartPauseStop();
}

void MainWindow::updateStartPauseStop() {
	bool running = simulation.isRunning();
	ui.actionStartSimulation->setEnabled(!running);
	ui.actionPauseSimulation->setEnabled(running);
	ui.actionStopSimulation->setEnabled(simulationStarted);
}

void MainWindow::updateMemoryContentList() {
	ui.memoryContentList->clear();
	QStringList lines;
	auto memoryContent = simulation.getMemoryContent();
	for (auto it = memoryContent.begin(); it != memoryContent.end(); it++) {
		uintptr_t end = (*it)->baseAddress + (*it)->data.length();
		QString line = QString("0x%2 - 0x%3: %1")
				.arg((*it)->name)
				.arg((*it)->baseAddress, 8, 16, QLatin1Char('0'))
				.arg(end, 8, 16, QLatin1Char('0'));
		lines.append(line);
	}
	ui.memoryContentList->addItems(lines);
}
