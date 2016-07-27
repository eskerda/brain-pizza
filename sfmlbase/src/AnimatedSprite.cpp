#include "AnimatedSprite.h"

#include "helpers/filehelpers.h"

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

AnimatedSprite::AnimatedSprite(const string& filePath) :
	Sprite(),
	row(0),
	column(0),
	time(0.0f),
	finished(false),
	repeat(true)
{

	ifstream file(filePath);
	//if (file.fail()) throw string("Animation " + filePath + " not found");

	//Load tileset image
	string spritesheet;
	file >> spritesheet;
	Load(DirnameOf(filePath) + spritesheet);

	file >> width >> height;
	
	string animationName;
	while (file >> animationName) {
		string s;
		getline(file,s);
		stringstream ss(s);

		int id = frameTimes.size();
		animationNames[animationName] = id;
		frameTimes.push_back(std::vector<float>());
		int ms;

		while(ss >> ms) frameTimes[id].push_back(ms/1000.f);

		//Log::d(animationName);
	}

	UpdateRect();
}

void AnimatedSprite::AddTime(float elapsedTime) {
	time += elapsedTime;
    while (time > frameTimes[row][column]) {
        time -= frameTimes[row][column];
		column++;
        if (column >= frameTimes[row].size()) {
            finished = true;
            if(repeat) {
                column = 0;
            } else {
                column--;
            }
        }
        UpdateRect();
    }
}

void AnimatedSprite::Reset()
{
    time = 0;
	column = 0;
	finished = false;
}

