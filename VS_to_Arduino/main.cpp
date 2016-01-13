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

int robotX, robotY;

#pragma region AnimationData

vector<keyframe> BlinkAnim = {
	{"../textures/neutral/eyeOpen.jpg", std::chrono::milliseconds(0),   robotX, robotY },
	{"../textures/wink/wink_03.jpg",    std::chrono::milliseconds(33),  robotX, robotY },
	{"../textures/wink/wink_04.jpg",    std::chrono::milliseconds(66),  robotX, robotY },
	{"../textures/wink/wink_05.jpg",    std::chrono::milliseconds(100), robotX, robotY },
	{"../textures/wink/wink_06.jpg",    std::chrono::milliseconds(133), robotX, robotY },
	{"../textures/wink/wink_05.jpg",    std::chrono::milliseconds(166), robotX, robotY },
	{"../textures/wink/wink_04.jpg",    std::chrono::milliseconds(200), robotX, robotY },
	{"../textures/wink/wink_03.jpg",    std::chrono::milliseconds(233), robotX, robotY },
	{"../textures/wink/wink_02.jpg",    std::chrono::milliseconds(266), robotX, robotY },
	{"../textures/neutral/eyeOpen.jpg", std::chrono::milliseconds(300), robotX, robotY }
};


vector<keyframe> SupriseAnim = {
	{ "../textures/neutral/eyeOpen.jpg", std::chrono::milliseconds(0),			0, 0 }, //0
	{ "../textures/suprise/suprise_4.jpg", std::chrono::milliseconds(33),		-1.147292, 0 }, //2
	{ "../textures/suprise/suprise_4.jpg", std::chrono::milliseconds(133),		-7.342667, 0 }, //8
	{ "../textures/suprise/suprise_4.jpg", std::chrono::milliseconds(266),		1.302063, 0 }, //16
	{ "../textures/suprise/suprise_3-5.jpg", std::chrono::milliseconds(300),	0.3375715, 0 },  //18
	{ "../textures/suprise/suprise_3-5.jpg", std::chrono::milliseconds(316),	0, 0 },  //19
	{ "../textures/suprise/suprise_3.jpg", std::chrono::milliseconds(333),		0, 0 }, //20
	{ "../textures/suprise/suprise_3.jpg", std::chrono::milliseconds(416),		0, 0 }, //25
	{ "../textures/suprise/suprise_3.jpg", std::chrono::milliseconds(600),		2.500671, 0 }, //36
	{ "../textures/suprise/close_2.jpg", std::chrono::milliseconds(616),		2.650166, 0.003886184 },  //37
	{ "../textures/suprise/close_3.jpg", std::chrono::milliseconds(683),		5.747901, 0.08441316 },  //41
	{ "../textures/suprise/close_4.jpg", std::chrono::milliseconds(750),		11.43361, 0.2322157 },  //45
	{ "../textures/suprise/close_5.jpg", std::chrono::milliseconds(816),		17.82514, 0.3983663 },  //49
	{ "../textures/suprise/close_6.jpg", std::chrono::milliseconds(883),		23.04031, 0.5339369 },  //53
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(950),		25.19696, 0.59 },  //57
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(983),		25.19696, -0.59 },  //59
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(1016),		25.19696, 0.59 },  //61
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(1050),		25.19696, -0.59 },  //63
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(1083),		25.19696, 0.59 },  //65
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(1116),		25.19696, -0.59 },  //67
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(1150),		25.19696, 0.59 },  //69
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(1183),		25.19696, -0.59 },  //71
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(1216),		25.19696, 0.59 },  //73
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(1250),		25.19696, -0.59 },  //75
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(1283),		25.19696, 0.59 },  //77
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(1316),		25.19696, -0.59 },  //79
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(1350),		25.19696, 0.59 },  //81
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(1383),		25.19696, -0.59 },  //83
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(1416),		25.19696, 0.59 },  //85
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(1450),		25.19696, -0.59 },  //87
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(1483),		25.19696, 0.59 },  //89
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(1516),		25.19696, -0.59 },  //91
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(1550),		25.19696, 0.59 },  //93	
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(1583),		25.19696, -0.59 },  //95
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(1616),		25.19696, 0.59 },  //97
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(1650),		25.19696, -0.59 },  //99
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(1683),		25.19696, 0.59 },  //101
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(1716),		25.19696, -0.59 },  //103
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(1750),		25.19696, 0.59 },  //105
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(1783),		25.19696, -0.59 },  //107
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(1816),		25.19696, 0.59 },  //109
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(1850),		25.19696, -0.59 },  //111
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(1883),		25.19696, 0.59 },  //113
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(1916),		25.19696, -0.59 },  //115
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(1966),		25.19696, 0.59 },  //118
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(2016),		25.19696, -0.59 },  //121
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(2066),		25.19696, 0.59 },  //124
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(2116),		25.19696, -0.59 },  //127
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(2183),		25.19696, 0.59 },  //131
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(2250),		25.19696, -0.59 },  //135
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(2316),		25.19696, 0.59 },  //139
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(2383),		25.19696, -0.59 },  //143
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(2483),		25.19696, 0.59 },  //149
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(2583),		25.19696, -0.59 },  //155
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(2683),		25.19696, 0.59 },  //161
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(2783),		25.19696, -0.59 },  //167
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(2883),		25.19696, 0.59 },  //173
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(2983),		25.19696, -0.59 },  //179
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(3083),		25.19696, 0 },  //185
	{ "../textures/suprise/close_7.jpg", std::chrono::milliseconds(3500),		25.19696, 0 },  //210
	{ "../textures/wink/wink_05.jpg", std::chrono::milliseconds(3533),			25.19696, 0 },     //212
	{ "../textures/wink/wink_04.jpg", std::chrono::milliseconds(3566),			25.19696, 0 },     //214
	{ "../textures/wink/wink_03.jpg", std::chrono::milliseconds(3600),			25.19696, 0 },     //216
	{ "../textures/wink/wink_02.jpg", std::chrono::milliseconds(3633),			25.19696, 0 },     //218
	{ "../textures/neutral/eyeOpen.jpg", std::chrono::milliseconds(3666),		25.19696, 0 },  //220
	{ "../textures/wink/wink_02.jpg", std::chrono::milliseconds(3700),			25.19696, 0 },     //222
	{ "../textures/wink/wink_02.jpg", std::chrono::milliseconds(3733),			25.19696, 0 },     //224
	{ "../textures/wink/wink_04.jpg", std::chrono::milliseconds(3766),			25.19696, 0 },     //226
	{ "../textures/wink/wink_05.jpg", std::chrono::milliseconds(3800),			25.19696, 0 },     //228
	{ "../textures/wink/wink_04.jpg", std::chrono::milliseconds(3833),			25.19696, 0 },     //230
	{ "../textures/wink/wink_02.jpg", std::chrono::milliseconds(3866),			25.19696, 0 },     //232
	{ "../textures/wink/wink_02.jpg", std::chrono::milliseconds(3900),			25.19696, 0 },     //234
	{ "../textures/neutral/eyeOpen.jpg", std::chrono::milliseconds(3933),		25.19696, 0 },  //236
	{ "../textures/neutral/eyeOpen.jpg", std::chrono::milliseconds(4333),		25.19696, 0 },  //260
	{ "../textures/suprise/lookR_02.jpg", std::chrono::milliseconds(4366),		25.19696, 0 }, //262
	{ "../textures/suprise/lookR_03.jpg", std::chrono::milliseconds(4400),		25.19696, 0 }, //264
	{ "../textures/suprise/lookR_03.jpg", std::chrono::milliseconds(4416),		25.19696, 0 }, //265
	{ "../textures/suprise/lookR_04.jpg", std::chrono::milliseconds(4433),		25.12848, 0.05156203 }, //266
	{ "../textures/suprise/lookR_05.jpg", std::chrono::milliseconds(4466),		24.60528, 0.4454874 }, //268
	{ "../textures/suprise/lookR_05.jpg", std::chrono::milliseconds(4983),		-1.719009, 20.2654 }, //299
	{ "../textures/suprise/lookR_05.jpg", std::chrono::milliseconds(5883),		-1.719009, 20.2654 }, //353
	{ "../textures/suprise/lookR_04.jpg", std::chrono::milliseconds(5916),		-1.719009, 20.2654 }, //355
	{ "../textures/suprise/lookR_03.jpg", std::chrono::milliseconds(5950),		-1.719009, 20.2654 }, //357
	{ "../textures/suprise/lookR_02.jpg", std::chrono::milliseconds(5983),		-1.719009, 20.2654 }, //359
	{ "../textures/neutral/eyeOpen.jpg", std::chrono::milliseconds(6016),		-1.719009, 20.2654 },  //361
	{ "../textures/suprise/lookL_02.jpg", std::chrono::milliseconds(6050),		-1.719009, 20.2654 }, //363
	{ "../textures/suprise/lookL_03.jpg", std::chrono::milliseconds(6083),		-1.719009, 20.2654 }, //365
	{ "../textures/suprise/lookL_04.jpg", std::chrono::milliseconds(6116),		-1.717053, 19.98162 }, //367
	{ "../textures/suprise/lookL_05.jpg", std::chrono::milliseconds(6150),		-1.711414, 19.1634 }, //369
	{ "../textures/suprise/lookL_05.jpg", std::chrono::milliseconds(6866),		-1.348434, -33.5 }, //412
	{ "../textures/suprise/lookL_05.jpg", std::chrono::milliseconds(7816),		-1.348434, -33.5 },      //469
	{ "../textures/suprise/lookL_04-5.jpg", std::chrono::milliseconds(7883),	-1.348434, -33.5 },    //473
	{ "../textures/suprise/lookL_04.jpg", std::chrono::milliseconds(7950),		-1.348434, -33.5 },      //477
	{ "../textures/suprise/lookL_04.jpg", std::chrono::milliseconds(8000),		-1.348434, -33.5 },	//480
	{ "../textures/suprise/lookL_03-5.jpg", std::chrono::milliseconds(8016),	-1.579713, -33.03981 },    //481
	{ "../textures/suprise/lookL_03-5.jpg", std::chrono::milliseconds(8050),	-2.24054, -31.72493 },    //483
	{ "../textures/suprise/lookL_03.jpg", std::chrono::milliseconds(8066),		-2.41259, -31.50904 },      //484
	{ "../textures/suprise/lookL_02-5.jpg", std::chrono::milliseconds(8133),	-4.977773, -28.29022 },    //488
	{ "../textures/suprise/lookL_02-5.jpg", std::chrono::milliseconds(8183),	-6.244736, -26.70042 },    //491
	{ "../textures/suprise/lookL_02.jpg", std::chrono::milliseconds(8200),		-6.219673, -26.59326 },      //492
	{ "../textures/suprise/lookL_01-5.jpg", std::chrono::milliseconds(8283),	-5.456642, -23.3308 },    //497
	{ "../textures/neutral/eyeOpen.jpg", std::chrono::milliseconds(8350),		-4.309409, -18.42561 },        //501
	{ "../textures/neutral/eyeOpen.jpg", std::chrono::milliseconds(8633),		0, 0 }        //518
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