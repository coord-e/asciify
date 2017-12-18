#include <algorithm>
#include <iostream>
#include <numeric>
#include <sstream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/text.hpp>

#include <args.hxx>

int main(int argc, char** argv)
{
  args::ArgumentParser argparser("asciify: Convert an image to ascii art");
  args::HelpFlag help(argparser, "help", "Print this help", {'h', "help"});
  args::Positional<std::string> arg_path(argparser, "image", "Path to image file");
  args::ValueFlag<int> arg_rows(argparser, "rows", "Number of rows (=5)", {'r', "rows"});
  args::ValueFlag<int> arg_cols(argparser, "cols", "Number of cols (=5)", {'c', "cols"});
  args::ValueFlag<double> arg_threshold1(argparser, "threshold1",
                                         "threshold1 of Canny edge detector (=50)", {"th1"});
  args::ValueFlag<double> arg_threshold2(argparser, "threshold2",
                                         "threshold2 of Canny edge detector (=200)", {"th2"});
  args::ValueFlag<int> arg_aperturesize(argparser, "apertureSize",
                                        "apertureSize of Canny edge detector (=3)", {"apsize"});
  args::Flag arg_l2gradient(argparser, "L2gradient",
                            "Whether use L2gradient in Canny edge detector", {"l2grad"});
  args::ValueFlag<std::string> arg_tessdata(argparser, "tessdata",
                                            "Path to tessdata directory (=/usr/share/tessdata)",
                                            {'t', "tessdata"});
  args::ValueFlag<std::string> arg_lang(argparser, "language",
                                        "Language to use in tesseract (=eng)", {'l', "lang"});
  args::ValueFlag<std::string> arg_charset(
      argparser, "charset", "charset to use in tesseract (=!'()*+,-./:;<=>[\\]_{|}~«°»)",
      {"charset"});
  args::Flag arg_verbose(argparser, "verbose", "Print verbose output and show images in process",
                         {'v', "verbose"});
  try {
    argparser.ParseCLI(argc, argv);
  } catch (args::Help) {
    std::cout << argparser;
    return 0;
  } catch (args::ParseError e) {
    std::cerr << e.what() << std::endl;
    std::cerr << argparser;
    return -1;
  }
  if (!arg_path) {
    std::cerr << "Specify a path to image file." << std::endl;
    std::cerr << argparser;
    return -1;
  }

  // rows : cols == canny_image.rows : canny_image.cols
  auto const rows     = arg_rows ? args::get(arg_rows) : 5;
  auto const cols     = arg_cols ? args::get(arg_cols) : 5;
  auto const path     = args::get(arg_path);
  auto const th1      = arg_threshold1 ? args::get(arg_threshold1) : 50;
  auto const th2      = arg_threshold2 ? args::get(arg_threshold2) : 200;
  auto const aps      = arg_aperturesize ? args::get(arg_aperturesize) : 3;
  bool const l2       = arg_l2gradient;
  auto const tessdata = arg_tessdata ? args::get(arg_tessdata) : "/usr/share/tessdata";
  auto const lang     = arg_lang ? args::get(arg_lang) : "eng";
  auto const charset  = arg_charset ? args::get(arg_charset) : "!'()*+,-./:;<=>[\\]_{|}~«°»";
  bool const verbose  = arg_verbose;

  cv::Ptr<cv::text::OCRTesseract> ocr;
  try {
    ocr = cv::text::OCRTesseract::create(tessdata.c_str(), lang.c_str(), charset.c_str(),
                                         cv::text::OEM_DEFAULT, cv::text::PSM_SINGLE_CHAR);
  } catch (int err) {
    std::cerr << "Failed to initialize tesseract. (" << err << ')' << std::endl;
    return -1;
  }

  auto raw_image = cv::imread(path, 0);
  if (!raw_image.data) {
    std::cerr << "Failed to load supplied image: " << path << std::endl;
    return -1;
  }

  cv::Mat canny_image;
  try {
    cv::Canny(raw_image, canny_image, th1, th2, aps, l2);
  } catch (cv::Exception& ex) {
    std::cerr << "Failed to apply edge detection." << std::endl;
    std::cerr << ex.what() << std::endl;
    return -1;
  }

  int cn = std::floor(canny_image.cols / cols) * cols;
  int rn = std::floor(canny_image.rows / rows) * rows;
  assert(!(rn % rows || cn % cols));

  if (verbose) {
    std::cerr << "Resized image: " << rn << 'x' << cn << std::endl;
    cv::imshow("Canny", canny_image);
    cv::waitKey(0);
  }

  cv::Mat src_image;
  try {
    cv::resize(canny_image, src_image, cv::Size(cn, rn));
  } catch (cv::Exception& ex) {
    std::cerr << "Failed to resize image." << std::endl;
    std::cerr << ex.what() << std::endl;
    return -1;
  }

  std::stringstream result;
  for (int y = 0; y < src_image.rows; y += rows) {
    for (int x = 0; x < src_image.cols; x += cols) {
      auto part_image = src_image(cv::Rect(x, y, cols, rows));

      std::string text;
      ocr->run(part_image, text);
      text.erase(std::remove(text.begin(), text.end(), '\n'), text.end());
      result << (text.empty() ? " " : text);
    }
    result << std::endl;
  }

  std::copy(std::istreambuf_iterator<char>(result), std::istreambuf_iterator<char>(),
            std::ostreambuf_iterator<char>(std::cout));

  return 0;
}
