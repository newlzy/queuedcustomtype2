#include "window.h"
#include <QtWidgets>
#pragma execution_character_set("utf-8")

Window::Window(QWidget *parent) : QWidget(parent),thread(new RenderThread(this))
{
    label = new QLabel(this);
    label->setAlignment(Qt::AlignCenter); //居中对齐

    loadButton = new QPushButton(tr("&加载图像"),this);
    resetButton = new QPushButton(tr("&停止"), this);
    resetButton->setEnabled(false); //小部件不启用

    //加载图像
    connect(loadButton,&QPushButton::clicked,this,QOverload<>::of(&Window::loadImages));
    //停止加载
    connect(resetButton,&QPushButton::clicked,thread,&RenderThread::stopProcess);
    //finished这个信号是在关联线程完成执行之前发出的
    connect(thread,&RenderThread::finished,this,&Window::resetUi);
    //绘制像素块
    connect(thread,&RenderThread::sendBlock,this,&Window::addBlock);

    //水平排列小部件
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    //添加一个最小尺寸为零的可伸缩空间(QSpacerItem)，并将伸缩因子拉伸到这个方框布局的末端
    buttonLayout->addStretch();
    buttonLayout->addWidget(loadButton);
    buttonLayout->addWidget(resetButton);
    buttonLayout->addStretch();

    //垂直排列小部件。
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(label);
    layout->addLayout(buttonLayout);

    setWindowTitle(tr("线程绘图"));
}

//加载图像
void Window::loadImage(const QImage &image)
{
    QImage useImage;
    //保存应用程序的主屏幕。以像素为单位保存屏幕上可用的几何图形
    QRect space = QGuiApplication::primaryScreen()->availableGeometry();

    if(image.width() > 0.75*space.width() || image.height() > 0.75*space.height()){
        //返回缩放到给定宽度和高度的矩形的图像副本
        useImage = image.scaled(0.75*space.width(),0.75*space.height(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    }else
        useImage = image;

    pixmap = QPixmap(useImage.width(),useImage.height());
    pixmap.fill(qRgb(255,255,255)); //用给定的颜色填充像素图
    label->setPixmap(pixmap);
    loadButton->setEnabled(false);
    resetButton->setEnabled(true);
    thread->processImage(useImage); //线程处理图像
}

//把图片像素块绘制出来
void Window::addBlock(const Block &block)
{
    QColor color = block.color(); //获取块类的颜色
    color.setAlpha(64); //设置透明度
    QPainter painter;
    painter.begin(&pixmap); //开始绘制
    painter.fillRect(block.rect(),color); //矩形为块类的矩形和颜色
    painter.end();
    label->setPixmap(pixmap);
}

//打开图像
void Window::loadImages()
{
    QStringList formats;
    //QByteArray字节数组。QImageReader与格式无关的接口，读取图像。
    const QList<QByteArray> supportedFormats = QImageReader::supportedImageFormats();

    for(const QByteArray &format : supportedFormats){
        //toLower返回字节数组的小写副本,bytearray被解释为Latin-1编码的字符串。
        if(format.toLower() == format){
            //QLatin1String类为US-ASCII/Latin-1编码的字符串文本提供了一个精简的包装器。fromLatin1返回一个用Latin-1字符串str初始化的QString。
            formats.append(QLatin1String("*.") + QString::fromLatin1(format));
        }
}
        //QFileDialog用户选择文件的对话框。join将所有字节数组连接到单个字节数组中，每个元素由给定的分隔符分隔
        QString newPath = QFileDialog::getOpenFileName(this,tr("打开图像"),path,tr("图像文件 (%1)").arg(formats.join(' ')));
        if(newPath.isEmpty())
            return;

        QImage image(newPath);
        if(!image.isNull()){
            loadImage(image);
            path = newPath;
        }



}

void Window::resetUi()
{
    loadButton->setEnabled(true); //加载图片
    resetButton->setEnabled(false); //停止
}
