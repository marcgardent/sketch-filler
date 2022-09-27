#include "renderer.h"
#include <QSGSimpleTextureNode>

Renderer::Renderer(QQuickItem *parent) : QQuickItem(parent)
{
    m_image = QImage("T:\\data\\sketch-filler2\\chalenges\\chalenge-01.png");
    setFlag(QQuickItem::ItemHasContents, true);
}

Renderer::~Renderer() {

}

QSGNode *Renderer::updatePaintNode(QSGNode * oldNode, QQuickItem::UpdatePaintNodeData *updatePaintNodeData) {

        auto node = dynamic_cast<QSGSimpleTextureNode *>(oldNode);
        if (!node) {
            node = new QSGSimpleTextureNode();
        }

        QSGTexture *texture = window()->createTextureFromImage(m_image, QQuickWindow::TextureIsOpaque);
        node->setOwnsTexture(true);
        //node->setRect(boundingRect());
        auto bound = boundingRect();
        auto fit_bound = std::min(bound.width(),bound.height());

        node->setRect(bound.width()/2 - fit_bound/2, bound.height()/2 - fit_bound/2, fit_bound, fit_bound);
        node->markDirty(QSGNode::DirtyForceUpdate);
        node->setTexture(texture);
        return node;
}
