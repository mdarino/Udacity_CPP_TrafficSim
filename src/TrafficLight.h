#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include "TrafficObject.h"

enum TrafficLightPhase
{
    red,
    green
};

// forward declarations to avoid include cycle
class Vehicle;


// FP.3 Define a class „MessageQueue“ which has the public methods send and receive. 
// Send should take an rvalue reference of type TrafficLightPhase whereas receive should return this type. 
// Also, the class should define an std::dequeue called _queue, which stores objects of type TrafficLightPhase. 
// Also, there should be an std::condition_variable as well as an std::mutex as private members. 

// Marcos Darino - 27-FEB-2021
// . FP.3 (DONE)
//   . Added the class definition for the messageQueue
//   . THis code is based on the example on the Building a Concurrent Message Queue unit


template <class T>
class MessageQueue
{
public:
    T receive();
    void send(T &&msg);
private:
    std::mutex _mutex_message;
    std::condition_variable _cond_message;
    std::deque<T> _queue_message;
};

// FP.1 : Define a class „TrafficLight“ which is a child class of TrafficObject. 
// The class shall have the public methods „void waitForGreen()“ and „void simulate()“ 
// as well as „TrafficLightPhase getCurrentPhase()“, where TrafficLightPhase is an enum that 
// can be either „red“ or „green“. Also, add the private method „void cycleThroughPhases()“. 
// Furthermore, there shall be the private member _currentPhase which can take „red“ or „green“ as its value. 

// Marcos Darino - 27-FEB-2021
// . FP.1 (DONE)
//   . Added the class definition for the TrafficLight


class TrafficLight : public TrafficObject
{
public:
    // constructor / desctructor
    TrafficLight();
    // getters / setters
    TrafficLightPhase getCurrentPhase();

    // typical behaviour methods
    void waitForGreen();
    void simulate();

private:
    // typical behaviour methods
    void cycleThroughPhases();
    TrafficLightPhase _currentPhase;

    // FP.4b : create a private member of type MessageQueue for messages of type TrafficLightPhase 
    // and use it within the infinite loop to push each new TrafficLightPhase into it by calling 
    // send in conjunction with move semantics.
    
    // Marcos Darino - 27-FEB-2021
    // . FP.4 (DONE)
    //   . Added the private queue called _messagesPhases

    std::condition_variable _condition;
    std::mutex _mutex;
    MessageQueue<TrafficLightPhase> _messagesPhases;
};

#endif