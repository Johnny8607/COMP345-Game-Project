#include "LoggingObserver.h"
using namespace std;

/*EXAMPLE CLASS METHOD THAT IMPLEMENTS ILOGGABLE AND SUBJECT
/ void saveCommand(....){
/.....
/.....
/notify(this);
/}
/
/std::string stringToLog() const override {
/return "Command Processed: " + command;
/}
*/

//Observer class implementation
Observer::Observer(){
}
// Observer::~Observer(){
// }

//Subject class implementation
Subject::Subject(){
    _observers = new list<Observer*>;
}
Subject::~Subject(){
    delete _observers;
}

void Subject::Attach(Observer* o){
    _observers->push_back(o);
};
void Subject::Detach(Observer* o){
    _observers->remove(o);
};
void Subject::Notify(ILoggable* loggable){
    list<Observer *>::iterator i = _observers->begin();
    for (; i != _observers->end(); ++i)
        (*i)->Update(loggable);
}

//LogObserver class implementation
LogObserver::LogObserver(){
    logFile.open("gamelog.txt", std::ios::out | std::ios::app);
    if(!logFile.is_open()){
        cerr << "Error: Could not open gamelog.txt for logging" << endl;
    }
}

LogObserver::~LogObserver() {
    if (logFile.is_open()){
        logFile.close();
    }
}

void LogObserver::Update(ILoggable* loggable){
    if(logFile.is_open() && loggable != nullptr){
        logFile << loggable->stringToLog() << endl;
        logFile.flush();
    }
}