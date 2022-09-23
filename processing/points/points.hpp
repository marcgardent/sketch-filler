#ifndef SKETCH_FILLER_POINTS_HPP
#define SKETCH_FILLER_POINTS_HPP

#include <opencv2/core/types.hpp>

namespace processing::points {
    using namespace cv;

    static int Dot(Point A, Point B) {
        return A.x * B.x + A.y * B.y;
    }

    static int Cross(Point A, Point B) {
        return A.x * B.y - A.y * B.x;
    }

    static double Magnitude(Point A) {
        return sqrt(A.x * A.x + A.y * A.y);
    }

    static Point2f Normalize(Point A) {
        float length = sqrt(A.x * A.x + A.y * A.y);
        Point2f normalized = Point2f(static_cast<float>(A.x) / length, static_cast<float>(A.y) / length);
        return normalized;
    }
    static Point2f Normalize2f(Point2f A) {
        float length = sqrt(A.x * A.x + A.y * A.y);
        Point2f normalized = Point2f(A.x / length, A.y / length);
        return normalized;
    }

    static Point roundPoint2f(Point2f A) {
        return Point(rint(A.x), rint(A.y));
    }

    static Point2f toPoint2f(Point A) {
        return Point(A.x, A.y);
    }

    static Point2f barycenter(Point A, Point B, Point C) {
        float x = static_cast<float>(A.x + B.x + C.x) / 3.0;
        float y = static_cast<float>(A.y + B.y + C.y) / 3.0;
        return Point2f(x, y);
    }
}

#endif //SKETCH_FILLER_POINTS_HPP
