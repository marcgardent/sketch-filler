#ifndef SKETCH_FILLER_PIXELS_HPP
#define SKETCH_FILLER_PIXELS_HPP
#include "opencv2/imgproc.hpp"

namespace processing::pixels {

    using namespace cv;
    using namespace std;
    void debug_thinning_strategy(Mat& thinning_output, Mat& working_mat);
}
#endif
