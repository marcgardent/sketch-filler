#include "renderer.h"

#include <QSGSimpleTextureNode>
#include <QSGImageNode>
#include <QDebug>

#include <opencv2/core/types.hpp>
#include <opencv2/imgproc.hpp>

#include "Filler.hpp"

Renderer::Renderer(QQuickItem *parent) : QQuickItem(parent) {
    // m_image = new QImage("T:\\data\\sketch-filler2\\chalenges\\case-03.png");
    m_image = new QImage("G:\\Mon Drive\\Ludopant Creative Studio\\Ludopant Galaxy\\order #2 - BD CJS S1E1 Paris\\Order 2.1\\2022-09-16 release Ch1&2\\Page finale 1.tiff");
    setFlag(QQuickItem::ItemHasContents, true);
    get_as_mat();
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
    if(dirty) {
        QSGTexture *texture = window()->createTextureFromImage(*m_image, QQuickWindow::TextureHasAlphaChannel);
        node->setTexture(texture);
        node->setOwnsTexture(true);
        dirty=false;
    }

    // layout
    auto bound = boundingRect();
    auto fit_bound = std::min(bound.width(), bound.height());
    node->setRect(bound.width() / 2 - fit_bound / 2, bound.height() / 2 - fit_bound / 2, fit_bound, fit_bound);
    node->markDirty(QSGNode::DirtyForceUpdate);

    fps();

    return node;
}

cv::Mat Renderer::get_as_mat() {
    return cv::Mat( m_image->height(), m_image->width(),
                  CV_8UC4,
                  m_image->bits(),
                  static_cast<size_t>(m_image->bytesPerLine())
    );
}

void Renderer::fps(){
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

    cv::Mat out= processing::filler::fill(mat);
    out.copyTo(mat);
    requestUpdate();
}

void Renderer::requestUpdate(){


    dirty = true;
    update();
}