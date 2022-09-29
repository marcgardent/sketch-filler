//
// Created by Garbo on 26/09/2022.
//

#ifndef SKETCHFILLER_RENDERER_H
#define SKETCHFILLER_RENDERER_H

#include <QQuickItem>
#include <QImage>
#include <QPixmap>
#include <QtQml>
#include <opencv2/core/mat.hpp>
#include "Filler.hpp"


class Renderer : public QQuickItem {
Q_OBJECT
QML_ELEMENT
public:
    explicit Renderer(QQuickItem *parent = nullptr);
    ~Renderer();
    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *) override;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    bool dirty_render=true;
    qint64 lastTime;
    QImage* m_image;
    cv::Mat get_as_mat();
    void fps();
    void requestRender();

public slots:
    void  fill_request();
};


#endif //SKETCHFILLER_RENDERER_H
