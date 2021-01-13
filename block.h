#ifndef BLOCK_H
#define BLOCK_H

#include <QColor>
#include <QMetaType> //管理元对象系统中的命名类型
#include <QRect> //矩形

//块
class Block
{
public:
    Block();
    Block(const Block &other);
    ~Block();

    Block(const QRect &rect, const QColor &color);

    QColor color() const;
    QRect rect() const;

private:
    QRect m_rect; //矩形
    QColor m_color;
};

//这个宏就会使QMetaType知道该类的类型。
Q_DECLARE_METATYPE(Block);

#endif // BLOCK_H
