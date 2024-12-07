#pragma once
#include <vector>
#include <algorithm>
#include "Observer.h"
class Subject {
public:
    virtual ~Subject() = 0;
    // Attach an observer
    void attach(Observer* observer) {
        observers.push_back(observer);
        observer->notify(this);
    }

    // Detach an observer
    void detach(Observer* observer) {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    // Notify all observers
    void notifyAll() {
        for (Observer* observer : observers) {
            observer->notify(this);  
        }
    }

private:
    std::vector<Observer*> observers; 
};
inline Subject::~Subject() {}