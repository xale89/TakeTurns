/*
 * Node.h
 *
 * Copyright (c) 2012 Carl Andersson
 * Copyright (c) 2012 Sebastian Ärleryd
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

#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string.h>
#include <gdt/gdt.h>
#include <glm/glm.hpp>

#include "Shader.h"

class Node {

private:
	static const string_t TAG;

	std::vector<Node*> mChildren;

protected:
	glm::mat4 mModelMatrix;

	static Shader *sShader;
	//Shader mShader;
	/*Node(Shader shader){
		mShader=shader;
		mShader=sShader;
		children=std::vector<Widget*>();
		mX=0;
		mY=0;
		mScaleX=1;
		mScaleY=1;
		mColorGreen=1;
		mColorBlue=1;
		mColorRed=1;
		mColorAlpha=1;
	}*/

public:
	float mX;
	float mY;

	float mColorGreen;
	float mColorRed;
	float mColorBlue;
	float mColorAlpha;

	float mScaleX;
	float mScaleY;

	Node();

	void move(float x,float y);

	void draw();
	virtual void selfDraw();
};

#endif //NODE_H
