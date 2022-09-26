#ifndef SKETCHFILLER_MORPHOLOGICAL_H
#define SKETCHFILLER_MORPHOLOGICAL_H
#include <opencv2/core/mat.hpp>

namespace processing::morphologicals {
    using namespace cv;

    void detect_gap_areas( InputArray src, OutputArray dst, int distance);

}


#endif //SKETCHFILLER_MORPHOLOGICAL_H
