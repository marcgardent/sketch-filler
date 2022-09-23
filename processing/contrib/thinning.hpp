#ifndef SKETCH_FILLER_THINNING_H
#define SKETCH_FILLER_THINNING_H
#include <opencv2/core/mat.hpp>

namespace cv {
    namespace ximgproc {

        enum ThinningTypes {
            THINNING_ZHANGSUEN = 0, // Thinning technique of Zhang-Suen
            THINNING_GUOHALL = 1  // Thinning technique of Guo-Hall
        };

        void thinning( InputArray src, OutputArray dst, int thinningType = THINNING_ZHANGSUEN);
    }
}




#endif //SKETCH_FILLER_THINNING_H
