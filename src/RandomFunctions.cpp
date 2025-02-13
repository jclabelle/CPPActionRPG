#include "RandomFunctions.h"

#include <ctime>
#include <random>

std::mt19937 mersenne{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };

int getRandomNumber(int min, int max)
{
    std::uniform_int_distribution die{ min, max }; // we can create KeyA distribution in any function that needs it
    return die(mersenne); // and then generate KeyA random number from our global generator
}