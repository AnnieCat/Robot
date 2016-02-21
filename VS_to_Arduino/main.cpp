#pragma comment(lib, "opengl32.lib")
#define STB_IMAGE_IMPLEMENTATION
#include <vector>
#include <string>

#include <chrono>
#include <thread>
#include <iostream>
#include <queue>
#include <memory>
#include <string>
#include <time.h>

#include "stb_image.h"

#include "pxcsensemanager.h"
//#include "pxcemotion.h"

#include "Midi.h"

#include "fmod.hpp"
#include "common.h"

#include "GLFW\glfw3.h"


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
	//float x, y;
	double x, y;
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

float robotX, robotY;

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

vector<keyframe> WinkAnim = {
	{ "../textures/suprise/suprise_3.jpg", std::chrono::milliseconds(0),				0, 0 },	//0
	{ "../textures/suprise/suprise_3.jpg", std::chrono::milliseconds(133),				-12.00215, 0 },	//8
	{ "../textures/suprise/suprise_3.jpg", std::chrono::milliseconds(266),				0, 0 },	//16
	{ "../textures/suprise/suprise_3.jpg", std::chrono::milliseconds(300),				0, 0 },	//18
	{ "../textures/suprise/suprise_2.jpg", std::chrono::milliseconds(333),				0, 0 },	//20

	{ "../textures/suprise/suprise_3.jpg", std::chrono::milliseconds(800),				0, 0 },	//48
	{ "../textures/wink/blush_02.jpg", std::chrono::milliseconds(833),					0, 0 },	//50
	{ "../textures/wink/blush_03.jpg", std::chrono::milliseconds(933),					0, -2.08 },	//56
	{ "../textures/wink/blush_04.jpg", std::chrono::milliseconds(1033),					0, -7.039998 },	//62
	{ "../textures/wink/blush_05.jpg", std::chrono::milliseconds(1133),					0, -12.96 },	//68
	{ "../textures/wink/blush_07.jpg", std::chrono::milliseconds(1333),					0, -20 },	//80

	{ "../textures/wink/blush_07.jpg", std::chrono::milliseconds(2600),					0, -20 },	//156
	{ "../textures/wink/blush_06.jpg", std::chrono::milliseconds(2666),					0, -20 },	//160
	{ "../textures/wink/blush_05.jpg", std::chrono::milliseconds(2733),					0, -17.92 },	//164
	{ "../textures/wink/blush_04.jpg", std::chrono::milliseconds(2800),					0, -12.96001 },	//168
	{ "../textures/wink/blush_03.jpg", std::chrono::milliseconds(2866),					0, -7.040013 },	//172
	{ "../textures/wink/blush_02.jpg", std::chrono::milliseconds(2933),					0, -2.079998 },	//176
	{ "../textures/neutral/eyeOpen.jpg", std::chrono::milliseconds(3000),				0, 0 },	//180
};

vector<keyframe> AbashedAnim = {
	{ "../textures/raisedEyebrow/sad_02.jpg", std::chrono::milliseconds(0), 0, 0 },	//0
	{ "../textures/raisedEyebrow/sad_02.jpg", std::chrono::milliseconds(166), 0.127626, 0.07203837 },	//10
	{ "../textures/raisedEyebrow/sad_03.jpg", std::chrono::milliseconds(200), 0.1762028, 0.09945752 },	//12
	{ "../textures/raisedEyebrow/sad_04.jpg", std::chrono::milliseconds(233), 0.2295162, 0.1295502 },	//14
	{ "../textures/raisedEyebrow/sad_05.jpg", std::chrono::milliseconds(266), 0.2863032, 0.1616036 },	//16
	{ "../textures/raisedEyebrow/sad_05.jpg", std::chrono::milliseconds(650), 0.7804787, 0.4405406 },	//39

	{ "../textures/raisedEyebrow/bottomLeft_02.jpg", std::chrono::milliseconds(1266), 0.02083659, 0.01176119 },	//76
	{ "../textures/raisedEyebrow/bottomLeft_03.jpg", std::chrono::milliseconds(1333), 0, 0 },	//80
	{ "../textures/raisedEyebrow/bottomLeft_04.jpg", std::chrono::milliseconds(1400), 0.7991387, -0.7889872 },	//84
	{ "../textures/raisedEyebrow/bottomLeft_05.jpg", std::chrono::milliseconds(1466), 2.924513, -2.887362 },	//88
	{ "../textures/raisedEyebrow/bottomLeft_06.jpg", std::chrono::milliseconds(1533), 5.968043, -5.892231 },	//92
	{ "../textures/raisedEyebrow/bottomLeft_07.jpg", std::chrono::milliseconds(1600), 9.521669, -9.400714 },	//96
	{ "../textures/raisedEyebrow/bottomLeft_07.jpg", std::chrono::milliseconds(1900), 20.88391, -20.61862 },	//114

	{ "../textures/raisedEyebrow/abashed_lookUp_02.jpg", std::chrono::milliseconds(3400), 20.88391, -20.61862 },	//204
	{ "../textures/raisedEyebrow/abashed_lookUp_03.jpg", std::chrono::milliseconds(3433), 20.88391, -20.61862 },	//206
	{ "../textures/raisedEyebrow/abashed_lookUp_04.jpg", std::chrono::milliseconds(3466), 20.88391, -20.61862 },	//208
	{ "../textures/raisedEyebrow/abashed_lookUp_05.jpg", std::chrono::milliseconds(3500), 20.70666, -20.50111 },	//210
	{ "../textures/raisedEyebrow/abashed_lookUp_06.jpg", std::chrono::milliseconds(3533), 20.23937, -20.19132 },	//212
	{ "../textures/raisedEyebrow/abashed_lookUp_07.jpg", std::chrono::milliseconds(3566), 19.57872, -19.75333 },	//214
	{ "../textures/raisedEyebrow/abashed_lookUp_08.jpg", std::chrono::milliseconds(3600), 18.82138, -19.25125 },	//216
	{ "../textures/raisedEyebrow/abashed_lookUp_09.jpg", std::chrono::milliseconds(3633), 18.06404, -18.74916 },	//218
	{ "../textures/raisedEyebrow/abashed_lookUp_09.jpg", std::chrono::milliseconds(3733), 16.75885, -17.88387 },	//224

	{ "../textures/raisedEyebrow/abashed_lookUp_09.jpg", std::chrono::milliseconds(4033), 16.75885, -17.88387 },	//242
	{ "../textures/raisedEyebrow/upperRight_blink_02.jpg", std::chrono::milliseconds(4066), 16.75885, -17.88387 },	//244
	{ "../textures/raisedEyebrow/upperRight_blink_03.jpg", std::chrono::milliseconds(4100), 16.75885, -17.88387 },	//246
	{ "../textures/raisedEyebrow/upperRight_blink_04.jpg", std::chrono::milliseconds(4133), 16.75885, -17.88387 },	//248
	{ "../textures/raisedEyebrow/upperRight_blink_05.jpg", std::chrono::milliseconds(4166), 16.75885, -17.88387 },	//250
	{ "../textures/raisedEyebrow/upperRight_blink_04.jpg", std::chrono::milliseconds(4200), 16.75885, -17.88387 },	//252
	{ "../textures/raisedEyebrow/upperRight_blink_03.jpg", std::chrono::milliseconds(4233), 16.75885, -17.88387 },	//254
	{ "../textures/raisedEyebrow/upperRight_blink_02.jpg", std::chrono::milliseconds(4266), 16.75885, -17.88387 },	//256
	{ "../textures/raisedEyebrow/abashed_lookUp_09.jpg", std::chrono::milliseconds(4300), 16.75885, -17.88387 },	//258

	{ "../textures/raisedEyebrow/abashed_lookUp_09.jpg", std::chrono::milliseconds(4833), 16.75885, -17.88387 },	//290
	{ "../textures/raisedEyebrow/upperRight_05.jpg", std::chrono::milliseconds(4866), 16.54533, -17.65602 },	//292
	{ "../textures/raisedEyebrow/upperRight_04.jpg", std::chrono::milliseconds(4933), 15.01593, -16.02395 },	//296
	{ "../textures/raisedEyebrow/upperRight_03.jpg", std::chrono::milliseconds(5000), 12.41397, -13.24732 },	//300
	{ "../textures/raisedEyebrow/upperRight_02.jpg", std::chrono::milliseconds(5066), 9.216137, -9.834815 },	//304
	{ "../textures/neutral/eyeOpen.jpg", std::chrono::milliseconds(5133), 5.899129, -6.295136 },	//308

	{ "../textures/neutral/eyeOpen.jpg", std::chrono::milliseconds(5333), 0, 0 },	//320
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

vector<keyframe> SmileAnim = {
	{ "../textures/neutral/eyeOpen.jpg", std::chrono::milliseconds(0),	robotX,	robotY },	//0
	{ "../textures/smile/smile_03.jpg", std::chrono::milliseconds(33),	robotX,	robotY },	//2
	{ "../textures/smile/smile_04.jpg", std::chrono::milliseconds(66),	robotX,	robotY },	//4
	{ "../textures/smile/smile_05.jpg", std::chrono::milliseconds(100), robotX,	robotY },	//6
	{ "../textures/smile/smile_06.jpg", std::chrono::milliseconds(133), robotX,	robotY },	//8
	{ "../textures/smile/smile_07.jpg", std::chrono::milliseconds(166), robotX,	robotY },	//10

	{ "../textures/smile/smile_07.jpg", std::chrono::milliseconds(466),  robotX, robotY },	//28
	{ "../textures/smile/smile_07.jpg", std::chrono::milliseconds(566),  robotX, robotY },	//34
	{ "../textures/smile/smile_07.jpg", std::chrono::milliseconds(666),  robotX, robotY },	//40
	{ "../textures/smile/smile_07.jpg", std::chrono::milliseconds(766),  robotX, robotY },	//46
	{ "../textures/smile/smile_07.jpg", std::chrono::milliseconds(866),  robotX, robotY },	//52
	{ "../textures/smile/smile_07.jpg", std::chrono::milliseconds(966),  robotX, robotY },	//58
	{ "../textures/smile/smile_07.jpg", std::chrono::milliseconds(1066), robotX, robotY },	//64
	{ "../textures/smile/smile_07.jpg", std::chrono::milliseconds(1166), robotX, robotY },	//70
	{ "../textures/smile/smile_07.jpg", std::chrono::milliseconds(1266), robotX, robotY },	//76
	{ "../textures/smile/smile_07.jpg", std::chrono::milliseconds(1366), robotX, robotY },	//82
	{ "../textures/smile/smile_07.jpg", std::chrono::milliseconds(1466), robotX, robotY },	//88
	{ "../textures/smile/smile_07.jpg", std::chrono::milliseconds(1550), robotX, robotY },	//93

	{ "../textures/smile/smile_07.jpg", std::chrono::milliseconds(1633),  robotX, robotY },	//98
	{ "../textures/smile/smile_06.jpg", std::chrono::milliseconds(1666),  robotX, robotY },	//100
	{ "../textures/smile/smile_05.jpg", std::chrono::milliseconds(1700),  robotX, robotY },	//102
	{ "../textures/smile/smile_04.jpg", std::chrono::milliseconds(1733),  robotX, robotY },	//104
	{ "../textures/smile/smile_03.jpg", std::chrono::milliseconds(1766),  robotX, robotY },	//106
	{ "../textures/smile/smile_02.jpg", std::chrono::milliseconds(1800),  robotX, robotY },	//108
	{ "../textures/neutral/eyeOpen.jpg", std::chrono::milliseconds(1833), robotX, robotY }	//110
};


vector<keyframe> HypnotizeAnim = {
	{ "../textures/hypnotize/twirl_02.jpg", std::chrono::milliseconds(33), robotX, robotY },	//0
	{ "../textures/hypnotize/twirl_03.jpg", std::chrono::milliseconds(66), robotX, robotY },	//2
	{ "../textures/hypnotize/twirl_04.jpg", std::chrono::milliseconds(100), robotX, robotY },	//4
	{ "../textures/hypnotize/twirl_05.jpg", std::chrono::milliseconds(133), robotX, robotY },	//6
	{ "../textures/hypnotize/twirl_06.jpg", std::chrono::milliseconds(166), robotX, robotY },	//8
	{ "../textures/hypnotize/twirl_07.jpg", std::chrono::milliseconds(200), robotX, robotY },	//10
	{ "../textures/hypnotize/twirl_08.jpg", std::chrono::milliseconds(233), robotX, robotY },	//12
	{ "../textures/hypnotize/twirl_09.jpg", std::chrono::milliseconds(266), robotX, robotY },	//14
	{ "../textures/hypnotize/twirl_10.jpg", std::chrono::milliseconds(300), robotX, robotY },	//16
	{ "../textures/hypnotize/twirl_11.jpg", std::chrono::milliseconds(333), robotX, robotY },	//18
	{ "../textures/hypnotize/twirl_12.jpg", std::chrono::milliseconds(366), robotX, robotY },	//20
	{ "../textures/hypnotize/twirl_13.jpg", std::chrono::milliseconds(400), robotX, robotY },	//22

	{ "../textures/hypnotize/twirl_02.jpg", std::chrono::milliseconds(433), robotX, robotY },	//24
	{ "../textures/hypnotize/twirl_03.jpg", std::chrono::milliseconds(466), robotX, robotY },	//26
	{ "../textures/hypnotize/twirl_04.jpg", std::chrono::milliseconds(500), robotX, robotY },	//28
	{ "../textures/hypnotize/twirl_05.jpg", std::chrono::milliseconds(533), robotX, robotY },	//30
	{ "../textures/hypnotize/twirl_06.jpg", std::chrono::milliseconds(566), robotX, robotY },	//32
	{ "../textures/hypnotize/twirl_07.jpg", std::chrono::milliseconds(600), robotX, robotY },	//34
	{ "../textures/hypnotize/twirl_08.jpg", std::chrono::milliseconds(633), robotX, robotY },	//36
	{ "../textures/hypnotize/twirl_09.jpg", std::chrono::milliseconds(666), robotX, robotY },	//38
	{ "../textures/hypnotize/twirl_10.jpg", std::chrono::milliseconds(700), robotX, robotY },	//40
	{ "../textures/hypnotize/twirl_11.jpg", std::chrono::milliseconds(733), robotX, robotY },	//42
	{ "../textures/hypnotize/twirl_12.jpg", std::chrono::milliseconds(766), robotX, robotY },	//44
	{ "../textures/hypnotize/twirl_13.jpg", std::chrono::milliseconds(800), robotX, robotY },	//46

	{ "../textures/hypnotize/twirl_02.jpg", std::chrono::milliseconds(833), robotX, robotY },	//48
	{ "../textures/hypnotize/twirl_03.jpg", std::chrono::milliseconds(866), robotX, robotY },	//50
	{ "../textures/hypnotize/twirl_04.jpg", std::chrono::milliseconds(900), robotX, robotY },	//52
	{ "../textures/hypnotize/twirl_05.jpg", std::chrono::milliseconds(933), robotX, robotY },	//54
	{ "../textures/hypnotize/twirl_06.jpg", std::chrono::milliseconds(966), robotX, robotY },	//56
	{ "../textures/hypnotize/twirl_07.jpg", std::chrono::milliseconds(1000), robotX, robotY },	//58
	{ "../textures/hypnotize/twirl_08.jpg", std::chrono::milliseconds(1033), robotX, robotY },	//60
	{ "../textures/hypnotize/twirl_09.jpg", std::chrono::milliseconds(1066), robotX, robotY },	//62
	{ "../textures/hypnotize/twirl_10.jpg", std::chrono::milliseconds(1100), robotX, robotY },	//64
	{ "../textures/hypnotize/twirl_11.jpg", std::chrono::milliseconds(1133), robotX, robotY },	//66
	{ "../textures/hypnotize/twirl_12.jpg", std::chrono::milliseconds(1166), robotX, robotY },	//68
	{ "../textures/hypnotize/twirl_13.jpg", std::chrono::milliseconds(1200), robotX, robotY },	//70

	{ "../textures/hypnotize/twirl_02.jpg", std::chrono::milliseconds(1233), robotX, robotY },	//72
	{ "../textures/hypnotize/twirl_03.jpg", std::chrono::milliseconds(1266), robotX, robotY },	//74
	{ "../textures/hypnotize/twirl_04.jpg", std::chrono::milliseconds(1300), robotX, robotY },	//76
	{ "../textures/hypnotize/twirl_05.jpg", std::chrono::milliseconds(1333), robotX, robotY },	//78
	{ "../textures/hypnotize/twirl_06.jpg", std::chrono::milliseconds(1366), robotX, robotY },	//80
	{ "../textures/hypnotize/twirl_07.jpg", std::chrono::milliseconds(1400), robotX, robotY },	//82
	{ "../textures/hypnotize/twirl_08.jpg", std::chrono::milliseconds(1433), robotX, robotY },	//84
	{ "../textures/hypnotize/twirl_09.jpg", std::chrono::milliseconds(1466), robotX, robotY },	//86
	{ "../textures/hypnotize/twirl_10.jpg", std::chrono::milliseconds(1500), robotX, robotY },	//88
	{ "../textures/hypnotize/twirl_11.jpg", std::chrono::milliseconds(1533), robotX, robotY },	//90
	{ "../textures/hypnotize/twirl_12.jpg", std::chrono::milliseconds(1566), robotX, robotY },	//92
	{ "../textures/hypnotize/twirl_13.jpg", std::chrono::milliseconds(1600), robotX, robotY },	//94
};

vector<keyframe> KissAnim = {
	{ "../textures/kiss/eyeToHeart_02.jpg", std::chrono::milliseconds(0), 0, 0 }, //0
	{ "../textures/kiss/eyeToHeart_03.jpg", std::chrono::milliseconds(66), 0, 0 }, //4
	{ "../textures/kiss/eyeToHeart_04.jpg", std::chrono::milliseconds(133), 0, 0 }, //8
	{ "../textures/kiss/eyeToHeart_05.jpg", std::chrono::milliseconds(200), 0, 0 }, //12
	{ "../textures/kiss/eyeToHeart_04.jpg", std::chrono::milliseconds(266), 0, 0 }, //16
	{ "../textures/kiss/eyeToHeart_03.jpg", std::chrono::milliseconds(333), 0, 0 }, //20
	{ "../textures/kiss/eyeToHeart_02.jpg", std::chrono::milliseconds(400), 0, 0 }, //24
	{ "../textures/kiss/eyeToHeart_01.jpg", std::chrono::milliseconds(466), 0, 0 }, //28
	{ "../textures/kiss/eyeToHeart_02.jpg", std::chrono::milliseconds(533), 0, 0 }, //32
	{ "../textures/kiss/eyeToHeart_03.jpg", std::chrono::milliseconds(600), 0, 0 }, //36
	{ "../textures/kiss/eyeToHeart_04.jpg", std::chrono::milliseconds(666), 0, 0 }, //40
	{ "../textures/kiss/eyeToHeart_05.jpg", std::chrono::milliseconds(733), 0, 0 }, //44
	{ "../textures/kiss/eyeToHeart_04.jpg", std::chrono::milliseconds(800), 0, 0 }, //48
	{ "../textures/kiss/eyeToHeart_03.jpg", std::chrono::milliseconds(866), 0, 0 }, //52
	{ "../textures/kiss/eyeToHeart_02.jpg", std::chrono::milliseconds(933), 0, 0 }, //56
	{ "../textures/kiss/eyeToHeart_01.jpg", std::chrono::milliseconds(1000), 0, 0 }, //60
	{ "../textures/kiss/eyeToHeart_02.jpg", std::chrono::milliseconds(1066), 0, 0 }, //64
	{ "../textures/kiss/eyeToHeart_03.jpg", std::chrono::milliseconds(1133), 0, 0 }, //68
	{ "../textures/kiss/eyeToHeart_04.jpg", std::chrono::milliseconds(1200), 0, 0 }, //72
	{ "../textures/kiss/eyeToHeart_05.jpg", std::chrono::milliseconds(1266), 0, 0 }, //76
	{ "../textures/kiss/eyeToHeart_04.jpg", std::chrono::milliseconds(1333), 0, 0 }, //80
	{ "../textures/kiss/eyeToHeart_03.jpg", std::chrono::milliseconds(1400), 0, 0 }, //84
	{ "../textures/kiss/eyeToHeart_02.jpg", std::chrono::milliseconds(1466), 0, 0 }, //88
	{ "../textures/kiss/eyeToHeart_01.jpg", std::chrono::milliseconds(1533), 0, 0 }, //92
	{ "../textures/kiss/eyeToHeart_02.jpg", std::chrono::milliseconds(1600), 0, 0 }, //96
	{ "../textures/kiss/eyeToHeart_03.jpg", std::chrono::milliseconds(1666), 0, 0 }, //100
	{ "../textures/kiss/eyeToHeart_04.jpg", std::chrono::milliseconds(1733), 0, 0 }, //104
	{ "../textures/kiss/eyeToHeart_05.jpg", std::chrono::milliseconds(1800), 0, 0 }, //108
	{ "../textures/kiss/eyeToHeart_04.jpg", std::chrono::milliseconds(1866), 0, 0 }, //112
	{ "../textures/kiss/eyeToHeart_03.jpg", std::chrono::milliseconds(1933), 0, 0 },		//116

	{ "../textures/kiss/eyeToHeart_02.jpg", std::chrono::milliseconds(2000), 3.3187, 0 }, //120
	{ "../textures/kiss/eyeToHeart_02.jpg", std::chrono::milliseconds(2066), 6.63739, 0 }, //124
	{ "../textures/kiss/eyeToHeart_01.jpg", std::chrono::milliseconds(2100), 5.718095, -2.014473 }, //126
	{ "../textures/kiss/eyeToHeart_01.jpg", std::chrono::milliseconds(2166), 3.091522, -7.770142 }, //130
	{ "../textures/kiss/eyeToHeart_01.jpg", std::chrono::milliseconds(2200), 1.105374, -8.773625 }, //132
	{ "../textures/kiss/eyeToHeart_01.jpg", std::chrono::milliseconds(2266), -4.569336, -11.64072 }, //136
	{ "../textures/kiss/eyeToHeart_02.jpg", std::chrono::milliseconds(2300), -5.427465, -11.55955 }, //138
	{ "../textures/kiss/eyeToHeart_03.jpg", std::chrono::milliseconds(2400), -13.15066, -10.82897 }, //144
	{ "../textures/kiss/eyeToHeart_03.jpg", std::chrono::milliseconds(2466), -16.15411, -10.54486 }, //148
	{ "../textures/kiss/eyeToHeart_04.jpg", std::chrono::milliseconds(2500), -16.9875, -8.892462 }, //150
	{ "../textures/kiss/eyeToHeart_05.jpg", std::chrono::milliseconds(2600), -21.48778, 0.03048706 }, //156
	{ "../textures/kiss/eyeToHeart_04.jpg", std::chrono::milliseconds(2700), -16.98749, 8.901994 }, //162
	{ "../textures/kiss/eyeToHeart_04.jpg", std::chrono::milliseconds(2733), -16.15411, 10.54486 }, //164
	{ "../textures/kiss/eyeToHeart_03.jpg", std::chrono::milliseconds(2800), -10.36172, 11.09279 }, //168
	{ "../textures/kiss/eyeToHeart_03.jpg", std::chrono::milliseconds(2866), -4.569336, 11.64072 }, //172
	{ "../textures/kiss/eyeToHeart_02.jpg", std::chrono::milliseconds(2900), -2.583171, 10.63723 }, //174
	{ "../textures/kiss/eyeToHeart_02.jpg", std::chrono::milliseconds(2966), 3.091522, 7.770142 }, //178
	{ "../textures/neutral/eyeOpen.jpg", std::chrono::milliseconds(3000), 2.290016, 5.755661 }, //180
	{ "../textures/neutral/eyeOpen.jpg", std::chrono::milliseconds(3066), 0, 0 }, //184
};

vector<keyframe> AngryAnim = {
	{ "../textures/tongue/shock_02.jpg", std::chrono::milliseconds(0), -1.996765, 0 }, //0
	{ "../textures/tongue/shock_03.jpg", std::chrono::milliseconds(33), -2.203956, 0 }, //2
	{ "../textures/tongue/shock_04.jpg", std::chrono::milliseconds(66), -2.588738, 0 }, //4
	{ "../textures/tongue/shock_05.jpg", std::chrono::milliseconds(100), -2.795929, 0 }, //6
	{ "../textures/tongue/shock_05.jpg", std::chrono::milliseconds(300), -1.996765, 0 }, //18
	{ "../textures/tongue/shock_05.jpg", std::chrono::milliseconds(633), -1.996765, 0 }, //38

	{ "../textures/tongue/angry_03.jpg", std::chrono::milliseconds(700), -1.525029, 0 }, //42
	{ "../textures/tongue/angry_04.jpg", std::chrono::milliseconds(766), -0.4001213, 0 }, //46
	{ "../textures/tongue/angry_05.jpg", std::chrono::milliseconds(966), 2.539154, 0 }, //58

	{ "../textures/tongue/rageAway_02.jpg", std::chrono::milliseconds(2733), -0.3805258, -17.55855 }, //164
	{ "../textures/tongue/rageAway_03.jpg", std::chrono::milliseconds(2766), -1.608116, -24.94111 }, //166
	{ "../textures/tongue/rageAway_04.jpg", std::chrono::milliseconds(2800), -2.835706, -32.32367 }, //168
	{ "../textures/tongue/rageAway_05.jpg", std::chrono::milliseconds(2833), -3.963762, -39.10765 }, //170
	{ "../textures/tongue/rageAway_06.jpg", std::chrono::milliseconds(2866), -4.89275, -44.69445 }, //172
	{ "../textures/tongue/rageAway_07.jpg", std::chrono::milliseconds(2900), -5.523139, -48.48553 }, //174
	{ "../textures/tongue/rageAway_08.jpg", std::chrono::milliseconds(2933), -5.755386, -49.88223 }, //176

	{ "../textures/tongue/rageAway_08.jpg", std::chrono::milliseconds(5000), -5.755386, -49.88223 }, //300
	{ "../textures/tongue/upperLeft_05.jpg", std::chrono::milliseconds(5066), -5.535151, -47.97345 }, //304
	{ "../textures/tongue/upperLeft_04.jpg", std::chrono::milliseconds(5166), -4.554641, -39.47531 }, //310
	{ "../textures/tongue/upperLeft_03.jpg", std::chrono::milliseconds(5266), -3.131311, -27.13923 }, //316
	{ "../textures/tongue/upperLeft_02.jpg", std::chrono::milliseconds(5366), -1.644729, -14.25494 }, //322
	{ "../textures/neutral/eyeOpen.jpg", std::chrono::milliseconds(5466), -0.474442, -4.112 }, //328
	{ "../textures/neutral/eyeOpen.jpg", std::chrono::milliseconds(5566), 0, 0 }, //334
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
	static FMOD::System *system;
	static FMOD::Sound		*sound;
	FMOD::Channel	*channel = 0;
	FMOD_RESULT		result;
	unsigned int	version;
	static void			*extradriverdata = 0;

	
	Common_Init(&extradriverdata);
	FMOD::System_Create(&system);
	system->init(32, FMOD_INIT_NORMAL, extradriverdata);
	result = system->createSound(Common_MediaPath(filename.c_str()), FMOD_DEFAULT, 0, &sound);
	
	cout << filename << endl;
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

	//mSenseMgr->EnableEmotion();
	cout << "initialized realSense" << endl;

	mSenseMgr->Init();
}


int supriseThreshhold, abashedThreshhold, winkThreshhold, smileThreshhold, hypnoThreshhold, kissThreshhold, tongueThreshhold;


int main()
{
	SendSignal mySignal;
	mySignal.setup();

	glfwInit();
	//GLFWwindow *win = glfwCreateWindow(800, 480, "Robot Face", nullptr, nullptr);
	GLFWwindow *win = glfwCreateWindow(800, 480, "Robot Face", glfwGetPrimaryMonitor(), nullptr); //fullscreen

	std::chrono::system_clock::time_point t0 = std::chrono::system_clock::now();
	

	MidiPlayer player; // Put it outside the loop
	
	

#pragma region loadData

	PXCFaceData::ExpressionsData::FaceExpressionResult raiseLeftBrow, raiseRightBrow, lowerLeftBrow, lowerRightBrow, openMouth, leftEyeClose, rightEyeClose, smile, kiss, tongue;
	

	
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
			mySignal.fdata->Update();

			int numFaces = mySignal.fdata->QueryNumberOfDetectedFaces();

			int closestface = -1;
			iSeeYou = false;
			int lowestdisparity = INT_MAX;
			int targetX, targetY;
#pragma region Face Tracking
			for (int i = 0; i < numFaces; ++i)
			{

				auto face = mySignal.fdata->QueryFaceByID(i);
				if (face)
				{
					PXCRectI32 faceBox;
					face->QueryDetection()->QueryBoundingRect(&faceBox);
					if (faceBox.x > -1 && faceBox.y > -1)
					{

						iSeeYou = true;

						int faceX = Map(faceBox.x, 1400, 0, 0, 80); //Camera min/max : 1400, 0    Robot Min/Max : 50 / 130 
						//int faceY = Map(faceBox.y, 0, 320, 0, 60); // Camera min/max:  0,320   Robot min/max:  40,100
						int faceY = Map(faceBox.y, 0, 700, 0, 60);
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
				
				player.SendMidiMessage(1, robotX, robotY);

				//cout << robotX << ", " << robotY << "       emotion: " << showingEmotion << ", neutral:" << neutral << endl;
				//cout << closestface << " " << numFaces << "      " <<  targetX << ", " << targetY << "      " << robotX << " " << robotY << "     " << showingEmotion << endl;
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
					edata->QueryExpression(PXCFaceData::ExpressionsData::EXPRESSION_BROW_LOWERER_LEFT, &lowerLeftBrow);
					edata->QueryExpression(PXCFaceData::ExpressionsData::EXPRESSION_BROW_LOWERER_RIGHT, &lowerRightBrow);
					
					//Mouth
					edata->QueryExpression(PXCFaceData::ExpressionsData::EXPRESSION_MOUTH_OPEN, &openMouth);
					edata->QueryExpression(PXCFaceData::ExpressionsData::EXPRESSION_SMILE, &smile);
					edata->QueryExpression(PXCFaceData::ExpressionsData::EXPRESSION_KISS, &kiss);
					edata->QueryExpression(PXCFaceData::ExpressionsData::EXPRESSION_TONGUE_OUT, &tongue);

					//Eyes
					edata->QueryExpression(PXCFaceData::ExpressionsData::EXPRESSION_EYES_CLOSED_LEFT, &leftEyeClose);
					edata->QueryExpression(PXCFaceData::ExpressionsData::EXPRESSION_EYES_CLOSED_RIGHT, &rightEyeClose);

					

					

					

#pragma endregion

#pragma region Sending Commands

					//suprise
					if (raiseLeftBrow.intensity > 70 && raiseRightBrow.intensity > 70 && openMouth.intensity > 30)
					{
						supriseThreshhold++;
						if (supriseThreshhold > 5)
							if (neutral && showingEmotion == false)
							{
								mySignal.start_animation(SupriseAnim);
								showingEmotion = true;
								supriseThreshhold = 0;
							}
					}
					//hypnotize is a bad one lots of false negatives
					/*if (raiseLeftBrow.intensity > 90 && raiseRightBrow.intensity > 90 && openMouth.intensity < 10)
					{
						hypnoThreshhold++;
						if (hypnoThreshhold > 20)
							if (neutral&&showingEmotion == false)
							{
								mySignal.start_animation(HypnotizeAnim);
								showingEmotion = true;
								hypnoThreshhold = 0;
							}
					}*/
					//abashed
					if (lowerLeftBrow.intensity > 80 && lowerRightBrow.intensity > 80)
					{
						abashedThreshhold++;
						if (abashedThreshhold > 40);
							if (neutral && showingEmotion == false)
							{
								mySignal.start_animation(AbashedAnim);
								showingEmotion = true;
								abashedThreshhold = 0;
							}
					}
					//Wink
					if (leftEyeClose.intensity > 80 && rightEyeClose.intensity < 80 || leftEyeClose.intensity < 80 && rightEyeClose.intensity > 80)
					{
						winkThreshhold++;
						if (winkThreshhold > 15)
							if (neutral && showingEmotion == false)
							{
								mySignal.start_animation(WinkAnim);
								showingEmotion = true;
								winkThreshhold = 0;
							}
					}

					//Smile
					if (smile.intensity > 50)
					{
						smileThreshhold++;
						if (smileThreshhold > 10)
							if (neutral&& showingEmotion == false)
							{
								mySignal.start_animation(SmileAnim);
								showingEmotion = true;
								smileThreshhold = 0;
							}
					}
					//kiss
					if (kiss.intensity > 80)
					{
						kissThreshhold++;
						if (kissThreshhold > 12)
							if (neutral && showingEmotion == false)
							{
								mySignal.start_animation(KissAnim);
								showingEmotion = true;
								kissThreshhold = 0;
							}
					}
					//Tongue don't use lots of false posatives and anim reversed
					/*if (tongue.intensity > 70)
					{
						tongueThreshhold++;
						if (tongueThreshhold > 15)
							if (neutral && showingEmotion == false)
							{
								mySignal.start_animation(AngryAnim);
								showingEmotion = true;
								tongueThreshhold = 0;
							}
					}*/

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

		int state = glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT);
		if (state == GLFW_PRESS)
			exit(EXIT_SUCCESS);


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
	neutral = false;
	current_animation = &anim;
	time_anim_started = std::chrono::system_clock::now();
	if (&anim == &BlinkAnim)
		AudioPlay("blink.wav");
	if (&anim == &SupriseAnim)
		AudioPlay("suprise.wav");
	if (&anim == &AbashedAnim)
		AudioPlay("sad.wav");
	if (&anim == &WinkAnim)
		AudioPlay("wink.wav");
	if (&anim == &SmileAnim)
		AudioPlay("happy.wav");
	if (&anim == &HypnotizeAnim)
		AudioPlay("hypnotize.wav");
	if (&anim == &KissAnim)
		AudioPlay("kiss.wav");
	if (&anim == &AngryAnim)
		AudioPlay("angry.wav");
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
					robotX = Map(p.y, -35, 35, 0, 80);
					robotY = Map(p.x, 45, -45, 80, 20);
					
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