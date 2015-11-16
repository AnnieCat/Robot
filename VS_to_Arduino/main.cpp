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
int timeIndex = 0;
bool neutral = true;
bool iSeeYou = false;

//Emotion Triggers
bool suprise = false;

stbi_uc *myImage = stbi_load("../textures/neutral/eyeOpen.jpg", &width, &height, nullptr, 3);

#pragma region AnimationData
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

vector<pair<string, int>> SupriseAnim = {
	{ "../textures/neutral/eyeOpen.jpg", 0 },
	{ "../textures/suprise/suprise_4.jpg", 2 },
	{ "../textures/suprise/suprise_3-5.jpg", 18 },
	{ "../textures/suprise/suprise_3.jpg", 20 },
	{ "../textures/suprise/close_2.jpg", 37 },
	{ "../textures/suprise/close_3.jpg", 41 },
	{ "../textures/suprise/close_4.jpg", 45 },
	{ "../textures/suprise/close_5.jpg", 49 },
	{ "../textures/suprise/close_6.jpg", 53 },
	{ "../textures/suprise/close_7.jpg", 57 },
	{ "../textures/suprise/close_7.jpg", 210 },
	{ "../textures/wink/wink_05.jpg", 212 },
	{ "../textures/wink/wink_04.jpg", 214 },
	{ "../textures/wink/wink_03.jpg", 216 },
	{ "../textures/wink/wink_02.jpg", 218 },
	{ "../textures/neutral/eyeOpen.jpg", 220 },
	{ "../textures/wink/wink_02.jpg", 222 },
	{ "../textures/wink/wink_02.jpg", 224 },
	{ "../textures/wink/wink_04.jpg", 226 },
	{ "../textures/wink/wink_05.jpg", 228 },
	{ "../textures/wink/wink_04.jpg", 230 },
	{ "../textures/wink/wink_02.jpg", 232 },
	{ "../textures/wink/wink_02.jpg", 234 },
	{ "../textures/neutral/eyeOpen.jpg", 236 },
	{ "../textures/neutral/eyeOpen.jpg", 260 },
	{ "../textures/suprise/lookR_02.jpg", 262 },
	{ "../textures/suprise/lookR_03.jpg", 264 },
	{ "../textures/suprise/lookR_04.jpg", 266 },
	{ "../textures/suprise/lookR_05.jpg", 268 },
	{ "../textures/suprise/lookR_05.jpg", 353 },
	{ "../textures/suprise/lookR_04.jpg", 355 },
	{ "../textures/suprise/lookR_03.jpg", 357 },
	{ "../textures/suprise/lookR_02.jpg", 359 },
	{ "../textures/neutral/eyeOpen.jpg", 361 },
	{ "../textures/suprise/lookL_02.jpg", 363 },
	{ "../textures/suprise/lookL_03.jpg", 365 },
	{ "../textures/suprise/lookL_04.jpg", 367 },
	{ "../textures/suprise/lookL_05.jpg", 369 },
	{ "../textures/suprise/lookL_05.jpg", 469 },
	{ "../textures/suprise/lookL_04-5.jpg", 473 },
	{ "../textures/suprise/lookL_04.jpg", 477 },
	{ "../textures/suprise/lookL_03-5.jpg", 481 },
	{ "../textures/suprise/lookL_03.jpg", 484 },
	{ "../textures/suprise/lookL_02-5.jpg", 488 },
	{ "../textures/suprise/lookL_02.jpg", 492 },
	{ "../textures/suprise/lookL_01-5.jpg", 498 },
	{ "../textures/neutral/eyeOpen.jpg", 501 }

};
#pragma endregion

class SendSignal	
{
	vector<pair<string, int>> anim;
	int anim_image_index;
	int anim_frame_count;
	//float frameMultiplier = 2;
	

public: 
	void setup();
	void timerUpdate(float timestep);
	void drawFrame(int, int);
	void loadSequence(vector<pair<string, int>>);
	void playSequence(vector<pair<string, int>>);
	void sendEmotion(string);
	void cleanup();
	PXCSenseManager *mSenseMgr;
	PXCFaceData *fdata;

private:
	PXCFaceModule *faceModule;
	PXCFaceConfiguration *facec;
	

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

int Map(int value, int low1, int high1, int low2, int high2)
{
	return low2 + (value - low1)*(high2 - low2) / (high1 - low1);
}

void SendSignal::setup(){

	mSenseMgr = PXCSenseManager::CreateInstance();

	if (!mSenseMgr)
		cout << "failed to create SDK sense Manager" << endl;

	mSenseMgr->EnableFace();

	faceModule = mSenseMgr->QueryFace();
	facec = faceModule->CreateActiveConfiguration(); 
	PXCFaceConfiguration::ExpressionsConfiguration *expc = facec->QueryExpressions();
	expc->Enable();
	expc->EnableAllExpressions();

	facec->SetTrackingMode(PXCFaceConfiguration::TrackingModeType::FACE_MODE_COLOR);
	facec->ApplyChanges();

	fdata = faceModule->CreateOutput();

	mSenseMgr->EnableEmotion();
	cout << "initialized realSense" << endl;

	mSenseMgr->Init();
}



int main()
{
	SendSignal mySignal;
	mySignal.setup();

	glfwInit();
	GLFWwindow *win = glfwCreateWindow(800, 480, "Robot Face", nullptr, nullptr);

	double t0 = glfwGetTime();

	MidiPlayer player; // Put it outside the loop

#pragma region loadData

	PXCFaceData::ExpressionsData::FaceExpressionResult raiseLeftBrow;
	PXCFaceData::ExpressionsData::FaceExpressionResult raiseRightBrow;
	PXCFaceData::ExpressionsData::FaceExpressionResult openMouth;

	mySignal.loadSequence(BlinkAnim);

#pragma endregion

	while (!glfwWindowShouldClose(win))
	{
		glfwPollEvents();


		const double t1 = glfwGetTime();
		const float timestep = (float)(t1 - t0);
		t0 = t1;

		// Today's lessons:
		// 1) Sometimes if shit is really fucked up, do a build -> clean solution (Typically if you've been messing around with project settings and stuff is refusing to compile)
		// 2) Use glfwGetTime, or std::chrono::high_resolution_clock (Google it if you want to learn more)
		// 3) If you have complex objects to construct, consider pulling them outside a loop. Some objects might take 1/4 of a second to construct, and will kill your time critical loop.
		mySignal.timerUpdate(timestep);
		
		// update camera information here

/*#pragma region Perceptual

		int numFaces = 0;
		
		if (mySignal.mSenseMgr->AcquireFrame(true) >= PXC_STATUS_NO_ERROR)
		{
			PXCEmotion *emotionDet = mySignal.mSenseMgr->QueryEmotion();
			PXCEmotion::EmotionData arrData[10];

			mySignal.fdata->Update();

			int numFaces = mySignal.fdata->QueryNumberOfDetectedFaces();

			for (int i = 0; i < numFaces; ++i)
			{
				PXCFaceData::Face *face = mySignal.fdata->QueryFaceByIndex(i);
				PXCFaceData::ExpressionsData *edata = face->QueryExpressions();

				emotionDet->QueryAllEmotionData(i, &arrData[0]);

#pragma region Face Tracking

				if (arrData->rectangle.x > -1 && arrData->rectangle.y > -1)
				{
					int myX, myY;
					iSeeYou = true;

					myX = Map(arrData->rectangle.x, 500, 0, 0, 80);
					myY = Map(arrData->rectangle.y, 0, 320, 0, 40);
					
				}
				else
					iSeeYou = false;

#pragma endregion

#pragma region Expression Logic

				if (iSeeYou)
				{
#pragma region Query Expression Data
					//Eyebrows
					edata->QueryExpression(PXCFaceData::ExpressionsData::EXPRESSION_BROW_RAISER_LEFT, &raiseLeftBrow);
					edata->QueryExpression(PXCFaceData::ExpressionsData::EXPRESSION_BROW_RAISER_RIGHT, &raiseRightBrow);
					
					//Mouth
					edata->QueryExpression(PXCFaceData::ExpressionsData::EXPRESSION_MOUTH_OPEN, &openMouth);

#pragma endregion

#pragma region Sending Commands
					//suprise
					if (raiseLeftBrow.intensity > 80 && raiseRightBrow.intensity > 80 && openMouth.intensity > 30)
					{
						if (!suprise)
						{
							mySignal.sendEmotion("suprise");
							suprise = true;
						}
					}
					else
						suprise = false;
#pragma endregion
				}
				mySignal.mSenseMgr->ReleaseFrame();
			}

#pragma endregion
		}

#pragma endregion*/

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
	mySignal.cleanup();
}

void SendSignal::sendEmotion(string myEmo)
{
	if (myEmo == "suprise")
	{
		cout << "suprise" << endl;
		//playSequence(SupriseAnim);
	}
}


void SendSignal::timerUpdate(float timestep)
{
	//This is our Blink release
	millisec += timestep;

	if (millisec > blinkVal)
	{
		if (neutral)
		{
			AudioPlay("blink.wav");
			cout << "blink!" << endl;
			cout << blinkVal << endl;
			//playSequence(BlinkAnim);
			playSequence(SupriseAnim);

			blinkVal = 6 + (rand() % 10);
			millisec = 0;
		}
		else
			millisec = 0;
	}


}


void SendSignal::drawFrame(int windowWidth, int windowHeight)
{
	int resetFrameCount;
	
	//cout << "time: " << glfwGetTime() << "               frames: "<< (int)(glfwGetTime() * 60) << endl;

	// If there is an animation and we have waited the appropriate number of frames
	if (anim_image_index < anim.size() && anim_frame_count == (anim[anim_image_index].second))
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

	//anim_frame_count = (int)(glfwGetTime() * 60);
	++anim_frame_count;
	
}

void SendSignal::loadSequence(vector<pair<string, int>> xxAnim)
{
	int xnumber = xxAnim.size();
	for (int i = 0; i < xnumber; i++)
	{
		cout << xxAnim[i].first << endl;
	}
}

void SendSignal::playSequence(vector<pair<string, int>> xxAnim)
{
	anim = xxAnim;
	anim_image_index = 0;
	anim_frame_count = 0;
	neutral = false;
}

void SendSignal::cleanup(){
	//Release Camera Data
	if (fdata) fdata->Release();
	if (facec) facec->Release();
	faceModule = nullptr;
	mSenseMgr->Close();
	mSenseMgr->Release();
	//Release FMOD

}

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