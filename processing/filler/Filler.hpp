//
// Created by Garbo on 28/09/2022.
//

#ifndef SKETCHFILLER_FILLER_HPP
#define SKETCHFILLER_FILLER_HPP

#include <opencv2/core/mat.hpp>


namespace processing::filler {

    using namespace cv;
    Mat fill(InputArray src);
}

#endif //SKETCHFILLER_FILLER_HPP
