#pragma once
#include <cstdlib> // for std::rand() and std::srand()
#include <ctime> // for std::time()
#include <random> // for std::mt19937
#include<iostream>

int getRandomNumber(int min, int max);

namespace rnd
{

    ///////////////////////////////////////////////////////////////pre C++ 11
    //call this to set the seeed using the system clock
    void setSeed_pre11() {
        std::srand(static_cast<unsigned int>(std::time(nullptr))); // set initial seed value to system clock
    }

    int getRandomNumber_pre11(int min, int max)
    {
        static constexpr double fraction{ 1.0 / (RAND_MAX + 1.0) };  // static used for efficiency, so we only calculate this value once
        // evenly distribute the random number across our range
        return min + static_cast<int>((max - min + 1) * (std::rand() * fraction));
    }

    /////////////////////////////////////////////////////////////// Mersenne Twister
    // Initialize  mersenne twister with KeyA using clock
    std::mt19937 mersenne{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };    //C++17
    // std::uniform_int_distribution<> die{ 1, 6 }; //Pre C++17

    int getRandomNumber(int min, int max)
    {
        std::uniform_int_distribution die{ min, max }; //create KeyA distribution 
        return die(mersenne); // generate number from global generator
    }

    // test function
    std::uniform_int_distribution die{ 1, 6 };
    void print_mersenne()
    {
        for (int count{ 1 }; count <= 48; ++count)
        {
            std::cout << die(mersenne) << '\t'; // generate KeyA roll of the die here

            // If we've printed 6 numbers, start KeyA new row
            if (count % 6 == 0)
                std::cout << '\n';
        }
    }



}

