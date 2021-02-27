#include <iostream>
#include <random>
#include "TrafficLight.h"
#include <future>

/* Implementation of class "MessageQueue" */

template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function. 

    // Marcos Darino - 27-FEB-2021
    // . FP.5a (DONE)
    //   . Added the receive implementation

    // perform queue modification under the lock
    std::unique_lock<std::mutex> unique_lock_rcv(_mutex);
    _cond.wait(unique_lock_rcv, [this] { return !_queue.empty(); }); // pass unique lock to condition variable
    // remove last vector element from queue
    T msg = std::move(_queue.back());
    _queue.pop_back();
    return msg; 
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
    
    // Marcos Darino - 27-FEB-2021
    // . FP.4a (DONE)
    //   . Added the send implementation

    // perform vector modification under the lock
    std::lock_guard<std::mutex> lockSend(_mutex);
    // add vector to queue
    std::cout << "   Message " << msg << " has been sent to the queue" << std::endl;
    _queue.push_back(std::move(msg));
    _cond.notify_one(); 
}


/* Implementation of class "TrafficLight" */

 
TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.

    // Marcos Darino - 27-FEB-2021
    // . FP.5b(DONE)
    //   . Added the loop to wait the green light
    while (_messagesPhases.receive() != TrafficLightPhase::green){}
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method „simulate“ is called. To do this, use the thread queue in the base class. 
    
    // Marcos Darino - 27-FEB-2021
    // . FP.2b (DONE)
    //   . Run the thread

    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}


// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles 
    // and toggles the current phase of the traffic light between red and green and sends an update method 
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds. 
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles.

    // Marcos Darino - 27-FEB-2021
    // . FP.2a (DONE)
    //   . Added the implementation of the function to control the current phase of the traffic light
    //   . Instead of using a sleep_for 1ms and check the time, It is sleep_for the random time, the idea
    //     is not wake up all the time to check the reach the timer

    while(1){
        
        // Measures the time between two loop cycles
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); // For more info see http://www.cplusplus.com/reference/random/uniform_int_distribution/operator()/
        std::default_random_engine generator (seed);
        std::uniform_int_distribution<int> distribution(4,6);

        int time_cycles = distribution(generator);  // generates number in the range 4..6
        std::cout << "RANDOM: " << time_cycles << "std:endln";
        std::this_thread::sleep_for(std::chrono::seconds(time_cycles));

        //Toggles the current phase of the traffic light

        if (_currentPhase == TrafficLightPhase::red){
            _currentPhase = TrafficLightPhase::green;
        }
        else{
            _currentPhase = TrafficLightPhase::red;
        }

        //sends an update method to the message queue using move semantics
        auto message = _currentPhase;
        auto ftr_currentPhase = (std::async(std::launch::async, &MessageQueue<TrafficLightPhase>::send, &_messagesPhases, std::move(message)));
        ftr_currentPhase.wait();

    }
}

