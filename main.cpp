#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

#define FILE "G:\\Mon Drive\\Ludopant Creative Studio\\Ludopant Galaxy\\order #2 - BD CJS S1E1 Paris\\Order 2.1\\2022-09-16 release Ch1&2\\Page finale 1.tiff"

using namespace cv;

int main()
{
    std::cout << "Open: " << FILE << std::endl;
    Mat img = cv::imread(FILE, IMREAD_UNCHANGED);

    if(img.empty())
    {
        std::cout << "Could not read the image: " << FILE << std::endl;
    }
    else{
        imshow("Display window", img);
        int k = waitKey(0); // Wait for a keystroke in the window
        if(k == 's')
        {
            imwrite("starry_night.png", img);
        }
    }
}