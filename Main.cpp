#include "ChangeColor.h"

namespace po = boost::program_options;

int main(int argc, char** argv)
{
	//Парсинг командной строки
	po::options_description desc("Description: \n Replace green with red in .jpg image ");
	desc.add_options()
		("help,h", "help")
		("inpath,i", po::value<std::string>(), "Input images folder path")
		("outpath,o", po::value<std::string>(), "Output images folder path")
		;
	po::variables_map vm;
	try 
	{
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);
	}
	catch (po::error& e) 
	{
		std::cout << e.what() << std::endl;
		std::cout << desc << std::endl;
		return 1;
	}

	if (vm.count("help"))
	{
		std::cout << desc << std::endl;
		return 0;
	}
	
	//Определение путей к папкам для чтения и сохранения
	std::string inpath;
	std::string outpath;
	if (!vm.count("inpath"))
	{
		std::cout << "No inpath is specified. Using default path " << DEFAULT_INPATH << std::endl;
		inpath = DEFAULT_INPATH;
	}
	else
	{
		inpath = vm["inpath"].as<std::string>();
	}

	if (!vm.count("outpath"))
	{
		std::cout << "No outpath is specified. Using default path " << inpath + "\\output"
			<< std::endl;
		outpath = inpath + "/output";
	}
	else
	{
		outpath = vm["outpath"].as<std::string>();
	}

	std::vector<std::string> fn;
	try
	{
		cv::glob(inpath + "/*.jpg", fn, false);
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}

	if (fn.empty())
	{
		std::cout << "No .jpg images in the specified folder. Exiting" << std::endl;
		return 0;
	}

	//Вектор изображений, находящихся в папке
	std::vector<cv::Mat> images;
	try
	{
		for (size_t i = 0; i < fn.size(); ++i)
		{
			images.emplace_back(cv::imread(fn[i]));
		}
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		std::cout << "Error during image reading" << std::endl;
		return 1;
	}

	//Изменение зеленого на красный и сохранение изображения в outpath
	auto ChangeColorEngine = std::make_unique<ChangeColor>();
	itvcvError error;
	for (size_t i = 0; i < images.size(); ++i)
	{
		cv::Mat& img = images[i];
		cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
		ChangeColorEngine->FeedFrame(img.data, img.cols, img.rows, img.cols * 3, &error);
		if (error != itvcvError::SUCCESS)
		{
			std::cout << "Error in FeedFrame with image " << i
				<< " , code" << (int)error;
			continue;
		}
		try
		{
			cv::cvtColor(img, img, cv::COLOR_RGB2BGR);
			cv::imwrite(outpath + "/" + std::to_string(i) + ".jpeg", img, {cv::IMWRITE_JPEG_QUALITY, 100});
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
			std::cout << "Error writing image " << i << std::endl;
			continue;
		}
	}

	return 0;
}
