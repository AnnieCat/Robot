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
#include <time.h>



using namespace std;

int width, height;
float millisec;
int blinkVal = 8;
int timeIndex = 0;
bool neutral = true;
bool iSeeYou = false;

//Emotion Triggers
bool showingEmotion = false;


// Begin Sterling suggestion
/*
typedef std::chrono::system_clock clock;
//clock::time_point
//clock::duration
typedef vector<pair<string, clock::duration>> animation;

animation BlinkAnimX = {
	{ "../textures/neutral/eyeOpen.jpg", std::chrono::milliseconds(33) },
	{ "../textures/wink/wink_03.jpg", std::chrono::milliseconds(66) },
	{ "../textures/wink/wink_04.jpg", std::chrono::milliseconds(100) },
	{ "../textures/wink/wink_05.jpg", std::chrono::milliseconds(133) },
	{ "../textures/wink/wink_06.jpg", std::chrono::milliseconds(166) },
	{ "../textures/wink/wink_05.jpg", std::chrono::milliseconds(200) },
	{ "../textures/wink/wink_04.jpg", std::chrono::milliseconds(233) },
	{ "../textures/wink/wink_03.jpg", std::chrono::milliseconds(266) },
	{ "../textures/wink/wink_02.jpg", std::chrono::milliseconds(300) },
	{ "../textures/neutral/eyeOpen.jpg", std::chrono::milliseconds(333) }
};

const animation * current_animation;
clock::time_point time_anim_started;

void start_animation(const animation & anim)
{
	current_animation = &anim;
	time_anim_started = clock::now();
}

void show_frame(const std::string & s)
{
	// ...
}

void show_current_frame()
{
	if (current_animation)
	{
		clock::duration time_passed = clock::now() - time_anim_started;

		for (auto & p : *current_animation)
		{
			if (time_passed < p.second)
			{
				show_frame(p.first);
				return;
			}
		}

		// Animation is done
		current_animation = nullptr;
		// Anything else you want to do here
	}
}

// End suggestion

*/

typedef vector<pair<string, std::chrono::milliseconds>> animation;
const animation * current_animation;
std::chrono::system_clock::time_point time_anim_started;


stbi_uc *myImage = stbi_load("../textures/neutral/eyeOpen.jpg", &width, &height, nullptr, 3);
stbi_uc *neutralImage = stbi_load("../textures/neutral/eyeOpen.jpg", &width, &height, nullptr, 3);

#pragma region AnimationData
/*vector<pair<string, int>> BlinkAnim = {
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
};*/


animation SupriseAnimT = {
	{ "../textures/neutral/eyeOpen.jpg", std::chrono::milliseconds(0) },
	{ "../textures/suprise/suprise_4.jpg", std::chrono::milliseconds(33) },
	{ "../textures/suprise/suprise_3-5.jpg", std::chrono::milliseconds(300) },
	{ "../textures/suprise/suprise_3.jpg", std::chrono::milliseconds(333) },
	{ "../textures/suprise/close_2.jpg", std::chrono::milliseconds(616) },
	{ "../textures/suprise/close_3.jpg", std::chrono::milliseconds(683) },
	{ "../textures/suprise/close_4.jpg", std::chrono::milliseconds(750) },
	{ "../textures/suprise/close_5.jpg", std::chrono::milliseconds(816) },
	{ "../textures/suprise/close_6.jpg", std::chrono::milliseconds(883) },
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(950) },
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(3500) },
	{ "../textures/wink/wink_05.jpg", std::chrono::milliseconds(3533) },
	{ "../textures/wink/wink_04.jpg", std::chrono::milliseconds(3566) },
	{ "../textures/wink/wink_03.jpg", std::chrono::milliseconds(3600) },
	{ "../textures/wink/wink_02.jpg", std::chrono::milliseconds(3633) },
	{ "../textures/neutral/eyeOpen.jpg", std::chrono::milliseconds(3666) },
	{ "../textures/wink/wink_02.jpg", std::chrono::milliseconds(3700) },
	{ "../textures/wink/wink_02.jpg", std::chrono::milliseconds(3733) },
	{ "../textures/wink/wink_04.jpg", std::chrono::milliseconds(3766) },
	{ "../textures/wink/wink_05.jpg", std::chrono::milliseconds(3800) },
	{ "../textures/wink/wink_04.jpg", std::chrono::milliseconds(3833) },
	{ "../textures/wink/wink_02.jpg", std::chrono::milliseconds(3866) },
	{ "../textures/wink/wink_02.jpg", std::chrono::milliseconds(3900) },
	{ "../textures/neutral/eyeOpen.jpg", std::chrono::milliseconds(3933) },
	{ "../textures/neutral/eyeOpen.jpg", std::chrono::milliseconds(4333) },
	{ "../textures/suprise/lookR_02.jpg", std::chrono::milliseconds(4366) },
	{ "../textures/suprise/lookR_03.jpg", std::chrono::milliseconds(4400) },
	{ "../textures/suprise/lookR_04.jpg", std::chrono::milliseconds(4433) },
	{ "../textures/suprise/lookR_05.jpg", std::chrono::milliseconds(4466) },
	{ "../textures/suprise/lookR_05.jpg", std::chrono::milliseconds(5883) },
	{ "../textures/suprise/lookR_04.jpg", std::chrono::milliseconds(5916) },
	{ "../textures/suprise/lookR_03.jpg", std::chrono::milliseconds(5950) },
	{ "../textures/suprise/lookR_02.jpg", std::chrono::milliseconds(5983) },
	{ "../textures/neutral/eyeOpen.jpg", std::chrono::milliseconds(6016) },
	{ "../textures/suprise/lookL_02.jpg", std::chrono::milliseconds(6050) },
	{ "../textures/suprise/lookL_03.jpg", std::chrono::milliseconds(6083) },
	{ "../textures/suprise/lookL_04.jpg", std::chrono::milliseconds(6116) },
	{ "../textures/suprise/lookL_05.jpg", std::chrono::milliseconds(6150) },
	{ "../textures/suprise/lookL_05.jpg", std::chrono::milliseconds(7816) },
	{ "../textures/suprise/lookL_04-5.jpg", std::chrono::milliseconds(7883) },
	{ "../textures/suprise/lookL_04.jpg", std::chrono::milliseconds(7950) },
	{ "../textures/suprise/lookL_03-5.jpg", std::chrono::milliseconds(8016) },
	{ "../textures/suprise/lookL_03.jpg", std::chrono::milliseconds(8066) },
	{ "../textures/suprise/lookL_02-5.jpg", std::chrono::milliseconds(8133) },
	{ "../textures/suprise/lookL_02.jpg", std::chrono::milliseconds(8200) },
	{ "../textures/suprise/lookL_01-5.jpg", std::chrono::milliseconds(8283) },
	{ "../textures/neutral/eyeOpen.jpg", std::chrono::milliseconds(8350) }

};

/*vector<pair<string, int>> SupriseAnim = {
	{ "../textures/neutral/eyeOpen.jpg", 0 },
	{ "../textures/suprise/suprise_4.jpg", 2 },
	{ "../textures/suprise/suprise_3-5.jpg", 16 },  
	{ "../textures/suprise/suprise_3.jpg", 2 },   
	{ "../textures/suprise/close_2.jpg", 17 },		
	{ "../textures/suprise/close_3.jpg", 4 },		
	{ "../textures/suprise/close_4.jpg", 4 },		
	{ "../textures/suprise/close_5.jpg", 4 },		
	{ "../textures/suprise/close_6.jpg", 4 },		
	{ "../textures/suprise/close_7.jpg", 4 },		
	{ "../textures/suprise/close_7.jpg", 153 },		
	{ "../textures/wink/wink_05.jpg", 2 },		
	{ "../textures/wink/wink_04.jpg", 2 },		
	{ "../textures/wink/wink_03.jpg", 2 },		
	{ "../textures/wink/wink_02.jpg", 2 },		
	{ "../textures/neutral/eyeOpen.jpg", 2 },	
	{ "../textures/wink/wink_02.jpg", 2 },		
	{ "../textures/wink/wink_02.jpg", 2 },		
	{ "../textures/wink/wink_04.jpg", 2 },		
	{ "../textures/wink/wink_05.jpg", 2 },		
	{ "../textures/wink/wink_04.jpg", 2 },		
	{ "../textures/wink/wink_02.jpg", 2 },		
	{ "../textures/wink/wink_02.jpg", 2 },		
	{ "../textures/neutral/eyeOpen.jpg", 2 },		
	{ "../textures/neutral/eyeOpen.jpg", 24 },		
	{ "../textures/suprise/lookR_02.jpg", 2 },	
	{ "../textures/suprise/lookR_03.jpg", 2 },	
	{ "../textures/suprise/lookR_04.jpg", 2 },	
	{ "../textures/suprise/lookR_05.jpg", 2 },	
	{ "../textures/suprise/lookR_05.jpg", 85 },	
	{ "../textures/suprise/lookR_04.jpg", 2 },	
	{ "../textures/suprise/lookR_03.jpg", 2 },	
	{ "../textures/suprise/lookR_02.jpg", 2 },	
	{ "../textures/neutral/eyeOpen.jpg", 2 },	
	{ "../textures/suprise/lookL_02.jpg", 2 },	
	{ "../textures/suprise/lookL_03.jpg", 2 },	
	{ "../textures/suprise/lookL_04.jpg", 2 },	
	{ "../textures/suprise/lookL_05.jpg", 2 },	
	{ "../textures/suprise/lookL_05.jpg", 100 },
	{ "../textures/suprise/lookL_04-5.jpg", 4 },
	{ "../textures/suprise/lookL_04.jpg", 4 },	
	{ "../textures/suprise/lookL_03-5.jpg", 4 },
	{ "../textures/suprise/lookL_03.jpg", 3 },	
	{ "../textures/suprise/lookL_02-5.jpg", 4 },
	{ "../textures/suprise/lookL_02.jpg", 4 },	
	{ "../textures/suprise/lookL_01-5.jpg", 4 },
	{ "../textures/neutral/eyeOpen.jpg", 3 }	

};*/
#pragma endregion

class SendSignal	
{
	//vector<pair<string, int>> anim;
	//int anim_image_index;
	//int anim_frame_count;
	//float frameMultiplier = 2;
	

public: 
	void setup();
	//void timerUpdate(float timestep);
	//void drawFrame(int, int);
	//void loadSequence(vector<pair<string, int>>);
	//void playSequence(vector<pair<string, int>>);
	//void sendEmotion(string);

	void start_animation(const animation & anim);
	void showFrame(const std::string & s);
	void show_current_frame();

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

	//double t0 = glfwGetTime();
	

	MidiPlayer player; // Put it outside the loop

#pragma region loadData

	PXCFaceData::ExpressionsData::FaceExpressionResult raiseLeftBrow;
	PXCFaceData::ExpressionsData::FaceExpressionResult raiseRightBrow;
	PXCFaceData::ExpressionsData::FaceExpressionResult openMouth;
	
#pragma endregion
	
	while (!glfwWindowShouldClose(win))
	{
		glfwPollEvents();
		

		//const double t1 = glfwGetTime();
		//const float timestep = (float)(t1 - t0);
		//t0 = t1;
		
		//mySignal.timerUpdate(timestep);
		
		// update camera information here
#pragma region Perceptual

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
						if (neutral)
						{
							mySignal.start_animation(SupriseAnimT);
							//showingEmotion = true;
						}
					}
#pragma endregion
				}
				mySignal.mSenseMgr->ReleaseFrame();
			}

#pragma endregion
		}

#pragma endregion

		// All Output goes Here- rendering, commands to arduino
		glfwGetFramebufferSize(win, &width, &height);
		glfwMakeContextCurrent(win);

		glViewport(0, 0, width, height);
		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glLoadIdentity();
		glOrtho(0, width, height, 0, -1, +1);
		
		//mySignal.drawFrame(width, height);
		mySignal.show_current_frame();

		glfwSwapBuffers(win);
	}

	glfwTerminate();
	mySignal.cleanup();
}

void SendSignal::start_animation(const animation & anim)
{
	current_animation = &anim;
	time_anim_started = std::chrono::system_clock::now();
	AudioPlay("suprise.wav");
}

void SendSignal::showFrame(const std::string & s)
{
	myImage = stbi_load(s.c_str(), &width, &height, nullptr, 3);

	glRasterPos2i((width - 800) / 2, (height - 480) / 2);
	glPixelZoom(1, -1);
	glDrawPixels(800, 480, GL_RGB, GL_UNSIGNED_BYTE, myImage);
}

void SendSignal::show_current_frame()
{
	if (current_animation)
	{
		std::chrono::system_clock::duration time_passed = std::chrono::system_clock::now() - time_anim_started;
		neutral = false;
		for (auto & p : *current_animation)
		{
			if (time_passed < p.second)
			{
				showFrame(p.first);
				return;
			}
		}
		neutral = true;
		current_animation = nullptr;
		glDrawPixels(800, 400, GL_RGB, GL_UNSIGNED_BYTE, neutralImage);
	}
	else
	{
		glRasterPos2i((width - 800) / 2, (height - 480) / 2);
		glPixelZoom(1, -1);
		glDrawPixels(800, 400, GL_RGB, GL_UNSIGNED_BYTE, neutralImage);
	}
}

/*void SendSignal::sendEmotion(string myEmo)
{
	if (myEmo == "suprise")
	{
		cout << "suprise" << endl;
		playSequence(SupriseAnim);
		//playSequence(SupriseAnimT);
		AudioPlay("suprise.wav");
	}
}*/


/*void SendSignal::timerUpdate(float timestep)
{
	//This is our Blink release
	millisec += timestep;
	//cout << millisec << endl;

	if (millisec > blinkVal)
	{
		if (neutral)
		{
			AudioPlay("blink.wav");
			//AudioPlay("suprise.wav");
			cout << "blink!" << endl;
			cout << blinkVal << endl;
			playSequence(BlinkAnim);
			//playSequence(SupriseAnim);

			blinkVal = 6 + (rand() % 10);
			millisec = 0;
		}
		else
		{
			if (!showingEmotion)
				millisec = 0;
		}
	}


}*/




/*void SendSignal::drawFrame(int windowWidth, int windowHeight)
{
	int resetFrameCount;
	

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
			showingEmotion = false;
			millisec = 0;
		}
	}

	glRasterPos2i((width - 800) / 2, (height - 480) / 2);
	glPixelZoom(1, -1);
	glDrawPixels(800, 480, GL_RGB, GL_UNSIGNED_BYTE, myImage);


	++anim_frame_count;
	
}*/

/*void SendSignal::drawFrame(int windowWidth, int windowHeight)
{
int resetFrameCount;



// If there is an animation and we have waited the appropriate number of frames
if (anim_image_index < anim.size())
{
	//auto v = std::find(std::begin(anim[0].second),std::end(anim[46].second,int(millisec*60));
	cout << anim[anim_image_index].second << endl;
	cout << millisec << endl;

	// Load the next image from the animation
	myImage = stbi_load(anim[anim_image_index].first.c_str(), &width, &height, nullptr, 3);


	++anim_image_index;
	anim_frame_count = 0;

	// If we have reached the end of the animation
	if (anim_image_index == anim.size())
	{
	neutral = true;
	showingEmotion = false;
	millisec = 0;

	cout << "finished" << endl;
	}
}

glRasterPos2i((width - 800) / 2, (height - 480) / 2);
glPixelZoom(1, -1);
glDrawPixels(800, 480, GL_RGB, GL_UNSIGNED_BYTE, myImage);


++anim_frame_count;

}*/


/*void SendSignal::playSequence(vector<pair<string, int>> xxAnim)
{
	anim = xxAnim;
	anim_image_index = 0;
	anim_frame_count = 0;
	neutral = false;
}*/

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