#include "Filler.hpp"
#include <opencv2/core/check.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include <opencv2/highgui.hpp>
#include <iostream>

namespace processing::filler {
    using namespace cv;
    cv::RNG rng(12365);

    inline cv::Scalar rng_bgra() {

        return  cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255),0);
    }

    void black_mask(InputArray bgra_source, OutputArray gray_dst) {
        CV_CheckTypeEQ(bgra_source.type(), CV_8UC3, "");
        Mat working_mat;
        cvtColor(bgra_source, working_mat, COLOR_BGRA2GRAY);
        threshold(working_mat, working_mat, 100, 255, THRESH_BINARY);
        bitwise_not(working_mat, working_mat);
        gray_dst.assign(working_mat);
    }

    void remove_solid_colors(InputArray gray_source, OutputArray dst) {
        CV_CheckTypeEQ(gray_source.type(), CV_8UC1, "");
        Mat ret_mat;
        int erosion_size = 5;
        Mat erosion_dst;
        Mat element = getStructuringElement(MORPH_ELLIPSE, Size(2 * erosion_size + 1, 2 * erosion_size + 1),
                                            Point(erosion_size, erosion_size));

        erode(gray_source, erosion_dst, element);
        bitwise_not(erosion_dst, erosion_dst);
        bitwise_and(gray_source, erosion_dst, ret_mat);
        dst.assign(ret_mat);
    }

    void fill_all(Mat black_layer, Mat color_layer) {
        cv::copyMakeBorder(black_layer, black_layer, 1, 1, 1, 1, cv::BORDER_REPLICATE);
        //cv::imshow("black_layer", black_layer);
        uchar fillValue = 255;
        auto flag = 4 | (fillValue << 8);
        int floodFillCount=0;
        for (int i = 0; i < color_layer.rows - 1; i++) {
            for (int j = 0; j < color_layer.cols - 1; j++) {
                uchar pixel = black_layer.at<uchar>(i+1, j+1);
                if(pixel==0){
                    Scalar color  = rng_bgra();
                    floodFill(color_layer, black_layer, Point(j,i), color, 0, Scalar(0,0,0), Scalar(0,0,0), flag);
                    floodFillCount++;
                }
            }
        }
        std::cout << "floodFillCount: " << floodFillCount << std::endl;

    }

    Mat fill(Mat src) {

        Mat black_layer;
        Mat object_id_layer;

        black_mask(src, black_layer);
        cvtColor(black_layer, object_id_layer, COLOR_GRAY2BGR);
        fill_all(black_layer,object_id_layer);

        //render.setTo(Scalar(0, 255, 0, 0), black_layer);

        object_id_layer = object_id_layer.mul(src/255);

        return object_id_layer;
    }
}