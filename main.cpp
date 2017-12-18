#include <iostream>
#include <numeric>
#include <algorithm>
#include <sstream>

#include <opencv2/opencv.hpp>
#include <opencv2/text.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

int main (int argc, char **argv)
{
  if(argc != 2){
    std::cerr << "Supply filename" << std::endl;
    return -1;
  }

  // Will be specified by cmdline options...
  // rows : cols == canny_image.rows : canny_image.cols
  auto rows = 5;
  auto cols = 5;
  std::string space = " ";

  auto ocr = cv::text::OCRTesseract::create("/usr/share/tessdata", "eng", "!'()*+,-./:;<=>[\\]_{|}~«°»", cv::text::OEM_DEFAULT,  cv::text::PSM_SINGLE_CHAR);
  //auto ocr = cv::text::OCRTesseract::create("/usr/share/tessdata", "eng", "#-/\\(){}^<>._|°", cv::text::OEM_DEFAULT,  cv::text::PSM_SINGLE_CHAR);

  auto raw_image = cv::imread(argv[1], 0);
  if(!raw_image.data){
    std::cerr << "Failed to load supplied image: " << argv[1] << std::endl;
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
