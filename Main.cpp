#include "ChangeColor.h"

namespace po = boost::program_options;

int main(int argc, char** argv)
{
	po::options_description desc("Description");
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
		std::cout << "No outpath is specified. Using default path " << DEFAULT_INPATH + "/output"
			<< std::endl;
		outpath = DEFAULT_INPATH + "/output";
	}
	else
	{
		outpath = vm["outpath"].as<std::string>();
	}



	std::vector<cv::String> fn;
	/*Есть идея замутить константу со всеми форматами, которые буду искать.
	Пока не трачу время. Тем более тогда надо будет запускать цикл по всем этим форматам,
	делать для каждого отдельные векторы и потом их слить. Короче отдельная функция*/
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

	std::vector<cv::Mat> images;
	size_t count = fn.size();
	try
	{
		for (size_t i = 0; i < count; ++i)
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

	auto ChangeColorEngine = std::make_unique<ChangeColor>();
	itvcvError error; //В этой и некст строчке лучше через указатель? 
	size_t counter = 0;
	for (auto& img : images)
	{
		++counter;

		cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
		ChangeColorEngine->FeedFrame(img.data, img.cols, img.rows, img.cols * 3, &error);
		if (error != itvcvError::SUCCESS)
		{
			std::cout << "Error in FeedFrame with image " << counter
				<< " , code" << (int)error;
			continue;
		}
		try
		{
			cv::cvtColor(img, img, cv::COLOR_RGB2BGR);
			cv::imwrite(outpath + "/" + std::to_string(counter) + ".jpeg", img, {cv::IMWRITE_JPEG_QUALITY, 100});
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
			std::cout << "Error writing image " << counter << std::endl;
			continue;
		}
	}

	return 0;
}
