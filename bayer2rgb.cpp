#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <fstream>

template <class OIt, typename Size_t>
bool read_n_bytes(std::istream& stream, Size_t n, OIt it) {
  if (stream.peek() == EOF || n == 0) {
    return false;
  }
  std::copy_n(std::istreambuf_iterator<char>{stream}, n, it);
  stream.get();
  return !stream.eof();
}
 
int main() {
    std::string bayerFilePath = "GRBG_RAW8.raw";
    std::string outputPath = "GRBG_RAW8.raw.converted.tif";
    int width = 1280;
    int height = 960;
    int depth = 1; // 1=8bit

    std::vector<uint8_t> imageData;
    imageData.reserve(width * height * depth);

    int cvType = CV_MAKETYPE(CV_8U, 1);
    cv::Mat bayerSource(height, width, cvType, &imageData[0]);
    cv::Mat rgb16Dest(height, width, CV_16UC3);

    int i = 0;
    imageData.clear();

    // read image
    std::ifstream inputFile(bayerFilePath, std::ios::binary);
    read_n_bytes(inputFile, width * height * depth, std::back_inserter(imageData));

    cv::cvtColor(bayerSource, rgb16Dest, CV_BayerGR2RGB_EA);

    cv::imwrite(outputPath, rgb16Dest);
    
    return 0;
}
