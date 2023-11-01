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
    Mat image = imread(argv[1]);

    if (image.empty()) {
        cerr << "Error: Unable to load input image." << endl;
        return 1;
    }

    // Get the selected option
    string option = argv[3];

    if (option == "-negative") {
        for(int i = 0; i < image.rows; i++) {
            for(int j = 0; j < image.cols * image.channels(); j++) { // or instead just image.cols
                image.at<uchar>(i, j) = 255 - image.at<uchar>(i, j); // or image.at<uchar>(i, j) = 255 - image.at<uchar>(i, j);
            }
        }
        imshow("New Negative Image", image);

    } else if (option == "-rotate_horizontally") {
        for (int i = 0; i < image.rows; i++) {
            for (int j = 0; j < image.cols / 2; j++) { // split half the image vertically (col), since its the vertical axe we trying to mirror
                for (int c = 0; c < image.channels(); c++) { // go to the three color channels r,g,b
                    uchar temp = image.at<uchar>(i, j * image.channels() + c);
                    image.at<uchar>(i, j * image.channels() + c) = image.at<uchar>(i, (image.cols - j - 1) * image.channels() + c);
                    image.at<uchar>(i, (image.cols - j - 1) * image.channels() + c) = temp;
                }
            }
        }
        imshow("Horizontally Mirrored Image", image);
    } else if (option == "-rotate_vertically") {
       for (int i = 0; i < image.rows / 2; i++) { // // split half the image horizontally (row), since its the vertical axe we trying to mirror
            for (int j = 0; j < image.cols * image.channels(); j++) {
                for (int c = 0; c < image.channels(); c++) { // go to the three color channels r,g,b
                    uchar temp = image.at<uchar>(i, j);
                    image.at<uchar>(i, j) = image.at<uchar>(image.rows - i - 1, j);
                    image.at<uchar>(image.rows - i - 1, j) = temp;
                }
            }
        }

        imshow("Vertically Mirrored Image", image);

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
    imwrite(argv[2], image);

    cout << "Process Completed. Saved as " << argv[2] << std::endl;

    waitKey(0); // Wait for user input

    return 0;
}
