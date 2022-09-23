#include "pixels.hpp"
#include <vector>
#include <algorithm>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

namespace processing::pixels {
    using namespace cv;
    using namespace std;

/**
 * | 9 | 2 | 3 |
 * | 8 | 1 | 4 |
 * | 7 | 6 | 5 |
 */
    struct Window3x3 {
        int x;
        int y;
        bool p1 = false; // Center
        bool p2 = false; // Top
        bool p3 = false; // Corner
        bool p4 = false; // Left
        bool p5 = false; // Corner
        bool p6 = false; // Bottom
        bool p7 = false; // Corner
        bool p8 = false; // right
        bool p9 = false; // Corner

        int countNeighbor() {
            return this->p2 + this->p3 + this->p4 + this->p5 + this->p6 + this->p7 + this->p8 + this->p9;
        }

        void debug() {
            std::cout << "[x:" << this->x << ", y:" << this->y << "]"
                      << " p1=" << this->p1
                      << " p2=" << this->p2
                      << " p3=" << this->p3
                      << " p4=" << this->p4
                      << " p5=" << this->p5
                      << " p6=" << this->p6
                      << " p7=" << this->p7
                      << " p8=" << this->p8
                      << " p9=" << this->p9
                      << std::endl;
        }
    };


    static bool getSafe(Mat &matrix, int x, int y) {
        //(y:row, x:col)!!!
        bool out = x < 0 || y < 0 || x >= matrix.cols || y >= matrix.rows;
        if (out) {
            return false;
        } else {
            return matrix.at<uchar>(y, x) == 0xff;
        }
    }

    void populate_connections(Mat &matrix, vector<Point> &ret, int x, int y) {
        if (getSafe(matrix, x, y)) {
            ret.push_back(Point(x, y));
        }
    }

    vector<Point> get_connections(Mat &matrix, int x, int y) {
        vector<Point> ret;

        //Hortogonal
        populate_connections(matrix, ret, x, y - 1);
        populate_connections(matrix, ret, x + 1, y);
        populate_connections(matrix, ret, x - 1, y);
        populate_connections(matrix, ret, x, y + 1);

        // Diagonal
        populate_connections(matrix, ret, x - 1, y + 1);
        populate_connections(matrix, ret, x + 1, y + 1);
        populate_connections(matrix, ret, x + 1, y - 1);
        populate_connections(matrix, ret, x - 1, y - 1);

        return ret;
    }

/**
 * | 9 | 2 | 3 |
 * | 8 | 1 | 4 |
 * | 7 | 6 | 5 |
 */
    Window3x3 get_3x3(Mat &matrix, int x, int y) {
        Window3x3 ret;
        ret.x = x;
        ret.y = y;
        ret.p1 = getSafe(matrix, x, y);
        ret.p2 = getSafe(matrix, x, y - 1);
        ret.p3 = getSafe(matrix, x + 1, y - 1);
        ret.p4 = getSafe(matrix, x + 1, y);
        ret.p5 = getSafe(matrix, x + 1, y + 1);
        ret.p6 = getSafe(matrix, x, y + 1);
        ret.p7 = getSafe(matrix, x - 1, y + 1);
        ret.p8 = getSafe(matrix, x - 1, y);
        ret.p9 = getSafe(matrix, x - 1, y - 1);
        return ret;
    }

    vector<Point> get_path(Mat &matrix, int x, int y, int iterations) {
        vector<Point> ret;
        ret.push_back(Point(x, y));
        std::cout << "Start Path" << std::endl;

        //debug
        Window3x3 window = get_3x3(matrix, x, y);
        window.debug();
        bool connection = true;

        for (int n = 0; n < iterations && connection; n++) {
            vector<Point> connections = get_connections(matrix, x, y);
            vector<Point> next;

            for (int i = 0; i < connections.size(); ++i) {
                for (int j = 0; j < ret.size(); ++j) {
                    auto found = std::find(ret.begin(), ret.end(), connections[i]);
                    if (found < ret.end()) {
                        connections.erase(std::next(connections.begin(), i));
                        i--;
                        break;
                    }
                }
            }
            connection = connections.size() > 0;
            if (connection) {
                ret.push_back(connections[0]);
                x = connections[0].x;
                y = connections[0].y;
                //debug
                window = get_3x3(matrix, x, y);
                window.debug();
            }
        }
        return ret;
    }


    void processing::pixels::debug_thinning_strategy(Mat &thinning_output, Mat &working_mat) {
        vector<Point> nodes;
        cv::findNonZero(thinning_output, nodes);
        std::cout << "number of nodes: " << nodes.size() << std::endl;
        Mat cut_output = Mat::zeros(working_mat.size(), CV_8UC3);
        for (size_t i = 0; i < nodes.size(); i++) {
            Point node = nodes[i];
            Window3x3 win = get_3x3(thinning_output, node.x, node.y);
            Vec3b &color = cut_output.at<Vec3b>(node.y, node.x);  // (y:row, x:col)!
            int neighbors = win.countNeighbor();
            if (neighbors == 1) { color[2] = 255; }
            else if (neighbors == 2) { color[0] = 255; }
            else if (neighbors == 3) { color[1] = 255; }
            else if (neighbors == 4) {
                color[0] = 255;
                color[1] = 255;
            }
            else {
                color[0] = 255;
                color[1] = 255;
                color[2] = 255;
            }
            if (neighbors == 1) {
                vector<Point> path = get_path(thinning_output, node.x, node.y, 16);
                Point first = path[0];
                Point last = path[path.size() - 1];
                Point direction = first - last;
/* extrapole */ {
                    float length = sqrt(direction.x * direction.x + direction.y * direction.y);
                    Point2f normalized = Point2f(static_cast<float>(direction.x) / length,
                                                 static_cast<float>(direction.y) / length);
                    for (int i = 1; i < 20; i++) {
                        Point next = first + Point(rint(normalized.x * i), rint(normalized.y * i));
                        bool hit = getSafe(thinning_output, next.x, next.y);

                        Vec3b &color = cut_output.at<Vec3b>(next.y, next.x);
                        color[0] = 128;
                        color[1] = 128;
                        color[2] = 128;

                        if (hit) {
                            std::cout << "Closed: " << next.x << ", " << next.y << std::endl;
                            circle(cut_output, first, 4, Scalar(255, 255, 255), 1);
                            circle(cut_output, next, 4, Scalar(255, 255, 255), 1);
                            break;
                        }
                    }
                }
            }
        }
        imshow("cut_output", cut_output);
    }

}