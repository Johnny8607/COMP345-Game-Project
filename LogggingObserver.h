#pragma once

class Observer
{
private:
public:
    ~Observer();
    virtual void Update() = 0;
protected:
    Observer();
};

Observer::Observer()
{
}

Observer::~Observer()
{
}
