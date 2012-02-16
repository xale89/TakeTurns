/*
 * Widget.h
 *
 * CopyRight (c) 2012 Carl Andersson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef WIDGET_H
#define WIDGET_H

#include <vector>
#include <string.h>
#include "gdt.h"

class Widget{

private:
	std::vector<Widget*> children;

	static const string_t TAG;
public:
	float mX;
	float mY;

	float green;
	float red;
	float blue;
	float alpha;

	float scaleX;
	float scaleY;

	Widget(){
		children=std::vector<Widget*>();
		mX=0;
		mY=0;
		scaleX=1;
		scaleY=1;
		green=1;
		blue=1;
		red=1;
		alpha=1;
	}

	void draw();
	virtual void selfDraw(){gdt_log(LOG_NORMAL, TAG, "selfDraw");};



};




#endif //WIDGET_H
