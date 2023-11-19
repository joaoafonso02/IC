#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char** argv) 
{ 
    if (argc != 4) { 
        std::cerr << "Usage: extract_channel <in_image_path> <channel> <out_image_path>\n";
        return -1; 
    } 

    cv::Mat image = cv::imread(argv[1], cv::IMREAD_COLOR); 
    if (image.empty()) { 
        std::cerr << "No image data\n"; 
        return -1; 
    } 

    int channel = std::stoi(argv[2]);
    if (channel < 0 || channel >= image.channels()) {
        std::cerr << "Invalid channel index\n";
        return -1;
    }

    cv::Mat out(image.size(), CV_8UC3);

    int y, x;
    for (y = 0; y < image.rows; y++) {
        for (x = 0; x < image.cols; x++) {
            out.at<cv::Vec3b>(y, x)[channel] = image.at<cv::Vec3b>(y, x)[channel];
        }
    }

    cv::imwrite(argv[3], out);
    cv::imshow("Extracted Channel", out);
    std::cout << "Process Completed. Saved as " << argv[3] << std::endl;

    cv::waitKey(0); // Wait for user input

    return 0; 
}
