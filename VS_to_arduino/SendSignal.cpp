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

	int headTurnModifier;

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

//face expressions-- Anger, Disgust, Fear, Joy, Sadness, Surprise
//use expression configuration interface
//https://software.intel.com/sites/landingpage/realsense/camera-sdk/v1.1/documentation/html/expressionsconfiguration_pxcfaceconfiguration.html
//https://software.intel.com/sites/landingpage/realsense/camera-sdk/v1.1/documentation/html/queryexpressions_face_pxcfacedata.html

void SendSignal::update()
{
	MidiPlayer player;

	while(true)
	{
		int myx;
		int mychannel;

		cout << "channel? " << endl;
		cin >> mychannel;
		cout << "value?  10-120 for x, 130 - 254 for y " << endl;
		cin >> myx;
		player.SendMidiMessage(mychannel, 10, myx);

		//send 10 - 120
		
	}

	/*if (mSenseMgr->AcquireFrame(true) >= PXC_STATUS_NO_ERROR)
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
							int myx;
							int myy;

							cout << "My X: " << outR.x << " My Y: " << outR.y << endl;
							
							cout << "x coordinate? " << endl;
							cin >> myx;
							player.SendMidiMessage(0, myx, 10);

							cout << "y coordinate? " << endl;
							cin >> myy;
							player.SendMidiMessage(1, myy, 10);



							//player.SendMidiMessage(0, lightbulb, 10);
							//std::this_thread::sleep_for(std::chrono::milliseconds(7));
						}
					}
				}
			}
		}
		mSenseMgr->ReleaseFrame();
	}*/
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