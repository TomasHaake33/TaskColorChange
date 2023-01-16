#include "Header.h"

void MyCreator::FeedFrame(unsigned char* packedRGBData, size_t width, size_t height, size_t strideInBytes, itvcvError* error)
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
    
    *error = itvcvError::SUCCESS;

    /*Создаю матрицу из packedRGBData, перевожу ее в HSV. Необходимо копирование, чтобы packedGRBData не менялось
    Она получится без оригинального паддинга, так что далее его придется учитывать*/
    auto img = cv::Mat(height, width, CV_8UC3, (void*)packedRGBData, strideInBytes).clone();
    cv::cvtColor(img, img, cv::COLOR_RGB2HSV);

    /*Выбираю область HSV, которую считаю зеленой. Создаю одноканальную маску, которая соответствует таким пикселям
    на img*/
    cv::Mat mask;
    cv::inRange(img, cv::Scalar(30, 30, 30), cv::Scalar(80, 255, 255), mask);

    //Паддинг с правой стороны
    size_t padding = strideInBytes / 3 - width;

    //Если пиксель на маске белый, меняю G и R компоненты оригинала в этом месте
    for (size_t i = 0; i < height; ++i)
        for (size_t j = 0; j < width; ++j)
        {
            unsigned char* maskPixel = &mask.data[i * width + j];
            if (*maskPixel != 0)
            {
                /*В оригинале возможен паддинг, которого нет в маске. Учитываю его*/
                unsigned char* imgPixel = &packedRGBData[(i * (padding + width) + j) * 3];
                std::swap(imgPixel[0], imgPixel[1]);
            }
        }
};