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
    }

    // Detach an observer
    void detach(Observer* observer) {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    // Notify all observers
    void notifyAll() {
        for (Observer* observer : observers) {
            observer->notify(this);  // Call notify on each attached observer
        }
    }

private:
    std::vector<Observer*> observers;  // List of observers (e.g., shaders)
};
inline Subject::~Subject() {}