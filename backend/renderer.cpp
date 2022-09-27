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
        node->setRect(boundingRect());
        node->markDirty(QSGNode::DirtyForceUpdate);
        node->setTexture(texture);


        return node;
}
