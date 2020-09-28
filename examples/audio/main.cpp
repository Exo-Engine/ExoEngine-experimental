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

using namespace	ExoEngine;
using namespace	ExoRenderer;
using namespace ExoAudio;

int	main(void)
{
	Engine		engine("resources/settings.xml");
	IAudio*		audio = engine.getAudio();
	IRenderer*	renderer = engine.getRenderer();
	IWindow*	window;
	IKeyboard*	keyboard;
	bool		run = true;

	renderer->initialize("example sound - Press 'space' to stop music & key 's' for playing a sound", 1280, 720, WindowMode::WINDOWED, false);
	audio->initialize();

	engine.getResourceManager()->load("resources/resources.xml");
	window = renderer->getWindow();
	keyboard = renderer->getKeyboard();

	ISource* source1 = audio->createSource();
	ISource* source2 = audio->createSource();

	auto music = audio->createMusic("resources/music.ogg"); // For the moment, no implementation with the ResourceManager #TODO
	auto sound = engine.getResourceManager()->get<ISound>("sound");

	source1->setAudio(music);
	source2->setAudio(sound.get());

	source1->play();

	while (run)
	{
		if (keyboard->isKeyDown(KeyboardKeys::KEY_ESCAPE))
			run = false;

		if (keyboard->isKeyDown(KeyboardKeys::KEY_S) && !keyboard->lastIsKeyDown(KeyboardKeys::KEY_S))
			source2->play();

		if (keyboard->isKeyDown(KeyboardKeys::KEY_SPACE) && !keyboard->lastIsKeyDown(KeyboardKeys::KEY_SPACE))
		{
			if (source1->getState() == source1->PLAYING)
				source1->stop();
			else
				source1->play();
		}

		if (window->getIsClosing())
			run = false;

		source1->streamingUpdate();
		renderer->swap();
	}

	delete source1;
	delete source2;

	delete music;

	return (EXIT_SUCCESS);
}
