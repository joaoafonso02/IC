#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <input_image> <output_image> <option>" << std::endl;
        cerr << "\nOptions:\n";
        cerr << "  -negative                    Creates the negative version of an image\n";
        cerr << "  -rotate_horizontally         Creates an horizontally mirrored version of an image\n";
        cerr << "  -rotate_vertically           Creates a vertically mirrored version of an image\n";
        cerr << "  -rotate <multiple of 90º>    Rotates an image by a multiple of 90º\n";
        cerr << "  -increase_light              Increases (more light) of an image\n";
        cerr << "  -decrease_light              Decreases (less light) of an image\n";
        
        return 1;
    }

    // Load the input image
    cv::Mat inputImage = cv::imread(argv[1]);

    if (inputImage.empty()) {
        std::cerr << "Error: Unable to load input image." << std::endl;
        return 1;
    }

    // Get the selected option
    std::string option = argv[3];

    if (option == "-negative") {
        // Implement the negative image creation
        // ...

    } else if (option == "-rotate_horizontally") {
        // Implement horizontal mirror
        // ...

    } else if (option == "-rotate_vertically") {
        // Implement vertical mirror
        // ...

    } else if (option == "-rotate_mulx90") {
        // Implement image rotation by 90 degrees
        // ...

    } else if (option == "-increase_light") {
        // Implement increasing intensity values
        // ...

    } else if (option == "-decrease_light") {
        // Implement decreasing intensity  values
        // ...

    } else {
        std::cerr << "Invalid option. Use -negative | -rotate_horizontally | -rotate_vertically |"
          << " -rotate <multiple of 90 degrees> | -increase_light | -decrease_light" << std::endl;

        return 1;
    }

    // Save the modified image to the output file
    cv::imwrite(argv[2], inputImage);

    return 0;
}
