#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "thinning.hpp"
#include "polygons.hpp"
#include "pixels.hpp"
#include "points.hpp"
#include "morphologicals.hpp"

using namespace cv;
using namespace std;

// #define SOURCE_PATH "G:\\Mon Drive\\Ludopant Creative Studio\\Ludopant Galaxy\\order #2 - BD CJS S1E1 Paris\\Order 2.1\\2022-09-16 release Ch1&2\\Page finale 1.tiff"
#define SOURCE_PATH "T:\\data\\sketch-filler2\\chalenges\\chalenge-01.png"

int main()
{

    std::cout << "Open: " << SOURCE_PATH << std::endl;
    Mat source = cv::imread(SOURCE_PATH, IMREAD_UNCHANGED);

    if(source.empty())
    {
        std::cout << "Could not read the image: " << SOURCE_PATH << std::endl;
    }
    else {
        imshow("Original", source);

        // Step Clean up the image
        Mat working_mat;
        std::cout << "source: channels:" << source.channels() << " depth:" << source.depth() << std::endl;
        cvtColor(source, working_mat, COLOR_BGR2GRAY);

        std::cout << "GRAYSCALE channels: " << working_mat.channels() << " depth:" << working_mat.depth() << std::endl;
        threshold(working_mat, working_mat, 100, 255, THRESH_BINARY);
        bitwise_not(working_mat, working_mat);
        std::cout << "Binary channels: " << working_mat.channels() << " depth:" << working_mat.depth() << std::endl;

        {
            // supprimer aplat
            int erosion_size = 5;
            Mat erosion_dst;
            Mat element = getStructuringElement(MORPH_ELLIPSE,
                                                Size(2 * erosion_size + 1, 2 * erosion_size + 1),
                                                Point(erosion_size, erosion_size));
            erode(working_mat, erosion_dst, element);
            bitwise_not(erosion_dst, erosion_dst);
            imshow("detect flattened", erosion_dst);
            bitwise_and(working_mat, erosion_dst, working_mat);
            imshow("Cleaned", working_mat);
        }

        // Morph strategy
        {
            //final output
            Mat result;
            cvtColor(working_mat, result,COLOR_GRAY2RGB);

            //hotspot
            Mat hotspot5;
            processing::morphologicals::detect_gap_areas(working_mat,hotspot5, 5);
            result.setTo(Scalar(128,128,255),hotspot5 );

            Mat hotspot2;
            processing::morphologicals::detect_gap_areas(working_mat,hotspot2, 4);
            result.setTo(Scalar(0,0,255),hotspot2 );

            //backbone
            Mat backbone = Mat::zeros( working_mat.size(), CV_8UC1 );
            cv::ximgproc::thinning(working_mat, backbone, cv::ximgproc::THINNING_ZHANGSUEN);
            result.setTo(Scalar(0,128,0),backbone );


            imshow("hotspot", result);
        }

        //  contours strategy
        {
            processing::polygon::debug(working_mat);
        }

        {
            // Step thinning strategy
            Mat thinning_output = Mat::zeros( working_mat.size(), CV_8UC1 );
            cv::ximgproc::thinning(working_mat, thinning_output, cv::ximgproc::THINNING_ZHANGSUEN);

            imshow( "edges output", thinning_output);
            processing::pixels::debug_thinning_strategy(thinning_output, working_mat);
        }

        //the End
        int k = waitKey(0);
    }
}