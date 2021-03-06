/*
 *	MIT License
 *
 *	Copyright (c) 2020 Gaëtan Dezeiraud and Ribault Paul
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *	copies of the Software, and to permit persons to whom the Software is
 *	furnished to do so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in all
 *	copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *	SOFTWARE.
 */

#include "Engine.h"
#include <UI/Image.h>
#include <UI/Cursor.h>

using namespace	ExoEngine;

int	main(void)
{
	Engine		engine("resources/settings.xml");
	IRenderer*	renderer = engine.getRenderer();
	IWindow*	window;
	Mouse*      mouse;
	Keyboard*	keyboard;
	bool		run = true;

	renderer->initialize("example window", 1280, 720, WindowMode::WINDOWED, false);
	engine.getResourceManager()->load("resources/resources.xml");
	window = renderer->getWindow();
	mouse = renderer->getMouse();
	keyboard = renderer->getKeyboard();
	window->setVsync(true);

	auto cam = renderer->createCamera();
	cam->setPosition(0.0f, 0.0f, 20.0f);

	renderer->setCurrentCamera(cam);

	// Cursor
	ICursor* cursor = new Cursor();
	std::shared_ptr<ITexture> cursorTexture = engine.getResourceManager()->get<ITexture>("cursor");
	cursor->setCursorTexture(cursorTexture);
	renderer->setCursor(cursor);

	// Red block
	sprite img;
	img.texture = engine.getResourceManager()->get<IArrayTexture>("red_block");
	img.scale = glm::vec2(3.6);
	renderer->add(img);

	// Label
	Label* label = new Label();
	label->setFont(engine.getResourceManager()->get<Font>("global_font"));
	label->setText("Hello World!");
	label->setLocalAnchor(AnchorPoint::TOP_LEFT);
	label->setFontScale(0.3f);
	renderer->add(label);

	while (run)
	{
		if (keyboard->isKeyDown(KeyboardKeys::KEY_ESCAPE))
			run = false;

		if (window->getIsClosing())
			run = false;

		cam->update(mouse, keyboard);
		renderer->draw();
		renderer->swap();
	}

	return (EXIT_SUCCESS);
}
