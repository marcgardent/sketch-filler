#include <iostream>
#include <sstream>
#include <string>
#include <filesystem>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "thinning.hpp"
#include "polygons.hpp"
#include "pixels.hpp"
#include "points.hpp"
#include "morphologicals.hpp"

namespace fs = std::filesystem;
using namespace cv;

int brush_size = 5;

int main(int argc, char *argv[]) {
    if(argc>=2){
        std::string in_file = argv[1];


        std::cout << "Open: " << in_file << std::endl;

        Mat source = cv::imread(in_file, IMREAD_UNCHANGED);
        if(!source.empty())
        {

            // Step Clean up the image
            Mat working_mat;
            {
                cvtColor(source, working_mat, COLOR_BGR2GRAY);
                threshold(working_mat, working_mat, 100, 255, THRESH_BINARY);
                bitwise_not(working_mat, working_mat);
            }

            {
                //final output
                Mat result;
                cvtColor(working_mat, result,COLOR_GRAY2RGB);

                //hotspot
                Mat hotspot5;
                processing::morphologicals::detect_gap_areas(working_mat,hotspot5, 5);
                result.setTo(Scalar(255,255,0),hotspot5 );

                bitwise_not(result, result);

                //save
                std::stringstream ss;
                fs::path parsed = fs::path(in_file);
                fs::path out_path =parsed.parent_path();
                out_path /= parsed.stem();
                out_path += "_outline_gap.png";
                imwrite(out_path.string(), result);
                std::cout << "Save: " << out_path << std::endl;
            }
        }
        else{
            std::cout << "Could not read the image: " << in_file << std::endl;
        }
    } else{
        std::cout << "add the file path to command line!" << std::endl;
    }

}