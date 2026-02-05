//
// Created by Tofu on 2026/2/4.
//

#ifndef ESCAPEREOPEN_GAME_H
#define ESCAPEREOPEN_GAME_H

#include <csignal>

#include "glpp.h"

#include <functional>
#include <future>
#include <mutex>
#include <thread>

enum GAMESTATUS{MAINMENU, DAWN };

class Renderer
{
    // class UI* evt;
public:
    Renderer() = default;
    virtual ~Renderer() = default;

    virtual void operator()(class UI*);
};

class UI
{
    std::thread thr;
    std::string title;
    std::mutex mutex;
    std::condition_variable has_renderer;
    Screen screen;
    Renderer* renderer;
    int width;
    int height;
    std::atomic_bool running;

    void render()
    {
        screen = Screen(width, height, title);
        do
        {
            std::unique_lock lock(mutex);
            has_renderer.wait(lock, [this]()->bool
            {
                return renderer;
            });
            running = true;
            (*renderer)(this);
        }while (renderer && screen);
        std::raise(SIGTERM);
    }

public:
    UI()
    : width(800), height(600), running(true),renderer(nullptr)
    {
        thr = std::thread([this](){render();});
    }
    void stop()
    {
        running = false;
    }
    void join()
    {
        if (thr.joinable())
            thr.join();
    }
    void change_renderer(Renderer* renderer)
    {
        {
            std::lock_guard lock(mutex);
            running = false;
            this->renderer = std::move(renderer);
        }
        has_renderer.notify_one();
    }
    void setupScreen(int width, int height, std::string title)
    {
        this->width = width;this->height = height; title = std::move(title);
    }

private:
    friend class Renderer;
};

class Data
{
    Renderer* renderer;
    std::function<void(UI*)> ticktock;
public:
    Data()
        :renderer(new Renderer())
    {}
    ~Data()
    {
        delete renderer;
    }
    friend class Game;
};

class Logic
{
    Data* data; //ref but no own
    bool running;
    std::thread thread;

    void run();
public:
    Logic()
        :thread([&](){run();}), data(nullptr)//, running(true)
    {
    }
    ~Logic()
    {
        stop();
        join();
    }
    void stop()
    {
        running = false;
    }
    void join()
    {
        if (thread.joinable())
            thread.join();
    }
};

class Game
{
    UI ui;
    Logic logic;
    Data* data;
    static bool exit;
    static void signal_handler(int);
public:
    Game();
    void setupScreen(int width, int height, const std::string& title)
    {
        ui.setupScreen(width, height, title);
    }
    int run()
    {
        data = new Data();
        ui.change_renderer(data->renderer);
        // logic.change_controller(data->ticktock);

        while (!exit)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        ui.stop();
        ui.join();
        logic.stop();
        logic.join();

        delete data;
        return 0;
    }
};

class GameMode
{

};

#endif //ESCAPEREOPEN_GAME_H