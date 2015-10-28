#define STB_IMAGE_IMPLEMENTATION

#include "SendSignal.h"
#include "stb_image.h"




using namespace std;
int width, height;
stbi_uc * myImage = stbi_load("textures/neutral/eyeOpen.jpg", &width, &height, nullptr, 3);



bool red_background = false;


float milliSec = 0;
int blinkVal = 8;


void SendSignal::updateTimer()
{
	milliSec += 0.01;
	if (milliSec > blinkVal)
	{
		cout << "blink!" << endl;
		cout << blinkVal << endl;
		playSequence(BlinkAnim);
		
	}
}

void on_mouse(GLFWwindow * win, int button, int action, int mods)
{
	//auto sig = (SendSignal*)glfwGetWindowUserPointer(win);
	//sig->playSequence(...);

	if (button == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS)
	{
		red_background = !red_background;
	}
}

void SendSignal::drawFrame(int windowWidth, int windowHeight)
{
	//Draw color image
	glRasterPos2i((width - 800) / 2, (height - 480) / 2);
	glPixelZoom(1, -1);
	glDrawPixels(800, 480, GL_RGB, GL_UNSIGNED_BYTE, myImage);
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

	blinkVal = 4 + (std::rand() % 10);
	milliSec = 0;
}
