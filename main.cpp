#include <iostream>
#include <numeric>
#include <algorithm>
#include <sstream>

#include <opencv2/opencv.hpp>
#include <opencv2/text.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <args.hxx>

int main (int argc, char **argv)
{
  args::ArgumentParser argparser("asciify: Convert an image to ascii art");
  args::HelpFlag help(argparser, "help", "Print this help", {'h', "help"});
  args::Positional<std::string> arg_path(argparser, "image", "Path to image file");
  try{
      argparser.ParseCLI(argc, argv);
  } catch (args::Help){
      std::cout << argparser;
      return 0;
  } catch (args::ParseError e){
      std::cerr << e.what() << std::endl;
      std::cerr << argparser;
      return -1;
  }
  if(!arg_path){
    std::cerr << "Specify a path to image file." << std::endl;
    std::cerr << argparser;
    return -1;
  }

  // rows : cols == canny_image.rows : canny_image.cols
  auto rows = 5;
  auto cols = 5;
  auto path = args::get(arg_path);
  std::string space = " ";

  auto ocr = cv::text::OCRTesseract::create("/usr/share/tessdata", "eng", "!'()*+,-./:;<=>[\\]_{|}~«°»", cv::text::OEM_DEFAULT,  cv::text::PSM_SINGLE_CHAR);
  //auto ocr = cv::text::OCRTesseract::create("/usr/share/tessdata", "eng", "#-/\\(){}^<>._|°", cv::text::OEM_DEFAULT,  cv::text::PSM_SINGLE_CHAR);

  auto raw_image = cv::imread(path, 0);
  if(!raw_image.data){
    std::cerr << "Failed to load supplied image: " << path << std::endl;
    return -1;
  }

  cv::Mat canny_image;
	cv::Canny(raw_image, canny_image, 50, 200);

  if(rows / cols != canny_image.rows / canny_image.cols){
    std::cerr << "Image ratios are different: " << static_cast<float>(rows) / cols << " vs " << static_cast<float>(canny_image.rows) / canny_image.cols << std::endl;
    return -1;
  }
  int cn = std::floor(canny_image.cols / cols) * cols;
  int rn = std::floor(canny_image.rows / rows) * rows;
  assert(!(rn % rows || cn % cols));

  // cv::imshow("Canny", canny_image);
  // cv::waitKey(0);

  cv::Mat src_image;
  cv::resize(canny_image, src_image, cv::Size(cn, rn));

  std::stringstream result;
  for(int y = 0; y < src_image.rows; y+=rows){
    for(int x = 0; x < src_image.cols; x+=cols){
      auto part_image = src_image(cv::Rect(x, y, cols, rows));

      std::string text;
      ocr->run(part_image, text);
      text.erase(std::remove(text.begin(), text.end(), '\n'), text.end());
      result << (text.empty() || text == " " ? space : text);
    }
    result << std::endl;
  }

  std::copy(std::istreambuf_iterator<char>(result),
             std::istreambuf_iterator<char>(),
             std::ostreambuf_iterator<char>(std::cout));
}
