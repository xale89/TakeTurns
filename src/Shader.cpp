/*
 * Shader.cpp
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

#include "Shader.h"



const string_t Shader::TAG = "Shader";

std::map<std::string, Shader*> Shader::sLoadedShaders;

GLuint Shader::compileShader(string_t shaderCode, GLenum type){
	GLuint shader = glCreateShader(type);

	int len = strlen(shaderCode);
	glShaderSource(shader, 1, &shaderCode, &len);

	glCompileShader(shader);

	GLint result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE) {
		//Get the length of the error log
		GLint logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

		//Get and print the error log and then exit with error
		char *infoLog = (char *) malloc(logLength);
		glGetShaderInfoLog(shader, logLength, NULL, infoLog);
		gdt_fatal(TAG, "Error compiling shader: %s", infoLog);
	}
	return shader;
}

GLuint Shader::createProgram(std::string vertFile, std::string fragFile) {
	GdtResource vRes = GdtResource(vertFile), fRes = GdtResource(fragFile);

		void *vertRawdata,*fragRawdata;
		int vertLength,fragLength;

		vertLength = vRes.getLength();
		vertRawdata = vRes.getBytes();


		fragLength = fRes.getLength();
		fragRawdata = fRes.getBytes();

		if (vertLength * fragLength == 0){
			gdt_fatal(TAG, "Unable to load shaders!: %s, %s", vertFile.c_str(),fragFile.c_str());
		}

		char *vertText = (char *) malloc(vertLength+1);
		memset(vertText, 0, vertLength+1);
		memcpy(vertText, vertRawdata, vertLength);

		char *fragText = (char *) malloc(fragLength+1);
		memset(fragText, 0, fragLength+1);
		memcpy(fragText, fragRawdata, fragLength);

		GLuint vertexShader = compileShader(vertText, GL_VERTEX_SHADER);
		GLuint fragmentShader = compileShader(fragText, GL_FRAGMENT_SHADER);

		GLErrorAssert(TAG, "Got error %s from OpenGL when compiling fragment shader %s and vertex shader %s.", fragFile.c_str(), vertFile.c_str());

		GLuint program = glCreateProgram();

		GLErrorAssert(TAG, "Got error %s from OpenGL when creating program for fragment shader %s and vertex shader %s.", fragFile.c_str(), vertFile.c_str());

		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		GLErrorAssert(TAG, "Got error %s from OpenGL when attaching fragment shader %s and vertex shader %s to program.", fragFile.c_str(), vertFile.c_str());

		glLinkProgram(program);

		GLErrorAssert(TAG, "Got error %s from OpenGL when linking fragment shader %s and vertex shader %s.", fragFile.c_str(), vertFile.c_str());

		GLint result;
		glGetProgramiv(program, GL_LINK_STATUS, &result);
		if (result == GL_FALSE) {
			//Get the length of the error log
			GLint logLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

			//Get and print the error log and then exit with error
			char *infoLog = (char *) malloc(logLength);
			glGetProgramInfoLog(program, logLength, NULL, infoLog);
			gdt_fatal(TAG, "Error linking program: %s", infoLog);
		}

		free(vertText);
		free(fragText);

		return program;
}

void Shader::reloadShader() {
	mProgramID = createProgram(mVertexShaderPath, mFragmentShaderPath);
}

Shader * Shader::load(std::string vertFile, std::string fragFile){
	if (sLoadedShaders.count(vertFile+fragFile)>0){
		gdt_log(LOG_NORMAL, TAG, "VertShader FragShader already loaded \"%s\" and \"%s\". Returning copy.", vertFile.c_str(),fragFile.c_str());
		return sLoadedShaders[vertFile+fragFile];
		//gdt_fatal(TAG, "Shader already loaded!: %s, %s", &vertFile[0],&fragFile[0]);
	}



	GLuint program = createProgram(vertFile, fragFile);

	//gdt_log(LOG_NORMAL, TAG, "test: %s\n%s",&text.vertData[0],&text.fragData[0]);

	Shader *newShader = new Shader(program);
	newShader->mVertexShaderPath = vertFile;
	newShader->mFragmentShaderPath = fragFile;
	sLoadedShaders[vertFile+fragFile] = newShader;

	return sLoadedShaders[vertFile+fragFile];
}

Shader* Shader::get(std::string vertFile,std::string fragFile){
	if (sLoadedShaders.count(vertFile+fragFile)==0)
		gdt_fatal(TAG, "Getting shader that not is loaded! VertShader: %s , FragShader: %s", vertFile.c_str(),fragFile.c_str());
	return sLoadedShaders[vertFile+fragFile];
}

void Shader::reload() {
	std::map<std::string, Shader*>::iterator it;
	for(it = sLoadedShaders.begin(); it != sLoadedShaders.end(); it++) {
		it->second->reloadShader();
	}
}

Shader::Shader() : mProgramID(-1) {}

Shader::Shader(GLint id) : mProgramID(id) {}

void Shader::use(){
	glUseProgram(mProgramID);
}

GLint Shader::getAttriLoc(string_t attri){
	return glGetAttribLocation(mProgramID, attri);
}

bool Shader::setUniformMat4f(string_t unifo,glm::mat4 mat){
	GLint unifoLoc=glGetUniformLocation(mProgramID, unifo);
	if (unifoLoc==-1){
		return false;
	}
	glUniformMatrix4fv(unifoLoc,1,false,glm::value_ptr(mat));
	return true;
}

bool Shader::setAttribute4f(string_t attri,float f1,float f2,float f3,float f4){
	GLint attriLoc=glGetAttribLocation(mProgramID, attri);
	if(attriLoc==-1){
		return false;
	}
	glVertexAttrib4f(attriLoc,f1,f2,f3,f4);
	return true;
}

bool Shader::setAttribute3f(string_t attri,float f1,float f2,float f3){
	GLint attriLoc=glGetAttribLocation(mProgramID, attri);
	if(attriLoc==-1){
		return false;
	}
	glVertexAttrib3f(attriLoc,f1,f2,f3);
	return true;
}

bool Shader::setAttribute2f(string_t attri,float f1,float f2){
	GLint attriLoc=glGetAttribLocation(mProgramID, attri);
	if(attriLoc==-1){
		return false;
	}
	glVertexAttrib2f(attriLoc,f1,f2);
	return true;
}

bool Shader::setAttribute1f(string_t attri,float f1){
	GLint attriLoc=glGetAttribLocation(mProgramID, attri);
	if(attriLoc==-1){
		return false;
	}
	glVertexAttrib1f(attriLoc,f1);
	return true;
}

bool Shader::setAttribute4f(string_t attri,float* f){
	GLint attriLoc=glGetAttribLocation(mProgramID, attri);
	if(attriLoc==-1){
		return false;
	}
	glVertexAttrib4f(attriLoc,f[0],f[1],f[2],f[3]);
	return true;
}

bool Shader::setAttribute3f(string_t attri,float* f){
	GLint attriLoc=glGetAttribLocation(mProgramID, attri);
	if(attriLoc==-1){
		return false;
	}
	glVertexAttrib3f(attriLoc,f[0],f[1],f[2]);
	return true;
}

bool Shader::setAttribute2f(string_t attri,float* f){
	GLint attriLoc=glGetAttribLocation(mProgramID, attri);
	if(attriLoc==-1){
		return false;
	}
	glVertexAttrib2f(attriLoc,f[0],f[1]);
	return true;
}

bool Shader::setAttribute1f(string_t attri,float* f){
	GLint attriLoc=glGetAttribLocation(mProgramID, attri);
	if(attriLoc==-1){
		return false;
	}
	glVertexAttrib1f(attriLoc,f[0]);
	return true;
}

bool Shader::setUniform4f(string_t unifo,float f1,float f2,float f3,float f4){
	GLint unifoLoc=glGetUniformLocation(mProgramID, unifo);
	if(unifoLoc==-1){
		return false;
	}
	glUniform4f(unifoLoc,f1,f2,f3,f4);
	return true;
}

bool Shader::setUniform3f(string_t unifo,float f1,float f2,float f3){
	GLint unifoLoc=glGetUniformLocation(mProgramID, unifo);
	if(unifoLoc==-1){
		return false;
	}
	glUniform3f(unifoLoc,f1,f2,f3);
	return true;
}

bool Shader::setUniform2f(string_t unifo,float f1,float f2){
	GLint unifoLoc=glGetUniformLocation(mProgramID, unifo);
	if(unifoLoc==-1){
		return false;
	}
	glUniform2f(unifoLoc,f1,f2);
	return true;
}

bool Shader::setUniform1f(string_t unifo,float f1){
	GLint unifoLoc=glGetUniformLocation(mProgramID, unifo);
	if(unifoLoc==-1){
		return false;
	}
	glUniform1f(unifoLoc,f1);
	return true;
}

bool Shader::setUniform4f(string_t unifo,float *f){
	GLint unifoLoc=glGetUniformLocation(mProgramID, unifo);
	if(unifoLoc==-1){
		return false;
	}
	glUniform4f(unifoLoc,f[1],f[2],f[3],f[4]);
	return true;
}

bool Shader::setUniform3f(string_t unifo,float *f){
	GLint unifoLoc=glGetUniformLocation(mProgramID, unifo);
	if(unifoLoc==-1){
		return false;
	}
	glUniform3f(unifoLoc,f[1],f[2],f[3]);
	return true;
}

bool Shader::setUniform2f(string_t unifo,float *f){
	GLint unifoLoc=glGetUniformLocation(mProgramID, unifo);
	if(unifoLoc==-1){
		return false;
	}
	glUniform2f(unifoLoc,f[1],f[2]);
	return true;
}

bool Shader::setUniform1f(string_t unifo,float *f){
	GLint unifoLoc=glGetUniformLocation(mProgramID, unifo);
	if(unifoLoc==-1){
		return false;
	}
	glUniform1f(unifoLoc,f[1]);
	return true;
}

bool Shader::setUniform1i(string_t unifo,int i){
	GLint unifoLoc=glGetUniformLocation(mProgramID, unifo);
	if(unifoLoc==-1){
		return false;
	}
	glUniform1i(unifoLoc,i);
	return true;
}
