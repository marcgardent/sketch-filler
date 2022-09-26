#include "morphologicals.hpp"
#include <opencv2/core.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

namespace processing::morphologicals {

    int erosion_type = MORPH_ELLIPSE;

    void detect_gap_areas(InputArray input, OutputArray output, int distance){
        Mat processed = input.getMat().clone();

        Mat element = getStructuringElement( erosion_type,
                                             Size( 2*distance + 1, 2*distance+1 ),
                                             Point( distance, distance ) );

        dilate( processed, processed, element );
        erode( processed, processed, element );

        bitwise_xor(input, processed,processed );

        output.assign(processed);
    }
}