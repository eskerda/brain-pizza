#pragma once

#include "constants.h"

#include "Sprite.h"

#include <assert.h>

#include <vector>
#include <string>
#include <map>

class AnimatedSprite : public Sprite
{
public:
	explicit AnimatedSprite(const std::string& name);
	virtual ~AnimatedSprite() {}

	void SetAnimation(std::string name) {
		assert(animationNames.find(name) != animationNames.end());
		int newrow = animationNames[name];
		finished = false;
		SetAnimationById(newrow);
	}
	void SetAnimationById(unsigned int number) {
		if (row != number) {
			row = number;
			column = 0;
			time = 0;
		}
		UpdateRect();
	}
    const std::map<std::string, int>& GetAnimationNames() const { return animationNames; }
    int GetTotalAnimations() const { return animationNames.size(); }
    int GetAnimationId() const { return row; }
	int GetAnimationFrame() const { return column; }
    void SetAnimationFrame(int frame) { column = frame; time = 0; UpdateRect(); }
    int GetTotalAnimationFrames() { return frameTimes[row].size(); }
    float GetAnimationFrameTime() { return time; }
	void AddTime(float elapsedTime);
    void SetRandomFrameAndTime() { column = RandInt(0,frameTimes[row].size()-1); time = frameTimes[row][column]*RandFloat(); }
	void Reset();
	bool isFinished() const { return finished; }
	void SetRepeat(bool b) { repeat = b; }

	virtual void OriginToCenter() override {
		setOrigin(width/2,height/2);
	}

private:
	void UpdateRect() { setTextureRect(sf::IntRect(column*width,row*height,width,height)); }

	std::map<std::string, int> animationNames;
	std::vector< std::vector<float> > frameTimes;
	int width, height;
	unsigned int row, column;
	float time;
	bool finished, repeat;
};


