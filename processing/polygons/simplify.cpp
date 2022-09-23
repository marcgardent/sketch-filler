//
// Created by Garbo on 23/09/2022.
//
#include <iostream>
#include "polygons.hpp"
namespace processing::polygon {

    static vector<Point> simplify(vector<Point> polygon) {
        vector<Point> ret;
        int modulo = polygon.size();
        for(size_t i=0; i<polygon.size();i++){
            Point A = polygon[i];
            Point B = polygon[(i+1) %modulo];
            Point2f B2f = toPoint2f(B);
            Point C = polygon[(i+2) % modulo];
            Point2f G = barycenter(A,B,C);
            Point2f BG= G - B2f;
            float dSquared = (BG.x*BG.x + BG.y*BG.y);
            if (dSquared >= 1){
                ret.push_back(B);
            }
        }
        return ret;
    }

    static vector<Point> simplifyAll(vector<Point> polygon) {
        vector<Point> current = polygon;
        vector<Point> ret;
        std::cout << "start simplifyAll" << std::endl;
        bool run = true;
        while(run) {
            vector<Point> next = simplify(current);
            std::cout << "  simplify:"<< current.size()<< "->"<< next.size()  << std::endl;
            int count  = current.size() - next.size();
            run = count > 0;
            current = next;
        }
        std::cout << "end simplifyAll" << std::endl;
        return ret;
    }
}