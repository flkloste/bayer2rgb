#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

template <class OIt, typename Size_t>
bool read_n_bytes(std::istream& stream, Size_t n, OIt it) {
  if (stream.peek() == EOF || n == 0) {
    return false;
  }
  std::copy_n(std::istreambuf_iterator<char>{stream}, n, it);
  stream.get();
  return !stream.eof();
}
 
int main(int argc, char* argv[])
{
    std::string usage = "Usage: " + std::string(argv[0]) + " <width> <height> <path-to-bayer-file>";
    if(argc != 4)
    {
        std::cout << usage << std::endl;
        return 1;
    }

    int width = 0;
    int height=0;
    std::istringstream iss(argv[1]);
    if(!(iss >> width))
    {
        std::cout << "Error: Invalid Width given: " << argv[1] << std::endl;
        std::cout << usage << std::endl;
        return 1;
    }

    std::istringstream iss2(argv[2]);
    if(!(iss2 >> height))
    {
        std::cout << "Error: Invalid height given: " << argv[2] << std::endl;
        std::cout << usage << std::endl;
        return 1;
    } 

    std::string bayerFilePath(argv[3]); 
    std::stringstream ss;
    ss << bayerFilePath << ".png";
    std::string outputPath = ss.str();
    int depth = 8; //bit depth

    std::vector<uint8_t> imageData;
    imageData.reserve(width * height * depth / 8);

    int cvType = CV_MAKETYPE(CV_8U, 1);
    cv::Mat bayerSource(height, width, cvType, &imageData[0]);
    cv::Mat rgb16Dest(height, width, CV_16UC3);

    int i = 0;
    imageData.clear();

    // read image
    std::ifstream inputFile(bayerFilePath, std::ios::binary);
    read_n_bytes(inputFile, width * height * depth / 8, std::back_inserter(imageData));

    cv::cvtColor(bayerSource, rgb16Dest, CV_BayerGR2RGB);

    cv::imwrite(outputPath, rgb16Dest);
    
    return 0;
}
