#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "pxcsensemanager.h"
#include "pxcemotion.h"

#include "Midi.h"

#include "fmod.hpp"
#include "common.h"

#include <GLFW\glfw3.h>
#pragma comment(lib, "opengl32.lib")

#include <vector>
#include <string>

#include <chrono>
#include <thread>
#include <iostream>
#include <queue>
#include <memory>
#include <string>

using namespace std;

int width, height;
float millisec;
int blinkVal = 8;
bool neutral = true;
stbi_uc *myImage = stbi_load("../textures/neutral/eyeOpen.jpg", &width, &height, nullptr, 3);

vector<pair<string, int>> BlinkAnim = {
	{ "../textures/neutral/eyeOpen.jpg", 0 },
	{ "../textures/wink/wink_03.jpg", 2 },
	{ "../textures/wink/wink_04.jpg", 2 },
	{ "../textures/wink/wink_05.jpg", 2 },
	{ "../textures/wink/wink_06.jpg", 2 },
	{ "../textures/wink/wink_05.jpg", 2 },
	{ "../textures/wink/wink_04.jpg", 2 },
	{ "../textures/wink/wink_03.jpg", 2 },
	{ "../textures/wink/wink_02.jpg", 2 },
	{ "../textures/neutral/eyeOpen.jpg", 2 }
};

class SendSignal	
{
	vector<pair<string, int>> anim;
	int anim_image_index;
	int anim_frame_count;

	

public: 
	//void setup();
	void timerUpdate();
	void drawFrame(int, int);
	//void playOneShot(const string, int);
	void playSequence(vector<pair<string, int>>);
	//void cleanup();
	
	

private:
	PXCSenseManager *mSenseMgr;
	PXCFaceModule *faceModule;
	PXCFaceConfiguration *facec;
	PXCFaceData *fdata;

};

void AudioPlay(string filename)
{
	FMOD::System *system;
	FMOD::Sound		*sound;
	FMOD::Channel	*channel = 0;
	FMOD_RESULT		result;
	unsigned int	version;
	void			*extradriverdata = 0;

	Common_Init(&extradriverdata);
	FMOD::System_Create(&system);
	system->init(32, FMOD_INIT_NORMAL, extradriverdata);
	result = system->createSound(Common_MediaPath(filename.c_str()), FMOD_DEFAULT, 0, &sound);

	system->playSound(sound, 0, false, &channel);
}

/*void SendSignal::setup(){
//setup RealSense
cout << "initialized" << endl;
}*/

int main()
{
	SendSignal mySignal;
	//mySignal.setup();

	glfwInit();
	GLFWwindow *win = glfwCreateWindow(800, 480, "Robot Face", nullptr, nullptr);

	while (!glfwWindowShouldClose(win))
	{
		glfwPollEvents();
		mySignal.timerUpdate();
		// update camera information here

		// All Output goes Here- rendering, commands to arduino
		glfwGetFramebufferSize(win, &width, &height);
		glfwMakeContextCurrent(win);

		glViewport(0, 0, width, height);
		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glLoadIdentity();
		glOrtho(0, width, height, 0, -1, +1);
		
		mySignal.drawFrame(width, height);
		

		glfwSwapBuffers(win);
	}

	glfwTerminate();
	//cleanup
}


void SendSignal::timerUpdate()
{
	//This is our Blink release
	(float)millisec += (float)0.01;
	if (millisec > blinkVal)
	{
		AudioPlay("blink.wav");
		cout << "blink!" << endl;
		cout << blinkVal << endl;
		playSequence(BlinkAnim);

		blinkVal = 6 + (rand() % 10);
		millisec = 0;
	}
}

void SendSignal::drawFrame(int windowWidth, int windowHeight)
{
	// If there is an animation and we have waited the appropriate number of frames
	if (anim_image_index < anim.size() && anim_frame_count == anim[anim_image_index].second)
	{
		// Load the next image from the animation
		myImage = stbi_load(anim[anim_image_index].first.c_str(), &width, &height, nullptr, 3);

		++anim_image_index;
		anim_frame_count = 0;

		// If we have reached the end of the animation
		if (anim_image_index == anim.size())
		{
			neutral = true;
		}
	}

	glRasterPos2i((width - 800) / 2, (height - 480) / 2);
	glPixelZoom(1, -1);
	glDrawPixels(800, 480, GL_RGB, GL_UNSIGNED_BYTE, myImage);

	++anim_frame_count;

}

void SendSignal::playSequence(vector<pair<string, int>> xxAnim)
{
	anim = xxAnim;
	anim_image_index = 0;
	anim_frame_count = 0;
	neutral = false;
}

/*void SendSignal::cleanup(){
	//Release Camera Data
	//Release FMOD
}*/

// These lines, ugly though they are, will ensure we have the right lib statements
#ifdef _DEBUG
#pragma comment(lib, "libpxc_d.lib")

#ifdef _WIN64
#pragma comment(lib, "fmodL64_vc.lib")
#else
#pragma comment(lib, "fmodL_vc.lib")
#endif

#else
#pragma comment(lib, "libpxc.lib")

#ifdef _WIN64
#pragma comment(lib, "fmod64_vc.lib")
#else
#pragma comment(lib, "fmod_vc.lib")
#endif

#endif