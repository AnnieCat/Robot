#include "Midi.h"
#include <chrono>
#include <thread>

#include <iostream>
#include <queue>
#include <memory>
#include <string>

#include "pxcsensemanager.h"
#include "pxcfaceconfiguration.h"

using namespace std;


class SendSignal
{
public:
	//Constructor and Destructor
	SendSignal(){	}
	~SendSignal(){	}
	void setup();
	void update();
	void cleanup();


private:
	PXCSenseManager *mSenseMgr;
	PXCFaceData *mFaceData;
};

void SendSignal::setup()
{


	mSenseMgr = PXCSenseManager::CreateInstance();
	auto st = mSenseMgr->EnableFace();
	auto fm = mSenseMgr->QueryFace();
	auto cfg = fm->CreateActiveConfiguration();
	st = mSenseMgr->Init();

	cfg->pose.isEnabled = false;
	cfg->ApplyChanges();
	mFaceData = fm->CreateOutput();
}

int Map(int value, int low1, int high1, int low2, int high2)
{
	return low2 + (value - low1) * (high2 - low2) / (high1 - low1);
}

//face expressions-- Anger, Disgust, Fear, Joy, Sadness, Surprise
//use expression configuration interface
//https://software.intel.com/sites/landingpage/realsense/camera-sdk/v1.1/documentation/html/expressionsconfiguration_pxcfaceconfiguration.html
//https://software.intel.com/sites/landingpage/realsense/camera-sdk/v1.1/documentation/html/queryexpressions_face_pxcfacedata.html

void SendSignal::update()
{
	// ---- Send explicit X Y coordinates
		
	MidiPlayer player;

	/*while(true)
	{
		int myx;
		int mychannel;

		cout << "channel? X is 1, Y is 2" << endl;
		cin >> mychannel;
		cout << "value?  10-120 for x, 130 - 254 for y " << endl;
		cin >> myx;
		player.SendMidiMessage(mychannel, 0, myx);

		//send 10 - 120
		
	}*/

	if (mSenseMgr->AcquireFrame(true) >= PXC_STATUS_NO_ERROR)
	{
		if (!mFaceData)
			mFaceData = mSenseMgr->QueryFace()->CreateOutput();
		if (mFaceData)
		{
			mFaceData->Update();
			auto numFaces = mFaceData->QueryNumberOfDetectedFaces();
			
			for (int i = 0; i < numFaces; ++i)
			{
				//track up to 4 faces -- only 1 face has landmarks
				auto foundFace = mFaceData->QueryFaceByIndex(i);
				if (foundFace)
				{
					auto dd = foundFace->QueryDetection();
					if (dd)
					{
						PXCRectI32 outR;
						if (dd->QueryBoundingRect(&outR))
						{
							MidiPlayer player;

							int myX, myY;
							
							// for the servo's extent extend last 2 digits to 180 and 10
							myX = Map(outR.x, 0, 1500, 130, 60);
							myY = Map(outR.y, 0, 730, 40, 80);

							cout << "Rotation Value: " << myX << endl;
							if (myX >= 60 && myX <= 130)
							{
								float temp = myX / 126;

								if (temp < 1)
								{
									player.SendMidiMessage(1, 0, myX);
									cout << "0 - " << myX << endl;
									cout << "rotation: " << myX << endl;
								}
								else
								{
									int modulo = myX % 126;
									player.SendMidiMessage(1, modulo, 126);
									cout << "1 - " << modulo << endl;
									cout << "rotation: " << myX << endl;
								}
							}
							
							if (myY >= 40 && myY <= 80)
							{
								float temp = myY / 126;

								if (temp < 1)
								{
									player.SendMidiMessage(2, 0, myY);
									cout << "0 - " << myY << endl;
									cout << "rotation: " << myY << endl;
								}
								else
								{
									int modulo = myY % 126;
									player.SendMidiMessage(2, modulo, 126);
									cout << "1 - " << modulo << endl;
									cout << "rotation: " << myY << endl;
								}
							}

							std::this_thread::sleep_for(std::chrono::milliseconds(50));}
					}
				}
			}
		}
		mSenseMgr->ReleaseFrame();
	}
}



void SendSignal::cleanup()
{
	if (mFaceData)
		mFaceData->Release();
	mSenseMgr->Close();
}

int main()
{
	SendSignal mySendSignal;

	mySendSignal.setup();

	while (true)
		mySendSignal.update();
	mySendSignal.cleanup();
}

// C:\Program Files (x86)\Intel\RSSDK\lib\$(Platform)
// $(GrapheneTrimpin)\third_party\lib\$(Platform)\

//  Sample Code -- C++ talking to Arduino

/*int main(){

	MidiPlayer player;

	while (1)
	{
		cout << "on or off? 0-1" << endl;
		int lightbulb;
		cin >> lightbulb;

		player.SendMidiMessage(1, lightbulb, 10);

		std::this_thread::sleep_for(std::chrono::milliseconds(7));
	}
}*/