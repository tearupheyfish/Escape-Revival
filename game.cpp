//
// Created by Tofu on 2026/2/4.
//

#include "game.h"

#include <chrono>

bool Game::exit = false;
bool registe_sig = false;

auto Game::signal_handler(int)->void{exit = true;}

Game::Game()
{
    if (!registe_sig)
    {
        std::signal(SIGTERM, signal_handler);
        std::signal(SIGINT, signal_handler);
        registe_sig = true;
    }
}

void Renderer::operator()(class UI* evt)
{
    glfwSwapInterval(1);

    while (evt->running && evt->screen)
    {
        glfwPollEvents();


        evt->screen.swapbuffer();
    }
}

void Logic::run()
{
    running = true;
    std::chrono::time_point<std::chrono::high_resolution_clock> last_time;
    while (running)
    {
        auto sleep_until = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(50);
        //do something
        std::this_thread::sleep_until(sleep_until);
    }
}
