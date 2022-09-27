#include "renderer.h"
#include <QSGSimpleTextureNode>
#include <QSGImageNode>
#include <QDebug>
#include "asm_opencv.h"
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc.hpp>


cv::RNG rng(12365);

inline cv::Scalar rng_argb() {
    return  cv::Scalar(0,rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
}

Renderer::Renderer(QQuickItem *parent) : QQuickItem(parent) {
    m_image = new QImage("T:\\data\\sketch-filler2\\chalenges\\case-03.png");
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

    { //Set Texture
        cv::Mat mat = get_as_mat();
        cv::line(mat, cv::Point(0,0), cv::Point(256,256), rng_argb(), 10);
        mat.release();

        QSGTexture *texture = window()->createTextureFromImage(*m_image, QQuickWindow::TextureHasAlphaChannel);
        node->setTexture(texture);
        node->setOwnsTexture(true);
    }

    auto bound = boundingRect();
    auto fit_bound = std::min(bound.width(), bound.height());
    node->setRect(bound.width() / 2 - fit_bound / 2, bound.height() / 2 - fit_bound / 2, fit_bound, fit_bound);
    node->markDirty(QSGNode::DirtyForceUpdate);

    {
        auto currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
        if (currentTime > lastTime) {
            auto fps = (currentTime - lastTime);
            lastTime = currentTime;
            qDebug() << "FPS: " << fps;
        }
    }
    return node;
}

cv::Mat Renderer::get_as_mat() {
    return cv::Mat( m_image->height(), m_image->width(),
                  CV_8UC4,
                  m_image->bits(),
                  static_cast<size_t>(m_image->bytesPerLine())
    );
}
