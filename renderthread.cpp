#include "renderthread.h"
#include <QRandomGenerator> //从一个高质量的随机数生成器中获取随机值

RenderThread::RenderThread(QObject *parent):QThread(parent)
{
    m_abort = false;
}

RenderThread::~RenderThread()
{
    mutex.lock();
    m_abort = true;
    mutex.unlock();
    wait(); //阻塞线程，直到满足以下任一条件:
}

//获取绘制图片所需的像素块
void RenderThread::processImage(const QImage &image)
{
    if(image.isNull())
        return;

    m_image = image;
    m_abort = false;
    start(); //通过调用run()开始线程的执行


}

//终止流程
void RenderThread::stopProcess()
{
    mutex.lock();
    m_abort = true; //是否中断
    mutex.unlock();
}

//开始执行进程
void RenderThread::run()
{
    int size = size = qMax(m_image.width()/20, m_image.height()/20);
    for(int s = size; s > 0; --s){
        for(int c = 0; c < 400; ++c){
            int x1 = qMax(0, QRandomGenerator::global()->bounded(m_image.width()) - s/2);
            int x2 = qMin(x1 + s/2 + 1, m_image.width());
            int y1 = qMax(0, QRandomGenerator::global()->bounded(m_image.height()) - s/2);
            int y2 = qMin(y1 + s/2 + 1, m_image.height());
            int n = 0;
            int red = 0;
            int green = 0;
            int blue = 0;
            for (int i = y1; i < y2; ++i) {
                for (int j = x1; j < x2; ++j) {
                    QRgb pixel = m_image.pixel(j, i);
                    red += qRed(pixel);
                    green += qGreen(pixel);
                    blue += qBlue(pixel);
                    n += 1;
                }
            }
            //块
            Block block(QRect(x1, y1, x2 - x1 + 1, y2 - y1 + 1),
                        QColor(red/n, green/n, blue/n));
            emit sendBlock(block);
            if(m_abort)
                return;
            msleep(10); //强制当前线程休眠毫秒。
        }
    }
}

