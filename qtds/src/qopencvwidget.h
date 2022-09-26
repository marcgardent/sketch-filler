#ifndef QOPENCV_WIDGET_H
#define QOPENCV_WIDGET_H
#include <QtOpenGLWidgets>
#include <opencv2/core/mat.hpp>

class QOpenCVWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    QOpenCVWidget(QWidget *parent);
    ~QOpenCVWidget();
    void paintGL();
    void initializeGL();
    void resizeGL(int,int);
    bool drawing;

private:
    cv::Mat original;
    GLenum format;
    GLuint texture;
    QColor bgColor;
    char* name;
    bool hidden;
    int startX,startY,endX,endY;
    QList<QPointF*> slopes;
    QWidget* parent;
    int mouseX,mouseY;

};

#endif // QOPENCV_WIDGET_H