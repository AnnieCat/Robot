#include "Midi.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <queue>

#include <memory>

using namespace std;

int main(){

	MidiPlayer player;

	while (1)
	{
		cout << "on or off? 0-1" << endl;
		int lightbulb;
		cin >> lightbulb;

		player.SendMidiMessage(1, lightbulb, 10);

		std::this_thread::sleep_for(std::chrono::milliseconds(7));
	}
}