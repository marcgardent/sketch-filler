//
// Created by Garbo on 26/09/2022.
//

#ifndef SKETCHFILLER_RENDERER_H
#define SKETCHFILLER_RENDERER_H

#include <QQuickItem>
#include <QImage>
#include <QtQml>

class Renderer : public QQuickItem {
Q_OBJECT
QML_ELEMENT
public:
    explicit Renderer(QQuickItem *parent = nullptr);
    ~Renderer();
    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *) override;

private:
    QImage m_image;

};


#endif //SKETCHFILLER_RENDERER_H
