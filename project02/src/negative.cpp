#include <opencv2/opencv.hpp> 
#include <stdio.h> 

int main(int argc, char** argv) 
{ 
    if (argc != 3) { 
        printf("usage: negative <in_image_path> <out_image_path>\n"); 
        return -1; 
    } 

    cv::Mat image = cv::imread(argv[1], cv::IMREAD_COLOR); 
    if (!image.data) { 
        printf("No image data \n"); 
        return -1; 
    } 

    cv::Mat out {image.size(), image.type()};

    int y, x, i;
    for(y=0; y<image.rows; y++) {
        for(x=0; x<image.cols; x++){
            cv::Vec3b pixel = image.at<cv::Vec3b>(y, x);
            for(i=0; i<image.channels(); i++) pixel[i] = 255 - pixel[i];
            out.at<cv::Vec3b>(y, x) = pixel;
        }
    }

    cv::imwrite(argv[2], out);
    imshow("Negative Image", out);
    std::cout << "Process Completed. Saved as " << argv[2] << std::endl;

    cv::waitKey(0); // Wait for user input

    return 0; 
}

