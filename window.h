#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include "renderthread.h"

QT_BEGIN_HEADER
class QLabel;
class QPushButton;
QT_END_NAMESPACE

class Window : public QWidget
{
    Q_OBJECT
public:
    Window(QWidget *parent = nullptr);
    void loadImage(const QImage &image); //加载图像
public slots:
    void addBlock(const Block &block); //添加块到显示

private slots:
    void loadImages(); //打开图像文件
    void resetUi();

private:
    QLabel *label; //这标签里面放像素图
    QPixmap pixmap;
    QPushButton *loadButton;
    QPushButton *resetButton;
    QString path; //图片路径
    RenderThread *thread; //自定义线程类


};

#endif // WINDOW_H
