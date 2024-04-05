#ifndef TIMER_H
#define TIMER_H

#include <chrono>


class Timer 
{
public:
    Timer() : running(false){};

    void start() 
    {
        startTime = std::chrono::high_resolution_clock::now();
        running = true;
    }

    void stop()
    {
    endTime = std::chrono::high_resolution_clock::now();
    running = false;
    }

    double elapsedSeconds()
    {
        if (running)
        {
            stop();
        }
        std::chrono::duration<double, std::nano> elapsed = endTime - startTime;
        return elapsed.count();
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime; 
    std::chrono::time_point<std::chrono::high_resolution_clock> endTime; 
    bool running; 
};

#endif // TIMER_H
