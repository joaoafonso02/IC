#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;

int main(int argc, char *argv[]) {

    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <file 1> <file 2>\n";
		return 1;
    }
    Mat img_a = imread(argv[1], IMREAD_COLOR);   // Read the file
    Mat img_b = imread(argv[2], IMREAD_COLOR);   // Read the file

    if (img_a.empty()) {                          // Check for invalid input
        std::cerr << "Could not open or find the image: " << argv[1] << std::endl;
        return 1;
    }

    if (img_b.empty()) {                          // Check for invalid input
        std::cerr << "Could not open or find the image: " << argv[2] << std::endl;
        return 1;
    }

    imshow("Display windowA", img_a);                   // Show our image inside it.
    imshow("Display windowB", img_b);                   // Show our image inside it.

    // Use a relative path to specify the image file location
    // std::string image_path = "../imagens_PPM/airplane.ppm";  // Relative path to the image
    // std::string image_path2 = samples::findFile("../roniiii.jpeg");

    // Mat img = imread(image_path, IMREAD_COLOR);
    // Mat img2 = imread(image_path2, IMREAD_COLOR);

    // if (img.empty())
    // {
    //     std::cout << "Could not read the image: " << image_path << std::endl;
    //     return 1;
    // }

    // imshow("Display window", img);
    // imshow("Display window 2", img2);

    int k = waitKey(0); // Wait for a keystroke in the window

    if (k == 's')
    {
        imwrite("../imagens_PPM/airplane.ppm", img_a); // Save the image in the same directory
    }

    // extract a color channel from an image creating a single channel image with the result

    // reading and writing pixel by pixel creating a single channel image with the result


    return 0;
}
