#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015-2020 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/Vertex.h"
#include "render/MeshResource.h"
#include "render/Camera.h"
#include "render/Lightning.h"
#include "render/ShaderResource.h"
#include "render/GraphicNode.h"
#include "render/window.h"

namespace Example
{
	class ExampleApp : public Core::App
	{
	public:
		/// constructor
		ExampleApp();
		/// destructor
		~ExampleApp();

		/// open app
		bool Open();
		/// run app
		void Run();
	private:
		int width, height;
		float64 prevX = 0, prevY = 0;
		float64 senseX = 0, senseY = 0;
		bool w, a, s, d,
			q, e, isRotate = false;
		M4 Em;
		M4 Evp;
		GLuint program;
		GLuint vertexShader;
		GLuint pixelShader;

		std::shared_ptr<MeshResource> cube;
		std::shared_ptr<ShaderResource> shaderResource;
		std::shared_ptr<GraphicNode> node;

		std::shared_ptr<MeshResource> monkey;
		std::shared_ptr<ShaderResource> shaderResourceSusanne;
		std::shared_ptr<GraphicNode> susanne;

		Display::Window* window;
	};
} // namespace Example