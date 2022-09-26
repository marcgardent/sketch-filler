#ifndef SKETCH_FILLER_CONTOURS_H
#define SKETCH_FILLER_CONTOURS_H
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "points.hpp"

namespace processing::polygon {

    using namespace cv;
    using namespace std;
    using namespace processing::points;

    void debug(Mat source);
    vector<Point> simplifyAll(vector<Point> polygon);

}
#endif