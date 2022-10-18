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
#include "render/Debug.h"
#include "core/Physics.h"
#include "render/Actor.h"

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
		void RenderUI();
		uint64_t duration;
		int width, height;
		uint32_t frameIndex = 0;
		Plane *plane;

		float64 prevX = 0, prevY = 0;
		float64 senseX = 0, senseY = 0;
		bool w, a, s, d,
			q, e, isPressed = false;

		GLuint program;
		GLuint vertexShader;
		GLuint pixelShader;

		M4 fireHydrantWorldSpaceTransform;
		M4 fireHydrantProjectionViewTransform;
		V4 fireHydrantColor{1, 1, 1, 1};
		std::shared_ptr<MeshResource> fireHydrantMesh;
		std::shared_ptr<TextureResource> fireHydrantTexture;
		std::shared_ptr<ShaderResource> fireHydrantScript;
		std::shared_ptr<GraphicNode> fireHydrant;

		M4 cubeWorldSpaceTransform;
		M4 cubeProjectionViewTransform;
		V4 cubeColor{1, 0, 0, 1};

		std::shared_ptr<MeshResource> cubeMesh;
		std::shared_ptr<TextureResource> cubeTexture;
		std::shared_ptr<ShaderResource> cubeScript;
		std::shared_ptr<GraphicNode> cube;

		// will have a entire array of these
		std::shared_ptr<MeshResource> quadMesh;
		std::shared_ptr<TextureResource> quadTexture;
		std::shared_ptr<ShaderResource> quadScript;
		std::shared_ptr<GraphicNode> quad;

		Display::Window *window;
	};
} // namespace Example