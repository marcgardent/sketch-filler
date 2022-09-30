#include "renderer.h"

#include <QSGSimpleTextureNode>
#include <QSGImageNode>
#include <QSGRectangleNode>
#include <QGuiApplication>
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

}

Renderer::~Renderer() {
}

QSGNode *Renderer::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *updatePaintNodeData) {


    QSGTransformNode *transform = dynamic_cast<QSGTransformNode *>(oldNode);
    QSGImageNode *node;

    if (!transform) {
        transform = new QSGTransformNode();
        node = window()->createImageNode();
        transform->appendChildNode(node);


        QSGRectangleNode *rect = window()->createRectangleNode();
        rect->setColor(QColor("red"));
        rect->setRect(0, 0, 100, 100);
        transform->appendChildNode(rect);

    } else {
        node = dynamic_cast<QSGImageNode *>(transform->firstChild());
    }


    //load texture
    if (dirty_render) {

        QSGTexture *texture = window()->createTextureFromImage(*m_image, QQuickWindow::TextureHasAlphaChannel);
        node->setTexture(texture);
        node->setOwnsTexture(true);
        dirty_render = false;
    }

    applyTransform(transform);
    applyFit(node);

    node->markDirty(QSGNode::DirtyForceUpdate);
    fps();
    return transform;
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
    if (event->buttons() & Qt::LeftButton) {
        m_press_left_click = event->position();
    }
    if (event->buttons() & Qt::RightButton) {
        m_press_right_click = event->position();
    }
    if (event->buttons() & Qt::MiddleButton) {
        m_press_middle_click = event->position();
    }
    event->position();
}

void Renderer::mouseMoveEvent(QMouseEvent *event) {
    qDebug() << "mouseMoveEvent";
    event->accept();
    Qt::KeyboardModifiers key = QGuiApplication::queryKeyboardModifiers();

    //pad
    if (event->buttons() & Qt::MiddleButton && key==Qt::NoModifier) {
        move(event->position() - m_press_middle_click);
        m_press_middle_click = event->position();
    }

    //zoom
//    if (event->buttons() & Qt::MiddleButton && key==Qt::ControlModifier) {
//        auto delta = (event->position() - m_press_middle_click);
//        zoom(delta.x()-delta.y(), event->position());
//        m_press_middle_click = event->position();
//    }
}

void Renderer::mouseReleaseEvent(QMouseEvent *event) {

    event->accept();
    QPointF tansformed = transform(event->position());
    qDebug() << "mouseReleaseEvent abs:" << event->position() << "transformed:" << tansformed;
}

void Renderer::wheelEvent(QWheelEvent *event) {
    qDebug() << "wheelEvent pixelDelta:" << event->pixelDelta() << "angleDelta:" << event->angleDelta();
    event->accept();
    zoom(event->angleDelta().y(), event->position());
}

void Renderer::applyFit(QSGImageNode *node) {
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
    node->setRect(-width / 2, -height / 2, width, height);
}

void Renderer::applyTransform(QSGTransformNode *transform) {
    QMatrix4x4 transformMat = QMatrix4x4();
    QRectF bound = boundingRect();
    QPointF O = QPointF(bound.width() / 2 + m_padding.x(), bound.height() / 2 + m_padding.y());
    transformMat.translate(O.x(), O.y());
    transformMat.scale(m_scale, m_scale);
    transform->setMatrix(transformMat);
}

void Renderer::move(QPointF delta) {
    qDebug() << "pad delta:" << delta;
    m_padding += delta;
    update();
}

QPointF Renderer::transform(QPointF point){
    QRectF bound = boundingRect();
    QPointF origin = QPointF(bound.width() / 2 + m_padding.x(), bound.height() / 2 + m_padding.y());
    return (point-origin)/m_scale;
}

void Renderer::zoom(float delta, QPointF target) {
    float new_scale =m_scale + (m_scale*delta/300.0);
    if(new_scale>0.5){
        QPointF before = transform(target);
        m_scale =new_scale;
        QPointF after = transform(target);

        //move to target:
        QPointF automove =after- before;
        m_padding += automove*m_scale;
        qDebug() << "Zoom automove:" << automove;
        update();
    }
}

