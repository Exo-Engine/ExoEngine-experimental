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

#pragma once

#include "Scene.h"
#include "Dynamic.h"
#include "IWidget.h"

#include <libxml/parser.h>

namespace	ExoEngine
{
	class	ResourceManager;
	class	Page : public Scene
	{
		public:
			Page(SceneStateMachine &stateMachine, const std::string& name, const std::string& file);
			~Page(void);

			virtual void	load(void);
			virtual void	unload(void);

			virtual void	draw(void);
			virtual void	handleEvents(void);
		private:

			typedef ExoRenderer::IWidget*	(Page::*t_widget_loader)(const std::map<std::string, std::string>&);

			ExoRenderer::IWidget*	loadImage(const std::map<std::string, std::string>& properties);
			ExoRenderer::IWidget*	loadButton(const std::map<std::string, std::string>& properties);
			ExoRenderer::IWidget*	loadCheckbox(const std::map<std::string, std::string>& properties);
			ExoRenderer::IWidget*	loadInput(const std::map<std::string, std::string>& properties);
			ExoRenderer::IWidget*	loadSelect(const std::map<std::string, std::string>& properties);
			ExoRenderer::IWidget*	loadSlider(const std::map<std::string, std::string>& properties);
			ExoRenderer::IWidget*	loadSpinner(const std::map<std::string, std::string>& properties);
			ExoRenderer::IWidget*	loadView(const std::map<std::string, std::string>& properties);

			void					load(xmlNodePtr node, ExoRenderer::IWidget* parentWidget);
			void					getProperties(xmlNodePtr node, std::map<std::string, std::string>& properties);

			std::map<std::string, ExoRenderer::IWidget*>	_widgets;
			ResourceManager*								_resourceManager;
	};
}
