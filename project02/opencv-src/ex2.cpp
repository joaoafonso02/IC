#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <input_image> <output_image> <option>" << std::endl;
        cerr << "\nOptions:\n";
        cerr << "  -negative                    Creates the negative version of an image\n";
        cerr << "  -rotate_horizontally         Creates an horizontally mirrored version of an image\n";
        cerr << "  -rotate_vertically           Creates a vertically mirrored version of an image\n";
        cerr << "  -rotate <multiple of 90º>    Rotates an image by a multiple of 90º\n";
        cerr << "  -increase_light <constant>   Increases (more light) of an image\n";
        cerr << "  -decrease_light <constant>   Decreases (less light) of an image\n";
        cerr << "  -black_white                 Creates a black and white version of an image\n";
        
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
        for (int i = 0; i < image.rows; i++) {
            for (int j = 0; j < image.cols; j++) {
                for (int c = 0; c < image.channels(); c++) {
                    image.at<Vec3b>(i, j)[c] = 255 - image.at<Vec3b>(i, j)[c];
                }
            }
        }
        imshow("New Negative Image", image);
    } else if (option == "-rotate_horizontally") {
        for (int i = 0; i < image.rows; i++) {
            for (int j = 0; j < image.cols / 2; j++) {
                for (int c = 0; c < image.channels(); c++) {  // Loop through the three color channels
                    Vec3b temp = image.at<Vec3b>(i, j);
                    image.at<Vec3b>(i, j)[c] = image.at<Vec3b>(i, image.cols - j - 1)[c];
                    image.at<Vec3b>(i, image.cols - j - 1)[c] = temp[c];
                }
            }
        }
        imshow("Horizontally Mirrored Image", image);

    } else if (option == "-rotate_vertically") {
        for (int i = 0; i < image.rows / 2; i++) {
            for (int j = 0; j < image.cols; j++) {
                for (int c = 0; c < image.channels(); c++) {  // Loop through the three color channels
                    Vec3b temp = image.at<Vec3b>(i, j);
                    image.at<Vec3b>(i, j)[c] = image.at<Vec3b>(image.rows - i - 1, j)[c];
                    image.at<Vec3b>(image.rows - i - 1, j)[c] = temp[c];
                }
            }
        }
        imshow("Vertically Mirrored Image", image);
    } else if (option == "-rotate") {
        int rotation = stoi(argv[4]);
        if (rotation % 90 != 0) {
            std::cerr << "Error: Invalid rotation. Use a multiple of 90º." << std::endl;
            return 1;
        }
        int num_rotations = (rotation / 90) % 4;
        cout << num_rotations;

        for (int i = 0; i < image.rows; i++) {
            for (int j = 0; j < image.cols * image.channels(); j++) {
                for (int c = 0; c < image.channels(); c++) {
                    if (num_rotations == 1) {
                        image.at<Vec3b>(i, j)[c] = image.at<Vec3b>(image.rows - j - 1, i)[c];
                    } else if (num_rotations == 2) {
                        image.at<Vec3b>(i, j)[c] = image.at<Vec3b>(image.rows - i - 1, image.cols - j - 1)[c];
                    } else if (num_rotations == 3) {
                        image.at<Vec3b>(i, j)[c] = image.at<Vec3b>(j, image.cols - i - 1)[c];
                    }
                }     
            }
        }
        imshow(to_string(rotation) + " degrees Rotation Image", image);

    } else if (option == "-increase_light") {
        if (argc <= 4) {
            std::cerr << "Error: Empty constant. Use a number (argv[4])." << std::endl;
            return 1;
        }
        for (int i = 0; i < image.rows; i++) {
            for (int j = 0; j < image.cols; j++) {
                for (int c = 0; c < image.channels(); c++) {
                    int new_channel_value = image.at<Vec3b>(i, j)[c] + stoi(argv[4]); // Increase brightness by 100
                    image.at<Vec3b>(i, j)[c] = (new_channel_value > 255) ? 255 : uchar(new_channel_value);
                    // 255 is the maximum 8 bit number
                }
            }
        }
        imshow("Increased Light Image", image);
        
    } else if (option == "-decrease_light") {
        for (int i = 0; i < image.rows; i++) {
            for (int j = 0; j < image.cols; j++) {
                for (int c = 0; c < image.channels(); c++) {
                    int new_channel_value = image.at<Vec3b>(i, j)[c] - stoi(argv[4]); // Increase brightness by 100
                    image.at<Vec3b>(i, j)[c] = (new_channel_value < 0) ? 0 : uchar(new_channel_value);
                    // 0 is the minimum 8 bit number
                }
            }
        }
        imshow("Decreased Light Image", image);

    } else if (option == "-black_white") {
        // https://e2eml.school/convert_rgb_to_grayscale#:~:text=An%20intuitive%20way%20to%20convert,into%20a%20reasonable%20gray%20approximation.
        // for each pixel, take the average of the red, green, and blue pixel values to get the grayscale value
        for (int i = 0; i < image.rows; i++) {
            for (int j = 0; j < image.cols; j++) {
                Vec3b pixel = image.at<Vec3b>(i, j);

                // Calculate the average of the three channels
                int average = (pixel[0] + pixel[1] + pixel[2]) / 3;

                image.at<Vec3b>(i, j)[0] = average;
                image.at<Vec3b>(i, j)[1] = average;
                image.at<Vec3b>(i, j)[2] = average;
            }
        }
        imshow("Black and White Image", image);

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
