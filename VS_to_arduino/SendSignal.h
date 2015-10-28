#include "pxcsensemanager.h"
#include "pxcemotion.h"

#include <GLFW\glfw3.h>
#pragma comment(lib, "opengl32.lib")

#include <vector>

#include <chrono>
#include <thread>

class SendSignal
{
public:
	void setup();
	void AnimationPlayer();
	void update();
	void onMouse(GLFWwindow*, int, int, int);
	void testUpdate();
	void cleanup();

	void playSequence(std::vector<std::pair<std::string, int>>);
	void playOneShot(const std::string, int);

	bool iSeeYou;

private:
	PXCSenseManager *mSenseMgr = nullptr;
	PXCFaceModule * faceModule = nullptr;
	PXCFaceConfiguration *facec = nullptr;
	PXCFaceData *fdata = nullptr;
};