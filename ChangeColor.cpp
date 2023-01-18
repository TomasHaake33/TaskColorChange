#include "ChangeColor.h"

void ChangeColor::FeedFrame(unsigned char* packedRGBData, size_t width, size_t height, size_t strideInBytes, itvcvError* error)
{

    if (width <= 0 || height <= 0)
    {
        *error = itvcvError::INCORRECT_DIMS;
        return;
    }

    //Страйд - ширина в байтах с учетом паддинга (с правой стороны). Должен быть как минимум в 3 раза больше ширины
    if (strideInBytes < width * 3)
    {
        *error = itvcvError::INCORRECT_STRIDE;
        return;
    }

    if (packedRGBData == nullptr)
    {
        *error = itvcvError::NULL_PTR;
        return;
    }
    

    /*Создаю матрицу из packedRGBData, перевожу ее в HSV. Необходимо копирование, чтобы packedGRBData не менялось
    Она получится без оригинального паддинга, так что далее его придется учитывать*/
    auto img = cv::Mat(height, width, CV_8UC3, (void*)packedRGBData, strideInBytes).clone();
    cv::cvtColor(img, img, cv::COLOR_RGB2HSV);

    cv::Mat mask;
    cv::inRange(img, GREEN_LOWER, GREEN_UPPER, mask);

    for (size_t i = 0; i < height; ++i)
    {
        for (size_t j = 0; j < width; ++j)
        {
            if (mask.data[i * width + j] != 0)
            {
                img.data[(i * width + j) * 3] = 0;
            }
        }
    }

    const auto padding = strideInBytes / 3 - width;
    cv::cvtColor(img, img, cv::COLOR_HSV2RGB);
    //Опять такой же цикл. Выглядит не очень, но альтернатив улучшающих нет кмк
    for (size_t i = 0; i < height; ++i)
    {
        for (size_t j = 0; j < width; ++j)
        {
            if (mask.data[i * width + j] != 0)
            {
                std::memcpy(&packedRGBData[(i * (padding + width) + j) * 3], 
                    &img.data[(i * width + j) * 3], 3);
            }
        }
    }
 
    *error = itvcvError::SUCCESS;
};