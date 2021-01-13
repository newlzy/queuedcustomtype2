#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QThread>
#include <QMutex> //互斥锁
#include <QImage>
#include "block.h"

class RenderThread : public QThread
{
    Q_OBJECT
public:
    RenderThread(QObject *parent = nullptr);
    ~RenderThread();

    void processImage(const QImage &image); //处理图片

signals:
    void sendBlock(const Block &block); //决定"块(Block)"类的样式

public slots:
    void stopProcess(); //终止流程

protected:
    void run();

private:
    bool m_abort; //是否中断
    QImage m_image;
    QMutex mutex;

};

#endif // RENDERTHREAD_H
