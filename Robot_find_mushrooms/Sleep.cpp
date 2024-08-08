#include "Sleep.h"
#include <thread>
#include <chrono>  

void slp(int sleepDuration) {
    std::this_thread::sleep_for(std::chrono::seconds(sleepDuration));
}
