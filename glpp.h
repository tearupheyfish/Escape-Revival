//
// Created by Tofu on 2026/2/4.
//

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <string>
#include <thread>
#include <vector>

class Enviroment
{
    static std::unique_ptr<Enviroment> instance_;
    std::thread::id tid;
    bool good;

public:
    Enviroment(std::thread::id tid)
        :tid(tid), good(true)
    {
        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }
    ~Enviroment()
    {
        glfwTerminate();
    }

    static void init()
    {

        if (!instance_)
        {
            instance_ = std::make_unique<Enviroment>(std::this_thread::get_id());
        }
        else
            if (std::this_thread::get_id() != instance_->tid)
                instance_.reset(new Enviroment(std::this_thread::get_id()));
    }
};

class Screen
{
    GLFWwindow* window;
public:
    Screen():window(nullptr)
    {}
    Screen(int width, int height, const std::string& title)
    {
        Enviroment::init();
        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        setascontext();//glad的加载是窗口相关的
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        onResize(defaultonresize);
    }
    ~Screen()
    {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    Screen(Screen&& right)
        :Screen()
    {
        if (&right != this)
            swap(*this, right);
    }
    Screen& operator=(Screen&& right)
    {
        if (&right != this)
            swap(*this, right);
        return *this;
    }

    Screen(const Screen&) = delete;
    Screen& operator=(const Screen&) = delete;

    friend void swap(Screen& left, Screen& right)
    {
        using std::swap;
        swap(left.window, right.window);
    }

    void setascontext()
    {
        glfwMakeContextCurrent(window);
    }
    operator bool()
    {
        return !glfwWindowShouldClose(window);
    }
    void close()
    {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    void swapbuffer()
    {
        glfwSwapBuffers(window);
    }
    void onResize(void func(GLFWwindow*, int, int))
    {
        glfwSetFramebufferSizeCallback(window, func);
    }
    int getKey(int keycode)
    {
        return glfwGetKey(window, keycode);
    }
    void onKeyEvent(GLFWkeyfun func)
    {
        glfwSetKeyCallback(window, func);
    }
private:
    static void defaultonresize(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }
};



