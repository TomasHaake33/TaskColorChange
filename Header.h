#pragma once

#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

enum class itvcvError
{
    SUCCESS = 0,
    INCORRECT_DIMS = 1,
    INCORRECT_STRIDE = 2,
    NULL_PTR = 3
};

class IChangeColorEngine
{
public:
    virtual void FeedFrame(unsigned char* packedRGBData, size_t width, size_t height, size_t strideInBytes, itvcvError* error) = 0;
    virtual ~IChangeColorEngine() {}
};

class MyCreator : public IChangeColorEngine
{
public:
    void FeedFrame(unsigned char* packedRGBData, size_t width, size_t height, size_t strideInBytes, itvcvError* error) override;
    //Через new ничего не выделяется, поэтому пустой
    ~MyCreator() {};
};