#ifndef SUBJECT_H
#define SUBJECT_H

#include <vector>

class Observer;

class Subject
{
public:
    Subject();
    void notifyObservers();
    void registerObserver(Observer* observer);

private:
    std::vector<Observer*> observers;
};

#endif // SUBJECT_H
