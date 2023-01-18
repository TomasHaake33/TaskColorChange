#pragma once

#include <iostream>
#include <string>

#include <boost/program_options.hpp>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

//Наверно лучше constexpr char DEFAULT_INPATH[]
const std::string DEFAULT_INPATH = "../images";

/*Выглядит как по мне хуево, т.к. не constexpr и юзает динамическую память.
Но альтернатив не вижу. Мб из хедера тогда их вытащить в другое место?*/
const cv::Scalar GREEN_LOWER(30, 40, 40);
const cv::Scalar GREEN_UPPER(80, 255, 255);


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

class ChangeColor : public IChangeColorEngine
{
public:
    void FeedFrame(unsigned char* packedRGBData, size_t width, size_t height, size_t strideInBytes, itvcvError* error) override;
    //Через new ничего не выделяется, поэтому пустой
    ~ChangeColor() {};
};