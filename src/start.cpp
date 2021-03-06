/*
 * start.cpp
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

#include <gdt/gdt.h>

#include "Game.h"
#include "Matrix4f.h"

Game *game;

void on_touch(touch_type_t what, int screenX, int screenY) {
	game->handleTouch(what,screenX,screenY);
}

void gdt_hook_initialize() {
	gdt_log(LOG_NORMAL, "start", "gdt_hook_initialize");

	gdt_set_callback_touch(&on_touch);
	game = new Game();
	game->init();
}

void gdt_hook_visible(bool newSurface) {
	gdt_log(LOG_NORMAL, "start", "gdt_hook_visible");

	game->visible(newSurface);
}

void gdt_hook_active() {
	gdt_log(LOG_NORMAL, "start", "gdt_hook_active");
}

void gdt_hook_inactive() {
	gdt_log(LOG_NORMAL, "start", "gdt_hook_inactive");
}

void gdt_hook_save_state() {
	gdt_log(LOG_NORMAL, "start", "gdt_hook_save_state");
}

void gdt_hook_hidden() {
	gdt_log(LOG_NORMAL, "start", "gdt_hook_hidden");
}

void gdt_hook_render() {
	//gdt_log(LOG_NORMAL, "start", "gdt_hook_render");

	game->render();
}
