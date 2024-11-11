#pragma once

class Subject;
class Observer
{
public:
	virtual ~Observer() = 0;
	virtual void notify(Subject* sub) = 0;
};
inline Observer::~Observer() {}