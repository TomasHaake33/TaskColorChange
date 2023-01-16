#include "Header.h"

int main()
{
	cv::namedWindow("Orig");
	cv::namedWindow("Output");

	itvcvError error;

	std::string path;
	std::cout << "Enter image path: " << std::endl;
	std::cin >> path;

	cv::Mat testImg = cv::imread(path);
	cv::imshow("Orig", testImg);

	//Произвольный правосторонний паддинг для доп. теста
	int padding = 50;
	cv::Mat padded(testImg.rows, testImg.cols + padding, testImg.type(), cv::Scalar(0));
	testImg.copyTo(padded(cv::Rect(0, 0, testImg.cols, testImg.rows)));
	cv::imshow("Orig", padded);

	cv::cvtColor(padded, padded, cv::COLOR_BGR2RGB);
	MyCreator creator;
	//Некорректные вызовы
	creator.FeedFrame(nullptr, testImg.cols, testImg.rows, padded.cols * 3, &error);
	creator.FeedFrame(0, testImg.cols, testImg.rows, padded.cols * 3, &error);
	creator.FeedFrame(padded.data, 0, 0, padded.cols * 3, &error);
	creator.FeedFrame(padded.data, testImg.cols, testImg.rows, padded.cols, &error);

	//Корректный вызов
	creator.FeedFrame(padded.data, testImg.cols, testImg.rows, padded.cols * 3, &error);

	//Эта конвертация нужна только для отображения
	cv::cvtColor(padded, padded, cv::COLOR_RGB2BGR);
	cv::imshow("Output", padded);
	cv::waitKey(0);
}
