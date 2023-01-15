#pragma once

#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

//Имитация кодов ошибок
enum class itvcvError
{
    SUCCESS = 0,
    BAD_FRAME = 1,
    ERROR_2 = 2,
    ERROR_3 = 3
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
    void FeedFrame(unsigned char* packedRGBData, size_t width, size_t height, size_t strideInBytes, itvcvError* error) override
    {

        switch (*error)
        {
        case itvcvError::BAD_FRAME:
            std::cerr << "Error: Bad frame";
            return;
        case itvcvError::ERROR_2:
            std::cerr << "Error: 2";
            return;
        case itvcvError::ERROR_3:
            std::cerr << "Error: 3";
            return;
        }

        //Создаю матрицу из packedRGBData, перевожу ее в HSV. Необходимо копирование, чтобы packedGRBData не менялось
        std::vector<int> sizes = { int(height), int(width) };
        cv::Mat temp(sizes, CV_8UC3, packedRGBData);
        cv::Mat img;
        temp.copyTo(img);
        cv::cvtColor(img, img, cv::COLOR_RGB2HSV);

        /*Выбираю область HSV, которую считаю зеленой. Создаю одноканальную маску, которая соответствует таким пикселям
        на оригинале*/
        cv::Mat mask;
        cv::inRange(img, cv::Scalar(30, 30, 30), cv::Scalar(80, 255, 255), mask);

        //Если пиксель на маске белый, меняю G и R компоненты оригинала в этом месте
        for (size_t i = 0; i < height; ++i)
            for (size_t j = 0; j < width; ++j)
            {
                unsigned char* maskPixel = &mask.data[i * width + j];
                if (*maskPixel != 0)
                {
                    unsigned char* imgPixel = &packedRGBData[(i * width + j) * 3];
                    std::swap(imgPixel[0], imgPixel[1]);
                }
            }
    };
};