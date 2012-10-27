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

#include "Log.hpp"

#include <fstream>
#include <iostream>

void Log::debug(const std::string &source, const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	log(LogLevel::Debug, source, fmt, args);
	va_end( args );
}
void Log::info(const std::string &source, const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	log(LogLevel::Information, source, fmt, args);
	va_end( args );
}
void Log::warning(const std::string &source, const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	log(LogLevel::Warning, source, fmt, args);
	va_end( args );
}
void Log::error(const std::string &source, const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	log(LogLevel::Error, source, fmt, args);
	va_end( args );
}

void Log::getContent(std::vector<LogLine> *content) {
	contentMutex.lock();
	*content = this->content;
	contentMutex.unlock();
}
void Log::clearContent() {
	contentMutex.lock();
	content.clear();
	contentMutex.unlock();
}
void Log::incrementTime() {
	time++;
}

void Log::save(const std::string &fileName) {
	std::ofstream file(fileName);
	if (!file) {
		// TODO: Proper exception class
		throw std::runtime_error("Log file could not be opened.");
	}
	contentMutex.lock();
	for (LogLine line : content) {
		file << line.time << "\t" << line.level << "\t" << line.source << "\t"
				<< line.text << std::endl;
	}
	contentMutex.unlock();
}

void Log::log(LogLevel::List level,
              const std::string &source,
              const char *fmt,
              va_list args) {
	char text[256];
	vsnprintf(text, 256, fmt, args);
	LogLine line;
	line.time = time;
	line.level = level;
	line.source = source;
	line.text = text;
	contentMutex.lock();
	content.push_back(line);
	std::cout << text << std::endl;
	contentMutex.unlock();
}
