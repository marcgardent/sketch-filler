#include "qopencvwidget.h"
#include <opencv2/core/mat.hpp>

// https://doc.qt.io/qt-5/qtquick-scenegraph-openglunderqml-example.html

QOpenCVWidget::QOpenCVWidget(QWidget *parent) : QOpenGLWidget(parent) {
    this->parent = parent;
    hidden = false;
    bgColor = QColor::fromRgb(0xe0, 0xdf, 0xe0);
    original =cv::Mat(cv::Size(this->height(), this->width()), CV_8UC3);
    format = GL_BGR;
    drawing = false;
    setMouseTracking(true);
    mouseX = 0;
    mouseY = 0;
    initializeGL();
}
QOpenCVWidget::~QOpenCVWidget(){

}
void QOpenCVWidget::initializeGL() {
    // https://stackoverflow.com/questions/16809833/opencv-image-loading-for-opengl-texture

    //use fast 4-byte alignment (default anyway) if possible
    glPixelStorei(GL_UNPACK_ALIGNMENT, (original.step & 3) ? 1 : 4);

    //set length of one complete row in data (doesn't need to equal image.cols)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, original.step/original.elemSize());



    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, this->width(), this->height(), 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glGenTextures(3, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width(), this->height(), 0, GL_BGR, GL_UNSIGNED_BYTE, NULL);
    glDisable(GL_TEXTURE_2D);


}


void QOpenCVWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    if (!hidden) {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0f, this->width(), this->height(), 0.0f, 0.0f, 1.0f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_BGR, original.rows, original.cols, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE,
                     original.ptr());

        glBegin(GL_QUADS);
        glTexCoord2i(0, 1);
        glVertex2i(0, this->height());
        glTexCoord2i(0, 0);
        glVertex2i(0, 0);
        glTexCoord2i(1, 0);
        glVertex2i(this->width(), 0);
        glTexCoord2i(1, 1);
        glVertex2i(this->width(), this->height());
        glEnd();

        glFlush();
    }

}


void QOpenCVWidget::resizeGL(int width, int height) {

    glViewport(0, 0, this->width(), this->height());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, this->width(), this->height(), 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}