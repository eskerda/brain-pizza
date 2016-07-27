#pragma once

//This is not the best thing in the world (see http://yosefk.com/c++fqa/heap.html#fqa-16.15 )
//But Unity has this feature, and it's relly handy
class SelfDestroy {
public:
	SelfDestroy() { destructionTimer = -2; }
	void Destroy(float time) { destructionTimer = time; }
	void Update(float dt) {
		if (destructionTimer < -1) return;
		destructionTimer -= dt;
		if (destructionTimer < 0) delete this;
	}
private:
	float destructionTimer;
	~SelfDestroy() { } //We will handle the deletion, also prevents allocation without a new
};
