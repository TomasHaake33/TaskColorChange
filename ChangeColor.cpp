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

    //Маска, соответствующая зеленому цвету
    cv::Mat mask;
    cv::inRange(img, GREEN_LOWER, GREEN_UPPER, mask);

    /*Если маска ненулевая на данном hsv пикселе, меняю его цвет на красный H(0, x, y)
    Создаю из него матрицу, чтобы конвертировать один пиксель в RGB по формуле из opencv.
    Копирую этот RGB пиксель в соответствующее место packedGRBData*/
    const auto padding = strideInBytes / 3 - width;
    for (size_t i = 0; i < height; ++i)
    {
        for (size_t j = 0; j < width; ++j)
        {
            if (mask.data[i * width + j] != 0)
            {
                cv::Mat hsvPixel(1, 1, CV_8UC3, img.at<cv::Vec3b>(i, j));
                hsvPixel.data[0] = 0;
                cv::cvtColor(hsvPixel, hsvPixel, cv::COLOR_HSV2RGB);
                std::memcpy(&packedRGBData[(i * (padding + width) + j) * 3],
                    &hsvPixel.data[0], 3);
            }
        }
    }
 
    *error = itvcvError::SUCCESS;
};
