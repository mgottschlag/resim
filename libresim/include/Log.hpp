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

#ifndef RESIM_LOG_HPP_INCLUDED
#define RESIM_LOG_HPP_INCLUDED

#include <string>
#include <cstdint>
#include <mutex>
#include <vector>
#include <cstdarg>
#include <fstream>

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
	Log(const std::string &filename);

	void debug(const std::string &source, const char *fmt, ...)
			__attribute__ ((format (printf, 3, 4)));
	void info(const std::string &source, const char *fmt, ...)
			__attribute__ ((format (printf, 3, 4)));
	void warning(const std::string &source, const char *fmt, ...)
			__attribute__ ((format (printf, 3, 4)));
	void error(const std::string &source, const char *fmt, ...)
			__attribute__ ((format (printf, 3, 4)));

	void incrementTime();
private:
	void log(LogLevel::List level,
	         const std::string &source,
	         const char *fmt,
	         va_list args);

	std::mutex fileMutex;
	std::ofstream file;

	uint64_t time;
};

#endif
