
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    // Read the image file
    Mat image = imread("image.png");

    // Check for failure
    if (image.empty()) 
    {
        cout << "Could not open or find the image" << endl;
        cin.get(); //wait for any key press
        return -1;
    }

    // Chroma subsample the image
    Mat chroma_image;
    cvtColor(image, chroma_image, COLOR_BGR2YCrCb);

    // Split the channels
    vector<Mat> channels;
    split(chroma_image, channels);

    // Get the rows and columns
    int rows = channels[0].rows;
    int cols = channels[0].cols;

    // Perform chroma subsampling
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            // Every 4th pixel in the Cr and Cb channels
            if ((i % 4 == 0) && (j % 4 == 0))
            {
                channels[1].at<uchar>(i, j) = channels[1].at<uchar>(i + 1, j);
                channels[2].at<uchar>(i, j) = channels[2].at<uchar>(i + 1, j);
            }
        }
    }

    // Merge the channels
    Mat merge_image;
    merge(channels, merge_image);

    // Convert back to BGR
    Mat final_image;
    cvtColor(merge_image, final_image, COLOR_YCrCb2BGR);

    // Define window names for original and final images
    String window_name = "Original Image";
    String window_name_final = "Chroma Subsampled Image";

    // Create windows with above names
    namedWindow(window_name, WINDOW_NORMAL);
    namedWindow(window_name_final, WINDOW_NORMAL);

    // Resize the windows
    resizeWindow(window_name, 600, 600);
    resizeWindow(window_name_final, 600, 600);

    // Show images inside the created windows.
    imshow(window_name, image);
    imshow(window_name_final, final_image);

    // Wait for key press before closing the windows
    waitKey(0);

    destroyAllWindows(); //Destroy all open windows

    return 0;
}