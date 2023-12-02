#pragma once

#include <filesystem>
#include <iostream>
#include <string_view>
#include <vector>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Clock.hpp>

namespace fs = std::filesystem;

std::string read_file_to_string(const std::filesystem::path& file_path);
std::vector<std::string> split_string(const std::string& string, char delim = ' ');

template <typename N, typename T>
sf::Vector2<N> cast_vector(const sf::Vector2<T>& vec)
{
    return sf::Vector2<N>{static_cast<N>(vec.x), static_cast<N>(vec.y)};
}

struct FPSCounter final
{
    sf::Clock timer;
    float frameTime = 0;
    float frameCount = 0;

    void update()
    {
        frameCount++;
        if (timer.getElapsedTime() > sf::seconds(0.25))
        {
            auto time = timer.getElapsedTime();
            frameTime = time.asMilliseconds() / frameCount;
            timer.restart();
            frameCount = 0;
        }
    }
};

template <int Ticks>
class TimeStep
{
  public:
    template <typename F>
    void update(F f)
    {
        sf::Time time = timer_.getElapsedTime();
        sf::Time elapsed = time - last_time_;
        last_time_ = time;
        lag_ += elapsed;
        while (lag_ >= timePerUpdate_)
        {
            lag_ -= timePerUpdate_;
            f(dt_.restart());
        }
    }

  private:
    const sf::Time timePerUpdate_ = sf::seconds(1.f / Ticks);
    sf::Clock timer_;
    sf::Clock dt_;
    sf::Time last_time_ = sf::Time::Zero;
    sf::Time lag_ = sf::Time::Zero;
};
