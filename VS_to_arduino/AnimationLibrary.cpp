#define STB_IMAGE_IMPLEMENTATION

#include "SendSignal.h"
#include "stb_image.h"
#include <string>

#include <map>





using namespace std;
int width, height;
stbi_uc * myImage = stbi_load("textures/neutral/eyeOpen.jpg", &width, &height, nullptr, 3);

bool neutral = true;
vector<pair<string, int>> BlinkAnim { { "textures/neutral/eyeOpen.jpg", 0 }, { "textures/wink/wink_03.jpg", 2 }, { "textures/wink/wink_04.jpg", 2 }, { "textures/wink/wink_05.jpg", 2 }, { "textures/wink/wink_06.jpg", 2 }, { "textures/wink/wink_05.jpg", 2 }, { "textures/wink/wink_04.jpg", 2 }, { "textures/wink/wink_03.jpg", 2 }, { "textures/wink/wink_02.jpg", 2 }, { "textures/neutral/eyeOpen.jpg" ,2} };

bool red_background = false;

void on_mouse(GLFWwindow * win, int button, int action, int mods)
{
	//auto sig = (SendSignal*)glfwGetWindowUserPointer(win);
	//sig->playSequence(...);

	if (button == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS)
	{
		red_background = !red_background;
	}
}

void SendSignal::AnimationPlayer()
{
	glfwInit();
	GLFWwindow * win = glfwCreateWindow(800, 480, "Robot Face", nullptr, nullptr);

	//glfwSetWindowUserPointer(win, this); 

	glfwSetMouseButtonCallback(win, on_mouse);

	while (!glfwWindowShouldClose(win))
	{
		//Input
		glfwPollEvents();
		
		//Get current size of window's framebuffer
		//int width, height;
		glfwGetFramebufferSize(win, &width, &height);
		
		//Rendering
		glfwMakeContextCurrent(win);
		glViewport(0, 0, width, height);

		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		//Coordinate System
		glLoadIdentity();
		glOrtho(0, width, height, 0, -1, +1);
		
		//Draw color image
		glRasterPos2i((width - 800) / 2, (height - 480) / 2);
		glPixelZoom(1, -1);
		glDrawPixels(800, 480, GL_RGB, GL_UNSIGNED_BYTE, myImage);

		glfwSwapBuffers(win);
	}
	glfwTerminate();

	cleanup();
}

void SendSignal::playOneShot(const std::string s, int timeTillLoad)
{ 
	//int width, height;
	std::this_thread::sleep_for(std::chrono::milliseconds(17 * timeTillLoad));
	myImage = stbi_load( s.c_str() , &width, &height, nullptr, 3);
}

void SendSignal::playSequence(vector<pair<string, int>> xxAnim)
{
	neutral = false;

	/*
	for(const auto &anim : xxAnim)
	{
		auto f = anim.first;
		auto t = anim.second;
	}
	*/
	for (int i = 0; i < xxAnim.size(); i++)
	{
		auto filename = xxAnim[i].first;
		auto timeInMillis = xxAnim[i].second;

		playOneShot(filename, timeInMillis);
	}
	
	//int width, height;
	myImage = stbi_load("textures/neutral/eyeOpen.jpg", &width, &height, nullptr, 3);
	neutral = true;
}
