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

#ifndef ARMSIM_LOG_HPP_INCLUDED
#define ARMSIM_LOG_HPP_INCLUDED

#include <string>
#include <cstdint>
#include <mutex>
#include <vector>

struct LogLevel {
	enum List {
		Debug,
		Information,
		Warning,
		Error
	};
};

struct LogLine {
	uint64_t time;
	LogLevel::List level;
	std::string source;
	std::string text;
};

/**
 * Class which logs simulator events to RAM and can save them to a file.
 */
class Log {
public:
	Log() : time(0) {
	}

	void debug(const std::string &source, const std::string &text);
	void info(const std::string &source, const std::string &text);
	void warning(const std::string &source, const std::string &text);
	void error(const std::string &source, const std::string &text);

	void getContent(std::vector<LogLine> *content);
	void clearContent();
	void incrementTime();

	void save(const std::string &fileName);
private:
	void log(LogLevel::List level,
	         const std::string &source,
	         const std::string &text);

	std::mutex contentMutex;
	std::vector<LogLine> content;

	uint64_t time;
};

#endif
