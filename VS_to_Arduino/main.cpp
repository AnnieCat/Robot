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
std::chrono::system_clock::duration millisec;
int blinkVal = 18;
int timeIndex = 0;
bool neutral = true;
bool iSeeYou = false;

//Emotion Triggers
bool showingEmotion = false;


typedef vector<pair<string, std::chrono::milliseconds>> animation;



struct keyframe
{
	std::string image;
	std::chrono::milliseconds time;
	int x, y;
};

//typedef  altAnimation;


const vector<keyframe> * current_animation;
std::chrono::system_clock::time_point time_anim_started;
std::chrono::system_clock::time_point reset_blink_timer;


stbi_uc *myImage = stbi_load("../textures/neutral/eyeOpen.jpg", &width, &height, nullptr, 3);
stbi_uc *neutralImage = stbi_load("../textures/neutral/eyeOpen.jpg", &width, &height, nullptr, 3);


/*template<class A, class B, class C>
std::vector<std::tuple<A,B,C>> threeway_zip(const std::vector<A> & a, const std::vector<B> & b, const std::vector<C> & c)
{
	std::vector<std::tuple<A, B, C>> results;
	for (size_t i = 0; i < a.size(); ++i)
	{
		results.push_back({ a[i], b[i], c[i] });
	}
	return results;
}*/


#pragma region AnimationData

vector<keyframe> BlinkAnim = {
	{"../textures/neutral/eyeOpen.jpg", std::chrono::milliseconds(0),   50, 60 },
	{"../textures/wink/wink_03.jpg",    std::chrono::milliseconds(33),  52, 60 },
	{"../textures/wink/wink_04.jpg",    std::chrono::milliseconds(66),  54, 60 },
	{"../textures/wink/wink_05.jpg",    std::chrono::milliseconds(100), 56, 60 },
	{"../textures/wink/wink_06.jpg",    std::chrono::milliseconds(133), 58, 60 },
	{"../textures/wink/wink_05.jpg",    std::chrono::milliseconds(166), 60, 60 },
	{"../textures/wink/wink_04.jpg",    std::chrono::milliseconds(200), 62, 60 },
	{"../textures/wink/wink_03.jpg",    std::chrono::milliseconds(233), 64, 60 },
	{"../textures/wink/wink_02.jpg",    std::chrono::milliseconds(266), 66, 60 },
	{"../textures/neutral/eyeOpen.jpg", std::chrono::milliseconds(300), 68, 60 }
};


vector<keyframe> SupriseAnim = {
	{ "../textures/neutral/eyeOpen.jpg", std::chrono::milliseconds(0), 40, 60 },
	{ "../textures/suprise/suprise_4.jpg", std::chrono::milliseconds(33), 40, 60 },
	{ "../textures/suprise/suprise_3-5.jpg", std::chrono::milliseconds(300), 40, 60 },
	{ "../textures/suprise/suprise_3.jpg", std::chrono::milliseconds(333), 40, 60 },
	{ "../textures/suprise/close_2.jpg", std::chrono::milliseconds(616), 40, 60 },
	{ "../textures/suprise/close_3.jpg", std::chrono::milliseconds(683), 40, 60 },
	{ "../textures/suprise/close_4.jpg", std::chrono::milliseconds(750), 40, 60 },
	{ "../textures/suprise/close_5.jpg", std::chrono::milliseconds(816), 40, 60 },
	{ "../textures/suprise/close_6.jpg", std::chrono::milliseconds(883), 40, 60 },
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(950), 40, 60 },
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(3500), 40, 60 },
	{ "../textures/wink/wink_05.jpg", std::chrono::milliseconds(3533), 40, 60 },
	{ "../textures/wink/wink_04.jpg", std::chrono::milliseconds(3566), 40, 60 },
	{ "../textures/wink/wink_03.jpg", std::chrono::milliseconds(3600), 40, 60 },
	{ "../textures/wink/wink_02.jpg", std::chrono::milliseconds(3633), 40, 60 },
	{ "../textures/neutral/eyeOpen.jpg", std::chrono::milliseconds(3666), 40, 60 },
	{ "../textures/wink/wink_02.jpg", std::chrono::milliseconds(3700), 40, 60 },
	{ "../textures/wink/wink_02.jpg", std::chrono::milliseconds(3733), 40, 60 },
	{ "../textures/wink/wink_04.jpg", std::chrono::milliseconds(3766), 40, 60 },
	{ "../textures/wink/wink_05.jpg", std::chrono::milliseconds(3800), 40, 60 },
	{ "../textures/wink/wink_04.jpg", std::chrono::milliseconds(3833), 40, 60 },
	{ "../textures/wink/wink_02.jpg", std::chrono::milliseconds(3866), 40, 60 },
	{ "../textures/wink/wink_02.jpg", std::chrono::milliseconds(3900), 40, 60 },
	{ "../textures/neutral/eyeOpen.jpg", std::chrono::milliseconds(3933), 40, 60 },
	{ "../textures/neutral/eyeOpen.jpg", std::chrono::milliseconds(4333), 40, 60 },
	{ "../textures/suprise/lookR_02.jpg", std::chrono::milliseconds(4366), 40, 60 },
	{ "../textures/suprise/lookR_03.jpg", std::chrono::milliseconds(4400), 40, 60 },
	{ "../textures/suprise/lookR_04.jpg", std::chrono::milliseconds(4433), 40, 60 },
	{ "../textures/suprise/lookR_05.jpg", std::chrono::milliseconds(4466), 40, 60 },
	{ "../textures/suprise/lookR_05.jpg", std::chrono::milliseconds(5883), 40, 60 },
	{ "../textures/suprise/lookR_04.jpg", std::chrono::milliseconds(5916), 40, 60 },
	{ "../textures/suprise/lookR_03.jpg", std::chrono::milliseconds(5950), 40, 60 },
	{ "../textures/suprise/lookR_02.jpg", std::chrono::milliseconds(5983), 40, 60 },
	{ "../textures/neutral/eyeOpen.jpg", std::chrono::milliseconds(6016), 40, 60 },
	{ "../textures/suprise/lookL_02.jpg", std::chrono::milliseconds(6050), 40, 60 },
	{ "../textures/suprise/lookL_03.jpg", std::chrono::milliseconds(6083), 40, 60 },
	{ "../textures/suprise/lookL_04.jpg", std::chrono::milliseconds(6116), 40, 60 },
	{ "../textures/suprise/lookL_05.jpg", std::chrono::milliseconds(6150), 40, 60 },
	{ "../textures/suprise/lookL_05.jpg", std::chrono::milliseconds(7816), 40, 60 },
	{ "../textures/suprise/lookL_04-5.jpg", std::chrono::milliseconds(7883), 40, 60 },
	{ "../textures/suprise/lookL_04.jpg", std::chrono::milliseconds(7950), 40, 60 },
	{ "../textures/suprise/lookL_03-5.jpg", std::chrono::milliseconds(8016), 40, 60 },
	{ "../textures/suprise/lookL_03.jpg", std::chrono::milliseconds(8066), 40, 60 },
	{ "../textures/suprise/lookL_02-5.jpg", std::chrono::milliseconds(8133), 40, 60 },
	{ "../textures/suprise/lookL_02.jpg", std::chrono::milliseconds(8200), 40, 60 },
	{ "../textures/suprise/lookL_01-5.jpg", std::chrono::milliseconds(8283), 40, 60 },
	{ "../textures/neutral/eyeOpen.jpg", std::chrono::milliseconds(8350), 40, 60 }
};


#pragma endregion

class SendSignal	
{
public: 
	void setup();
	void start_animation(const vector<keyframe> & anim);
	void showFrame(const std::string & s);
	void show_current_frame();

	void blink_timer(std::chrono::system_clock::duration x);

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


int robotX, robotY;


int main()
{
	SendSignal mySignal;
	mySignal.setup();

	glfwInit();
	GLFWwindow *win = glfwCreateWindow(800, 480, "Robot Face", nullptr, nullptr);
	//GLFWwindow *win = glfwCreateWindow(800, 480, "Robot Face", glfwGetPrimaryMonitor(), nullptr); //fullscreen

	std::chrono::system_clock::time_point t0 = std::chrono::system_clock::now();
	

	MidiPlayer player; // Put it outside the loop
	
	

#pragma region loadData

	PXCFaceData::ExpressionsData::FaceExpressionResult raiseLeftBrow;
	PXCFaceData::ExpressionsData::FaceExpressionResult raiseRightBrow;
	PXCFaceData::ExpressionsData::FaceExpressionResult openMouth;
	
#pragma endregion
	robotX = 30, robotY = 50;

	while (!glfwWindowShouldClose(win))
	{
		glfwPollEvents();
		

		const std::chrono::system_clock::time_point t1 = std::chrono::system_clock::now();
		const std::chrono::system_clock::duration timestep = t1 - t0;
		t0 = t1;
		
		mySignal.blink_timer(timestep);
		
		// update camera information here
#pragma region Perceptual

		int numFaces = 0;
		
		if (mySignal.mSenseMgr->AcquireFrame(true) >= PXC_STATUS_NO_ERROR)
		{
			PXCEmotion *emotionDet = mySignal.mSenseMgr->QueryEmotion();
			PXCEmotion::EmotionData emotion[10];

			mySignal.fdata->Update();

			int numFaces = mySignal.fdata->QueryNumberOfDetectedFaces();

			int closestface = -1;
			iSeeYou = false;
			int lowestdisparity = INT_MAX;
			int targetX, targetY;

			for (int i = 0; i < numFaces; ++i)
			{

				emotionDet->QueryAllEmotionData(i, &emotion[0]);

#pragma region Face Tracking

				if (emotion[0].rectangle.x > -1 && emotion[0].rectangle.y > -1)
				{

					iSeeYou = true;
					
					int faceX = Map(emotion[0].rectangle.x, 500, 0, 0, 80); //Camera min/max : 500, 0    Robot Min/Max : 50 / 130 
					int faceY = Map(emotion[0].rectangle.y, 0, 320, 0, 60); // Camera min/max:  0,320   Robot min/max:  40,100
					int d = abs(faceX - robotX) + abs(faceY - robotY);   // disparity between robot's current gaze and i'th face in the camera's current frame 
					if (d < lowestdisparity)
					{
						closestface = i;
						lowestdisparity = d;
						targetX = faceX;
						targetY = faceY;
					}

					// the following code pivots the robot to turn toward the target face up to 1 degree on each axis per frame.



				}
			}
			if (iSeeYou)
			{
				
				if (targetX > robotX)
					robotX++;
				if (targetX < robotX)
					robotX--;
				if (targetY > robotY)
					robotY++;
				if (targetY < robotY)
					robotY--;
				
				player.SendMidiMessage(numFaces + 1, robotX, robotY);

				cout << closestface << " " << numFaces << "      " <<  targetX << ", " << targetY << "      " << robotX << " " << robotY << "     " << showingEmotion << endl;
			}


#pragma endregion

#pragma region Expression Logic

			if (iSeeYou)
			{
				PXCFaceData::Face *face = mySignal.fdata->QueryFaceByIndex(closestface);
				PXCFaceData::ExpressionsData *edata = face->QueryExpressions();
				if (edata)
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
							mySignal.start_animation(SupriseAnim);

							cout << "suprise!" << endl;
							showingEmotion = true;
						}
					}
#pragma endregion
				}
			}
				
			
		
			mySignal.mSenseMgr->ReleaseFrame();

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
		
		mySignal.show_current_frame();

		glfwSwapBuffers(win);
	}

	glfwTerminate();
	mySignal.cleanup();
}


void SendSignal::blink_timer(std::chrono::system_clock::duration x)
{
	std::chrono::system_clock::duration currentTime = std::chrono::system_clock::now() - reset_blink_timer;
	if (currentTime > std::chrono::seconds(blinkVal) && neutral)
	{
		start_animation(BlinkAnim);
		reset_blink_timer = std::chrono::system_clock::now();
		blinkVal = 6 + (rand() % 10);
		cout << blinkVal << endl;
		neutral = false;

	}
}


void SendSignal::start_animation(const vector<keyframe> & anim)
{
	current_animation = &anim;
	time_anim_started = std::chrono::system_clock::now();
	if (&anim == &BlinkAnim)
		AudioPlay("blink.wav");
	//if (&anim == &SupriseAnim)
	//	AudioPlay("suprise.wav");
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
	MidiPlayer player;

	//if (current_animation)
	if (current_animation)
	{
		std::chrono::system_clock::duration time_passed = std::chrono::system_clock::now() - time_anim_started;
		
		for (auto & p : *current_animation)
		{
			
			if (time_passed < p.time)
			{
				showFrame(p.image); // p.x, p.y
				if (showingEmotion)
				{
					robotX = p.x;
					robotY = p.y;
				}
				return;
			}
		}
		
		current_animation = nullptr;
	}

	if(!current_animation)
	{
		glRasterPos2i((width - 800) / 2, (height - 480) / 2);
		glPixelZoom(1, -1);
		glDrawPixels(800, 400, GL_RGB, GL_UNSIGNED_BYTE, neutralImage);		
		neutral = true;
		showingEmotion = false;
	}
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