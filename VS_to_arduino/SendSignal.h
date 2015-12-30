#include "pxcsensemanager.h"
#include "pxcemotion.h"

#include <GLFW\glfw3.h>
#pragma comment(lib, "opengl32.lib")

#include <vector>
#include <string>
//#include <map>

#include <chrono>
#include <thread>

#include <iostream>
#include <queue>
#include <memory>
#include <string>




class SendSignal
{
public:
	SendSignal();

	void setup();
	void updateTimer();
	void resetTimer();
	void drawFrame(int windowWidth, int windowHeight);
	void update();
	void testUpdate();
	void cleanup();

	void playSequence(std::vector<std::pair<std::string, int>>);
	void playOneShot(const std::string, int);

	
	bool neutral = true;
	bool iSeeYou;
	bool projectOpen = true;
	GLFWwindow * win;

	std::vector<std::pair<std::string, int>> BlinkAnim;

private:
	PXCSenseManager *mSenseMgr = nullptr;
	PXCFaceModule * faceModule = nullptr;
	PXCFaceConfiguration *facec = nullptr;
	PXCFaceData *fdata = nullptr;
	//GLFWwindow * win = glfwCreateWindow(800, 480, "Robot Face", nullptr, nullptr);
};