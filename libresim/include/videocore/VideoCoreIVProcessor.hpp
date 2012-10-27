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

#ifndef RESIM_VIDEOCOREIVPROCESSOR_HPP_INCLUDED
#define RESIM_VIDEOCOREIVPROCESSOR_HPP_INCLUDED

#include "../Processor.hpp"
#include "VideoCoreIVRegisterFile.hpp"

class VideoCoreIVProcessor : public Processor {
public:
	VideoCoreIVProcessor();
	VideoCoreIVProcessor(const VideoCoreIVProcessor &other);

	virtual void setRegister(const std::string &name,
	                         uint64_t value);
	virtual uint64_t getRegister(const std::string &name);
	virtual int getRegisterSize(const std::string &name);
	virtual const std::vector<std::string> &getRegisterList();
	virtual void run(unsigned int steps);

	virtual Processor *clone() const;

	virtual std::string getName() const {
		return "VideoCore IV";
	}
private:
	VideoCoreIVRegisterFile registers;
};

#endif
