// C:\Program Files (x86)\Intel\RSSDK\lib\$(Platform)
// $(GrapheneTrimpin)\third_party\lib\$(Platform)\

#include "SendSignal.h"

#include "Midi.h"



using namespace std;

int width, height;
stbi_uc * myImage = stbi_load("textures/neutral/eyeOpen.jpg", &width, &height, nullptr, 3);

SendSignal::SendSignal()
{
	BlinkAnim = {
		{ "textures/neutral/eyeOpen.jpg", 0 },
		{ "textures/wink/wink_03.jpg", 2 },
		{ "textures/wink/wink_04.jpg", 2 },
		{ "textures/wink/wink_05.jpg", 2 },
		{ "textures/wink/wink_06.jpg", 2 },
		{ "textures/wink/wink_05.jpg", 2 },
		{ "textures/wink/wink_04.jpg", 2 },
		{ "textures/wink/wink_03.jpg", 2 },
		{ "textures/wink/wink_02.jpg", 2 },
		{ "textures/neutral/eyeOpen.jpg", 2 }
	};
}

void SendSignal::setup()
{
	mSenseMgr = PXCSenseManager::CreateInstance();

	if (!mSenseMgr)
		cout << "failed to create SDK Sense Manager" << endl;

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
	cout << "initialized" << endl;

	mSenseMgr->Init();	
}

int Map(int value, int low1, int high1, int low2, int high2)
{
	return low2 + (value - low1) * (high2 - low2) / (high1 - low1);
}

void SendSignal::testUpdate()
{
	MidiPlayer player;
	int num;
	
	cout << "10 or 20?" << endl;
	cin >> num;
	player.SendMidiMessage(1, 10, num);
}

void SendSignal::update()
{	
	MidiPlayer player;
	int numFaces = 0;

	int num;

	cout << "number?" << endl;
	cin >> num;
	player.SendMidiMessage(1, 10, 10);


	if (mSenseMgr->AcquireFrame(true) >= PXC_STATUS_NO_ERROR)
	{
		PXCEmotion *emotionDet = mSenseMgr->QueryEmotion();
		PXCEmotion::EmotionData arrData[10];

		fdata->Update();

		int numFaces = fdata->QueryNumberOfDetectedFaces();

		for (int i = 0; i < numFaces; ++i)
		{
			PXCFaceData::Face *face = fdata->QueryFaceByIndex(i);
			PXCFaceData::ExpressionsData *edata = face->QueryExpressions();

			emotionDet->QueryAllEmotionData(i, &arrData[0]);

# pragma region Face Tracking

			if (arrData->rectangle.x > -1 && arrData->rectangle.y > -1)
			{
				int myX, myY;
				iSeeYou = true;

				myX = Map(arrData->rectangle.x, 500, 0, 0, 80);
				myY = Map(arrData->rectangle.y, 0, 320, 0, 40);
				
				//player.SendMidiMessage(1, myX + 60, 10);
				//player.SendMidiMessage(2, myY + 40, 10);

				cout << "X: " << myX << endl;
				cout << "Y: " << myY << endl;
				 
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
			}
			else
				iSeeYou = false;

# pragma endregion 

# pragma region Expression Logic
			
			if (iSeeYou)
			{
				//PXCFaceData::ExpressionsData::FaceExpressionResult smileScore;
				//edata->QueryExpression(PXCFaceData::ExpressionsData::EXPRESSION_SMILE, &smileScore);

				PXCFaceData::ExpressionsData::FaceExpressionResult raiseLeftBrow;
				edata->QueryExpression(PXCFaceData::ExpressionsData::EXPRESSION_BROW_RAISER_LEFT, &raiseLeftBrow);
				PXCFaceData::ExpressionsData::FaceExpressionResult raiseRightBrow;
				edata->QueryExpression(PXCFaceData::ExpressionsData::EXPRESSION_BROW_RAISER_RIGHT, &raiseRightBrow);

				//PXCFaceData::ExpressionsData::FaceExpressionResult eyeClosedLeft;
				//edata->QueryExpression(PXCFaceData::ExpressionsData::EXPRESSION_EYES_CLOSED_LEFT, &eyeClosedLeft);
				//PXCFaceData::ExpressionsData::FaceExpressionResult eyeClosedRight;
				//edata->QueryExpression(PXCFaceData::ExpressionsData::EXPRESSION_EYES_CLOSED_RIGHT, &eyeClosedRight);

				//PXCFaceData::ExpressionsData::FaceExpressionResult kiss;
				//edata->QueryExpression(PXCFaceData::ExpressionsData::EXPRESSION_KISS, &kiss);

				PXCFaceData::ExpressionsData::FaceExpressionResult openMouth;
				edata->QueryExpression(PXCFaceData::ExpressionsData::EXPRESSION_MOUTH_OPEN, &openMouth);

				//PXCFaceData::ExpressionsData::FaceExpressionResult tongueOut;
				//edata->QueryExpression(PXCFaceData::ExpressionsData::EXPRESSION_TONGUE_OUT, &tongueOut);

				if (raiseLeftBrow.intensity > 80 && raiseRightBrow.intensity > 80 && openMouth.intensity > 30)
					cout << "Suprised" << endl;

				/*if (smileScore.intensity > 80)
					cout << "smile back!" << endl;

				if (raiseLeftBrow.intensity > 80 && raiseRightBrow.intensity > 80)
					cout << "eyebrows up" << endl;
				if (raiseLeftBrow.intensity > 80 && raiseRightBrow.intensity < 80)
					cout << "eyebrow raised" << endl;
				if (raiseLeftBrow.intensity < 80 && raiseRightBrow.intensity > 80)
					cout << "eyebrow raised" << endl;

				if (eyeClosedLeft.intensity > 80 && eyeClosedRight.intensity > 80)
					cout << "eyes Closed" << endl;
				//else
				// eyes open

				if (kiss.intensity > 80)
					cout << "kissy face!" << endl;

				if (openMouth.intensity > 80)
					cout << "say Ahhhhh" << endl;

				if (tongueOut.intensity > 80)
					cout << "Stick Tongue Out" << endl;*/ 
			}

# pragma endregion

		/*if (!mFaceData)
			mFaceData = mSenseMgr->QueryFace()->CreateOutput();
		if (mFaceData)
		{
			mFaceData->Update();
			auto numFaces = mFaceData->QueryNumberOfDetectedFaces();
#pragma region "Faces"	
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
							cout << "found face" << endl;
							MidiPlayer player;

							int myX, myY;
							
							myX = Map(outR.x, 0, 1500, 130, 60);
							myY = Map(outR.y, 0, 730, 40, 80);

							cout << "Rotation Value: " << myX << endl;
							if (myX >= 60 && myX <= 130)
							{
								float temp = myX / 126;

								if (temp < 1)
								{
									//player.SendMidiMessage(1, 0, myX);
									cout << "0 - " << myX << endl;
									cout << "rotation: " << myX << endl;
								}
								else
								{
									int modulo = myX % 126;
									//player.SendMidiMessage(1, modulo, 126);
									cout << "1 - " << modulo << endl;
									cout << "rotation: " << myX << endl;
								}
							}
							
							if (myY >= 40 && myY <= 80)
							{
								float temp = myY / 126;

								if (temp < 1)
								{
									//player.SendMidiMessage(2, 0, myY);
									cout << "0 - " << myY << endl;
									cout << "rotation: " << myY << endl;
								}
								else
								{
									int modulo = myY % 126;
									//player.SendMidiMessage(2, modulo, 126);
									cout << "1 - " << modulo << endl;
									cout << "rotation: " << myY << endl;
								}
							}


							std::this_thread::sleep_for(std::chrono::milliseconds(50));
						}
					}
					PXCEmotion::EmotionData emoData[10];
					auto emo = mSenseMgr->QueryEmotion();
				//}
			//}*/

			}

		mSenseMgr->ReleaseFrame();
	}
}

void SendSignal::cleanup()
{
	if(fdata) fdata->Release();
	if(facec) facec->Release();
	faceModule = nullptr;
	mSenseMgr->Close();
	mSenseMgr->Release();
}



int main()
{
	// Set up our SendSignal instance
	SendSignal mySendSignal;
	mySendSignal.setup();

	// Open up a GLFW window
	glfwInit();
	GLFWwindow * win = glfwCreateWindow(800, 480, "Robot Face", nullptr, nullptr);
	//glfwSetWindowUserPointer(win, this); 
	//glfwSetMouseButtonCallback(win, on_mouse);

	while (!glfwWindowShouldClose(win))
	{
		// Handle all input (user input, camera feed, etc.)
		glfwPollEvents();
		mySendSignal.updateTimer();
		// TODO: Could call SendSignal::update(), etc...

		// Dispatch all output (rendering, commands sent out to arduino, etc.)
		int width, height;
		glfwGetFramebufferSize(win, &width, &height);
		glfwMakeContextCurrent(win);

		glViewport(0, 0, width, height);
		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		// Coordinate System
		glLoadIdentity();
		glOrtho(0, width, height, 0, -1, +1);

		mySendSignal.drawFrame(width, height);

		glfwSwapBuffers(win);
	}

	glfwTerminate();
	mySendSignal.cleanup();

	//while (true)
	//mySendSignal.testUpdate();
	//mySendSignal.update();
	//mySendSignal.timer();
}