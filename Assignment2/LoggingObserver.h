#pragma once
#include <list>
#include <iostream>
#include <fstream>
using namespace std;

//Forward declarations
class Observer;
class Subject;
class ILoggable;

//ALL CLASSES THAT WILL LOG THIER COMMANDS WILL HAVE TO BE A CHILD OF THIS CLASS(INTERFACE)
class ILoggable {
public:
    virtual ~ILoggable() = default;
    virtual std::string stringToLog() const = 0;
};

class Observer
{
private:
public:
    ~Observer();
    virtual void Update(ILoggable* loggable) = 0;
protected:
    Observer();
};

// loggable classes must also implement this subject class
class Subject{
private:
list<Observer*> *_observers;
public:
virtual void Attach(Observer* o);
virtual void Detach(Observer* o);
virtual void Notify(ILoggable* loggable);
Subject();
~Subject();
};



class LogObserver : public Observer{
private:
std::ofstream logFile;
public:
LogObserver();
~LogObserver();
void update(ILoggable* loggable);
};

