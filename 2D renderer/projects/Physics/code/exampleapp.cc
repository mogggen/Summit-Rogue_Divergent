//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2020 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "imgui.h"
//#include "stb_image.h"
#include "exampleapp.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <chrono>
#ifdef __linux__
#include <unistd.h>
#endif
struct Actor;

using namespace Display;
namespace Example
{
	//------------------------------------------------------------------------------
	/**
	 */
	ExampleApp::ExampleApp()
	{
		// empty
	}

	//------------------------------------------------------------------------------
	/**
	 */
	ExampleApp::~ExampleApp()
	{
		// empty
	}

	//------------------------------------------------------------------------------
	/**
	 */

	void Print(M4 m)
	{
		for (size_t i = 0; i < 4; i++)
		{
			V4 v = m[i];
			std::cout << '(';
			for (char i = 0; i < 4; i++)
				std::cout << v.data[i] << (i == 3 ? ")\n" : ", ");
		}
	}

	bool
	ExampleApp::Open()
	{
		App::Open();
		this->window = new Display::Window;

		// assign ExampleApp variables
		w = a = s = d = q = e = false;
		window->GetSize(width, height);

		window->SetKeyPressFunction([this](int32 keycode, int32 scancode, int32 action, int32 mods)
									{
			//deltatime
			switch (keycode)
			{
			case GLFW_KEY_ESCAPE: window->Close(); break;
			case GLFW_KEY_W: w = action; break;
			case GLFW_KEY_S: s = action; break;
			case GLFW_KEY_A: a = action; break;
			case GLFW_KEY_D: d = action; break;

			case GLFW_KEY_Q: q = action; break;
			case GLFW_KEY_E: e = action; break;
			} });

		window->SetMousePressFunction([this](int32 button, int32 action, int32 mods)
									  { isPressed = button == GLFW_MOUSE_BUTTON_1 && action; });

		window->SetMouseMoveFunction([this](float64 x, float64 y)
									 {
			if (isPressed)
			{
				senseX = prevX + (0.002 * (x - width / 2));
				senseY = prevY + (0.002 * (y - height / 2));
				cubeProjectionViewTransform = Rotation(V4(1, 0, 0), senseY) * Rotation(V4(0, 1, 0), senseX);
				fireHydrantProjectionViewTransform = Rotation(V4(1, 0, 0), senseY) * Rotation(V4(0, 1, 0), senseX);
			} });

		if (this->window->Open())
		{
			// set clear color to gray
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

			// MeshResource
			fireHydrantMesh = MeshResource::LoadObj("textures/fireHydrant.obj");

			// TextureResource
			fireHydrantTexture = std::make_shared<TextureResource>("textures/cubepic.png");

			// shaderResource
			fireHydrantScript = std::make_shared<ShaderResource>();
			fireHydrantScript->getShaderResource(this->vertexShader, this->pixelShader, this->program);

			// Actor
			Actor *fireHydrantActor = new Actor();

			// GraphicNode
			fireHydrant = std::make_shared<GraphicNode>(fireHydrantMesh, fireHydrantTexture, fireHydrantScript, fireHydrantActor);

			// MeshResource
			cubeMesh = MeshResource::LoadObj("textures/cube.obj");

			cubeTexture = std::make_shared<TextureResource>("textures/red.png");

			// shaderResource
			cubeScript = std::make_shared<ShaderResource>();
			cubeScript->getShaderResource(this->vertexShader, this->pixelShader, this->program, "textures/vs.glsl", "textures/psNoTexture.glsl");
			// note: bindTexture() still requires a texture, but just won't use it

			// Actor
			Actor temp2;
			Actor *cubeActor = &temp2;

			// GraphicNode
			cube = std::make_shared<GraphicNode>(cubeMesh, cubeTexture, cubeScript, cubeActor);

			// MeshResource
			quadMesh = MeshResource::LoadObj("textures/quad.obj");

			// Actor
			Actor temp3;
			Actor *quadActor = &temp3;

			// GraphicNode
			quad = std::make_shared<GraphicNode>(quadMesh, cubeTexture, cubeScript, quadActor);

			this->window->SetUiRender([this]()
									  { this->RenderUI(); });
			return true;
		}
		return false;
	}

	inline bool sphereTrigger(const V2& a, const V2& b, const float& radius)
	{
		if (radius <= 0)
		{
			printf("sphereTrigger: radius is invalid.\n");
		}
		return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)) < radius;
	}

	inline bool squreTrigger(const V2& lhsPos, const V2& lhsSize, const V2& rhsPos, const V2& rhsSize)
	{
		if (lhsSize.x <= 0 &&
			lhsSize.y <= 0 &&
			rhsSize.x <= 0 &&
			rhsSize.y <= 0)
		{
			printf("squareTrigger: size is invalid.\n");
			exit(1);
		}
		return 
			lhsPos.x + lhsSize.x >= rhsPos.x - rhsSize.x ||
			lhsPos.x - lhsSize.x < rhsPos.x + rhsSize.x ||
			lhsPos.y + lhsSize.y >= rhsPos.y - rhsSize.y ||
			lhsPos.y - lhsSize.y < rhsPos.y + rhsSize.y;
	}

	void decceleratingAnimation(V2& current, const V2& target, const float& delta=0.01f)
	{
		if (sphereTrigger(current, target, delta * 100.f))
		{
			current = target;
			return;
		}
		current += (target - current) * delta;
	}

	// check conditions that depend on this value
	inline void getEffectedConditions()
	{
		// loop the globals checking if there are conditions
		// checking the conditions if there were any
	}

	//------------------------------------------------------------------------------
	/**
	 */

	void
	ExampleApp::Run()
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		Camera cam(90, (float)width / height, 0.01f, 100.0f);
		cam.setPos(V4(0, 0, 30));
		cam.setRot(V4(0, 1, 0), M_PI);

		Lightning light(V3(10, 10, 10), V3(1, 1, 1), .01f);
		float camSpeed = .08f;

		// set identies
		fireHydrantWorldSpaceTransform = fireHydrantProjectionViewTransform = Translate(V4());
		
		cubeWorldSpaceTransform = Translate(V4(0, 0, 0));
		M4 cubeWorldSpaceTransform2 = Translate(V4(0, 0, 0));
		cubeProjectionViewTransform = Translate(V4());
		M4 cubeProjectionViewTransform2 = Translate(V4());

		while (this->window->IsOpen())
		{
			auto start = std::chrono::high_resolution_clock::now();

			cam.setPos(cam.getPos() + Normalize(V4(d - a, s - w, 0)) * -camSpeed);

			cubeWorldSpaceTransform = cubeWorldSpaceTransform *
									  Translate(Normalize(V4(a - d, w - s, 0)) * -camSpeed);

			// cube view space
			cubeProjectionViewTransform = cam.pv() * cubeWorldSpaceTransform;
			cubeProjectionViewTransform2 = cam.pv() * cubeWorldSpaceTransform2;


			// equation
			double mouseWorldX, mouseWorldY;

			if (isPressed)
			{
				glfwGetCursorPos(this->window->GetHandle(), &mouseWorldX, &mouseWorldY);
				
				std::cout << "x:" << mouseWorldX << " y:" << mouseWorldY << std::endl;
			}

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// fireHydrantScript->setM4(cam.pv(), "m4ProjViewPos");
			cubeScript->setM4(cam.pv(), "m4ProjViewPos");

			//light.bindLight(fireHydrantScript, cam.getPos());
			//fireHydrant->DrawScene(fireHydrantProjectionViewTransform, fireHydrantColor);

			light.bindLight(cubeScript, cam.getPos());
			cube->DrawScene(cubeProjectionViewTransform, cubeColor);
			cube->DrawScene(cubeProjectionViewTransform2, cubeColor);

			this->window->Update();
			frameIndex++;

			this->window->SwapBuffers();
			auto finish = std::chrono::system_clock::now();
		}
	}

	// introduction
	// the society of these NPCs are a mess. everyone is always blaming everyone else for not doing their job right
	// and terrorism, murder, stealing will take down even the best of the best.
	// how long will you manage to dodge the crossfire
	//
	// a day cycle: you wake up, you have a certain time that you work where you will have to finish all you assigned work (work units) on time, failing this will get you fired
	// you can apply for a new place of work at any of the areas;
	// anyone unable to find work within a 'long cycle'
	// will either become slaves or enroll in the millitary
	// to fight of witches, ogres, wizards, etc.
	// 
	
	// cycles
	// if you haven't reached home by the time you will sleep in place
	// íf you manage to reach your house however you will dream of 4 diffrent events
	// these 4 events are the 4 most likely plans to be executed in the upcoming cycle
	//


	// you will posses someone new every cycle, live their life for a day then posses someone new,
	// you will meet other people as the one you are possesing, so try to learn everything you can about them
	// 
	// there could be multiple gamemodes (diffrent gamemodes for diffrent days)
	// 
	// 
	// there could be minigames in the tavern like the 8 buffer brainfvck game
	// if the person you're possesing kills someone, you swap soul with a random NPC and that NPC knows that you did it, but noone else
	// NPCs act in a response to others reaction to your actions.
	// ranks amoungs individuals
	// 
	// factory: the local were everyone will perform their assigned work (each work would take diffrent time depending on the talent of the worker whihch could be improved or worsen over time, the union will try and balance this)
	// union: distributes work amongst NPCs
	// bank: distribute salaries in the form of everyday items, according to how to
	// the white house: every one votes for what each item should be pegged to and what value (mathamatical pain)
	// graveyard: where most individuals are taken after passing
	//
	// // kill a certain person
	// // vandilize area
	// // terrorize area
	// // make ofspring with someone
	// // boycot work
	// every person has 5 plans,
	// plans can have up to 4 conditions that have to meet before the NPC attempts to perform the plan,
	// conditions can range from:
	// if this person dies
	// if these two people have ofspring
	// if these two people are married
	// if this person is cheated on
	// if a location is terrorized
	// if a location is vandalized
	// if a certain value (skills, pegged item) of a certain stat is met
	// if a certain value of a certain stat is not met within a number of cycles
	// 
	// (following through on your possesed NPCs plan is optional)
	// you can only ask them about one of them each day (they might change, they might lie to you)
	// everyone holds one story which is a chain of messages
	//
	// skills
	// work morale
	// sanity
	// wealth
	// health
	// loving
	// social
	// (is only used for conditionals in a completly arbitrary facion)
	//
	// pegged item
	// example: 1 shovel = 3.55 loafs of bread
	//

	// red thread
	//
	// each worker get's assigned a need for items, each day completely random, they have to meet this qouta or they will die 
	// every long cycle that completes a new item will be introduce, the name and initial value is pegged and is just there to throw of the player (you),
	// the item is completely modular, can be produced in the factory at a new station, and has no actual effect or utility other than being a inconvenience
	// 
	// union will try to figure out how much of each items needs to be produced based on the pegged values of each item, voted forward by the NPCs, calulated as a average, to meet the expectation of giving everyone a equal salary,
	// given out to all the workes to make sure they are happy, which will be harder and harder the more items their are and society will colapse
	// SUPER MUCH WORK, NOT REALLY NECCESSERY: NPCs may choose to favor their own needs and produce work units that directly favor their income
	// 


	void ExampleApp::RenderUI()
	{
		bool show = true;
		ImGui::Begin("Mega Cringe", &show, ImGuiWindowFlags_NoSavedSettings);
		
		ImGui::Text("Name: %s", "Hello");
		ImGui::Text("Occupation: %s", "factory");

		ImGui::Text("morale: %d", 0);
		ImGui::Text("sanity: %d", 0);
		ImGui::Text("wealth: %d", 0);
		ImGui::Text("health: %d", 0);
		ImGui::Text("loving: %d", 0);
		ImGui::Text("");
		ImGui::Text("year:   %d", 0);
		ImGui::Text("--time stats--");
		ImGui::Text("Total: %d", 0);
		ImGui::Text("Active: %d", 0);
		//ImGui::Text("frames: %d %.0f", frameIndex, (float)1e6 / duration);
		ImGui::End();
	}

} // namespace Example