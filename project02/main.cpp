#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <input_image> <output_image> <channel (B/G/R) | copy>" << std::endl;
        return 1;
    }

    Mat image = imread(argv[1]); // read file

    if (image.empty()) { // check if it is an image file
        cerr << "Error: Unable to read the input image." << endl;
        return 1;
    }

    char channel = argv[3][0]; // red, green or blue
    Mat newImage = Mat::zeros(image.size(), image.type()); // new image all pixels = 0

    // Loop through the image and copy the specified color channel or the entire image
    uchar* pixel;
    for (int i = 0; i < image.rows; i++) {
        pixel = image.ptr<uchar>(i);
        for (int j = 0; j < image.cols; j++) {
            if (strcmp(argv[3], "copy") == 0) {
                newImage.at<Vec3b>(i, j) = image.at<Vec3b>(i, j);  // Copy the entire image
            } else {
                switch (channel) {
                    case 'B':
                    case 'b':
                        newImage.at<Vec3b>(i, j)[0] = pixel[0];  // Blue channel
                        break;
                    case 'G':
                    case 'g':
                        newImage.at<Vec3b>(i, j)[1] = pixel[1];  // Green channel
                        break;
                    case 'R':
                    case 'r':
                        newImage.at<Vec3b>(i, j)[2] = pixel[2];  // Red channel
                        break;
                    default:
                        std::cerr << "Error: Invalid channel. Use 'B', 'G', 'R', or 'copy'." << std::endl;
                        return 1;
                }
            }
        }
    }

    imshow("New Image", newImage);

    // Save the new image
    imwrite(argv[2], newImage);

    cout << "Channel extraction complete. Saved as " << argv[2] << std::endl;

    waitKey(0); // Wait for user input

    return 0;
}
