#pragma once

class noncopyable {
protected:
    noncopyable() {}
    ~noncopyable() {}
private:
	noncopyable(const noncopyable&);
	void operator=(const noncopyable&);

};
