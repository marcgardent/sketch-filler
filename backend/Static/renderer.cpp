#include "renderer.h"

#include <QSGSimpleTextureNode>
#include <QSGImageNode>
#include <QDebug>

#include <opencv2/core/types.hpp>
#include <opencv2/imgproc.hpp>

#include "Filler.hpp"

Renderer::Renderer(QQuickItem *parent) : QQuickItem(parent) {
    // m_image = new QImage("T:\\data\\sketch-filler2\\chalenges\\case-03.png");
    m_image = new QImage(
            "G:\\Mon Drive\\Ludopant Creative Studio\\Ludopant Galaxy\\order #2 - BD CJS S1E1 Paris\\Order 2.1\\2022-09-16 release Ch1&2\\Page finale 1.tiff");
    m_image->convertTo(QImage::Format_BGR888);

    setFlag(QQuickItem::ItemHasContents, true);
    setAcceptedMouseButtons(Qt::AllButtons);
    setScale(0.9);
}

Renderer::~Renderer() {
}

QSGNode *Renderer::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *updatePaintNodeData) {
    auto node = dynamic_cast<QSGImageNode *>(oldNode);

    if (!node) {
        node = window()->createImageNode();
        lastTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    }

    //load texture
    if (dirty_render) {
        QSGTexture *texture = window()->createTextureFromImage(*m_image, QQuickWindow::TextureHasAlphaChannel);
        node->setTexture(texture);
        node->setOwnsTexture(true);
        dirty_render = false;
    }

    // layout
    {
        auto bound = boundingRect();
        int height;
        int width;
        if ((double) m_image->width() / (double) m_image->height() > (double) bound.width() / (double) bound.height()) {
            height = (int) (bound.width() * (double) m_image->height() / (double) m_image->width());
            width = bound.width();
        } else {
            height = bound.height();
            width = (int) (bound.height() * (double) m_image->width() / (double) m_image->height());
        }
        node->setRect(bound.width() / 2 - width / 2, bound.height() / 2 - height / 2, width, height);
    }

    node->markDirty(QSGNode::DirtyForceUpdate);
    fps();
    return node;
}

cv::Mat Renderer::get_as_mat() {
    return cv::Mat(m_image->height(), m_image->width(),
                   CV_8UC3,
                   m_image->bits(),
                   static_cast<size_t>(m_image->bytesPerLine())
    );
}

void Renderer::fps() {
    auto currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    if (currentTime > lastTime) {
        auto fps = (currentTime - lastTime);
        lastTime = currentTime;
        qDebug() << "FPS: " << fps;
    }
}

void Renderer::fill_request() {
    cv::Mat mat = get_as_mat();
    //cv::line(mat, cv::Point(0,0), cv::Point(256,256), rng_argb(), 10);

    cv::Mat out = processing::filler::fill(mat);
    out.copyTo(mat);
    requestRender();
}

void Renderer::requestRender() {
    dirty_render = true;
    update();
}


void Renderer::mousePressEvent(QMouseEvent *event) {
    qDebug() << "mousePressEvent";
    event->accept();
}

void Renderer::mouseMoveEvent(QMouseEvent *event) {
    qDebug() << "mouseMoveEvent";
    event->accept();
}

void Renderer::mouseReleaseEvent(QMouseEvent *event) {
    qDebug() << "mouseReleaseEvent";
    event->accept();
}

void Renderer::wheelEvent(QWheelEvent *event) {
    qDebug() << "wheelEvent pixelDelta:" << event->pixelDelta() << "angleDelta:" << event->angleDelta();
    event->accept();

    double delta=((double)event->angleDelta().y()/1200.0);
    qDebug() << "zoom:" << delta;
    setScale(scale()+delta);
}
