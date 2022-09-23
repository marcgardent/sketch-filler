#include "polygons.hpp"

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

#define FORWARD_STEP 1
#define BACKWARD_STEP -1

#define next_PROP 0
#define previous_PROP 1
#define first_child_PROP 2
#define parent_PROP 3

namespace processing::polygon {

    using namespace cv;
    using namespace std;
    using namespace points;



    RNG rng(12365);


    static vector<Point2f> walk(vector<Point> polygon, int seed, int direction, int step_count, float step_length) {
        vector<Point2f> ret;
        ret.push_back(Point2f(polygon[seed].x, polygon[seed].y));
        float walked = 0;
        int cursor = seed;
        int modulo = polygon.size();

        float journey = step_count * step_length;
        float remainder = 0;
        //std::cout << "start journey" << std::endl;
        while (journey > 0) {

            int next_cursor = (modulo + cursor + direction) % modulo;
            Point A = polygon[cursor % modulo];
            Point2f A2f = Point2f(A.x, A.y);
            Point B = polygon[next_cursor];
            Point AB = B - A;
            double magnitude = Magnitude(AB);
            Point2f unit = Normalize(AB);
            double progress = 0;

            //std::cout << "  start segment: magnitude:" << magnitude << std::endl;

            //generate checkpoint along the AB segment:
            while (progress + (step_length - remainder) < magnitude && journey > 0) {
                progress += step_length - remainder; //walk
                journey -= step_length; //consume the complete step
                remainder = 0; // clear remainder

                //add checkpoint
                ret.push_back(A2f + unit * progress);
                //std::cout << "    step: progress:" << progress << " length: " << step_length << "remainder" << remainder << " journey:" << journey << std::endl;
            }
            remainder = magnitude - progress;
            cursor = next_cursor;
            //std::cout << "  end segment: remainder:" << remainder << " cursor: " << cursor << std::endl;
        }
        return ret;
    }


    static Scalar rng_color() {
        return Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
    }

    void trace_flat_hierarchy(vector<Vec4i> hierarchy) {
        //get values
        for (size_t index = 0; index < hierarchy.size(); index++) {
            int next = hierarchy[index][0];
            int previous = hierarchy[index][1];
            int first_child = hierarchy[index][2];
            int parent = hierarchy[index][3];
            std::cout << "node(index=" << index << ","
                      << "next=" << next << ","
                      << "previous=" << previous << ","
                      << "first_child=" << first_child << ","
                      << "parent=" << parent << ")"
                      << std::endl;
        }
    }


    void trace_hierarchy(vector<Vec4i> hierarchy, size_t index, int level) {
        //get values
        int next = hierarchy[index][next_PROP];
        int previous = hierarchy[index][previous_PROP];
        int first_child = hierarchy[index][first_child_PROP];
        int parent = hierarchy[index][parent_PROP];

        //print node
        std::string s = "";
        s.append(level * 3, ' ');
        std::cout << s << "* node(" << index << ")" << std::endl;

        //navigate
        if (first_child != -1) {
            trace_hierarchy(hierarchy, first_child, level + 1);
        }
        if (next != -1) {
            trace_hierarchy(hierarchy, next, level);
        }
    }

    vector<size_t> get_fraternity(vector<Vec4i> hierarchy, size_t index) {
        //get values
        size_t next = index;
        vector<size_t> ret;
        while (next != -1) {
            ret.push_back(next);
            next = hierarchy[next][0];
        }
        return ret;
    }


    Point2f compute_barycenter(vector<Point> polygon, size_t seed) {
        bool PONDERATION = true;
        //https://stackoverflow.com/questions/45323590/do-contours-returned-by-cvfindcontours-have-a-consistent-orientation
        // Outer contours -> counter-clockwise
        // Inner contours -> clockwise
        int step_length = 1;
        int step_count = 10;
        vector<Point2f> forward_checkpoints = walk(polygon, seed, FORWARD_STEP, step_count, step_length);
        vector<Point2f> backward_checkpoints = walk(polygon, seed, BACKWARD_STEP, step_count, step_length);
        float x = 0;
        float y = 0;
        float weights = 0;
        float max = step_count + 1;
        float w = 0;
        for (size_t i = 0; i < forward_checkpoints.size(); i++) {
            w = PONDERATION ? max - i : 1;
            weights += w;
            x += forward_checkpoints[i].x * w;
            y += forward_checkpoints[i].y * w;
        }
        for (size_t i = 0; i < backward_checkpoints.size(); i++) {
            w = PONDERATION ? max - i : 1;
            weights += w;
            x += backward_checkpoints[i].x * w;
            y += backward_checkpoints[i].y * w;
        }
        return Point2f(x / weights, y / weights);
    }

    void debug(Mat source) {

        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(source, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
        Mat drawing = Mat::zeros(source.size(), CV_8UC3);

        trace_flat_hierarchy(hierarchy);
        trace_hierarchy(hierarchy, 0, 0);

        vector<size_t> outers = get_fraternity(hierarchy, 0);
        for (size_t i = 0; i < outers.size(); i++) {
            Scalar color = rng_color();
            auto outer = hierarchy[outers[i]];
            vector<size_t> inners = get_fraternity(hierarchy, outer[first_child_PROP]);


            for (size_t j = 0; j < inners.size(); j++) {
                vector<Point> polygon = contours[inners[j]];
//                auto br = compute_barycenter(polygon, 3);
//                line(drawing, polygon[3], Point(rint(br.x), rint(br.y)), Scalar(255,0,255), 1);

                for (size_t k = 0; k < polygon.size(); k++) {
                    auto br = compute_barycenter(polygon, k);
                    auto normal = Normalize(br - toPoint2f(polygon[k]));
                    line(drawing, polygon[k], polygon[k] + roundPoint2f(normal * 100), Scalar(255, 0, 255), 1);
                    line(drawing, polygon[k], polygon[k] + roundPoint2f(normal * (-100)), Scalar(0, 255, 0), 1);
                }
                drawContours(drawing, contours, (int) inners[j], Scalar(200, 200, 200), 1, LINE_8, hierarchy, 0);
            }
            drawContours(drawing, contours, (int) outers[i], Scalar(255, 255, 255), 1, LINE_8, hierarchy, 0);
            //process_contours_as_edges(contours, outers[i],inners);
        }
        imshow("coutours", drawing);
        return;
    }
}
