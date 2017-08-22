/******************************************************************************
** 文件名:   codetablewnd.h
** 描  述:   码表窗口
** 创建日期:  2017.08.20
** 创建人:    lx
** 修改记录:
******************************************************************************/
#ifndef CODETABLEWND_H
#define CODETABLEWND_H

#include <QWidget>
#include <QPainter>

// 角度范围对应的颜色
struct RangeColor
{
    qreal fStartAngle;      // 起始角度
    qreal fEndAngle;        // 结束角度
    bool bStart;            // 如果等于其实角度颜色是否有效
    bool bEnd;              // 如果等于结束角度颜色是否有效
    QColor color;           // 颜色

    RangeColor() :
        fStartAngle(0),
        fEndAngle(0),
        bStart(true),
        bEnd(true)
    {
    }
};

// 临时QPainter状态管理
class LocalPainterStateManage
{
public:
    LocalPainterStateManage(QPainter *painter) : m_pPainter(painter)
    {
        if (m_pPainter)
            m_pPainter->save();
    }

    ~LocalPainterStateManage()
    {
        if (m_pPainter)
            m_pPainter->restore();
    }

private:
    QPainter *m_pPainter;       // 保持临时painter变量
};

// 码表界面类
class CodeTableWnd : public QWidget
{
    Q_OBJECT
public:
    explicit CodeTableWnd(QWidget *parent = 0);
    ~CodeTableWnd();

public:
    // 设置角度范围所对应的颜色
    void setRange2Color(const QList<RangeColor>& colors);
    // 设置arc线条的颜色
    void setArcLineColor(const QColor& color);
    // 设置文本的颜色
    void setLabelColor(const QColor& color);

    // 显示值的字体
    QFont getValueFont() const;
    void setValueFont(const QFont& font, bool update=true);

    // 值
    qreal getValue() const;
    void setValue(qreal value, bool update=true);

    // 显示文本的字体
    QFont getLabelFont() const;
    void setLabelFont(const QFont& font, bool update=true);

    // 文本
    QString getLabel() const;
    void setLabel(const QString& label, bool update=true);

    // 刻度个数
    void getScaleCount(int& iMainScaleCnt, int& iSubScaleCnt);
    void setScaleCount(int iMainScaleCnt, int iSubScaleCnt);

    // 值范围
    void getValueRange(qreal& min, qreal& max);
    void setValueRange(qreal min, qreal max);

    // 值个数
    int getValueMainScaleCount();
    void setValueMainScaleCount(int iCnt);

protected:
    // 绘图
    virtual void paintEvent(QPaintEvent *event);

private:
    // 画arc图形
    void drawArcGraph(QPainter *painter, const QRect& rect, const QList<RangeColor>& colors);

    // 画arc线条
    void drawArcLine(QPainter *painter, const QRect& rect);

    // 画刻度
    void drawScale(QPainter *painter, const QRect& rect);

    // 画刻度标签
    void drawScaleLabel(QPainter *painter, const QRect& rect);

    // 画标签和值
    void drawText(QPainter *painter, const QRect& rect);

    // 画值标识
    void drawValuePointer(QPainter *painter, const QRect& rect);

private:
    // 初始化角度范围所对应的颜色，也可在外部设置
    void initRangeColors(QList<RangeColor>& colors);
    // 根据角度范围所对应的颜色值获取颜色
    QColor findColor4Angle2Range(const QList<RangeColor>& colors, qreal fAngle);
    // 计算文本所占rect
    QRect calcTextRect4Center(const QString& text, const QFont& font, const QPoint& point);
    // 计算文本所占rect
    QRect calcTextRect4TopRight(const QString& text, const QFont& font, const QPoint& point);
    // 计算文本所占rect
    QRect calcTextRect4TopLeft(const QString& text, const QFont& font, const QPoint& point);
    // 计算文本所占rect
    QRect calcTextRect4BottomRight(const QString& text, const QFont& font, const QPoint& point);
    // 计算文本所占rect
    QRect calcTextRect4BottomLeft(const QString& text, const QFont& font, const QPoint& point);
    // 计算文本所占rect
    QRect calcTextRect4BottomCenter(const QString& text, const QFont& font, const QPoint& point);

signals:

public slots:

private:
    QList<RangeColor> m_list4RangeColors;       // 角度范围所对应的颜色
    QColor m_color4ArcLine;                     // arc线条的颜色
    QColor m_color4Label;                       // 文本的颜色

    QFont m_font4Value;                         // 显示值的字体
    qreal m_fValue;                             // 值

    QFont m_font4Label;                         // 显示的文本的字体
    QString m_strLabel;                         // 显示的文本

    int m_iMainScaleCnt;                        // 主刻度个数
    int m_iSubScaleCnt;                         // 一个主刻度内的子刻度个数

    qreal m_fMinValue;                          // 最小值
    qreal m_fMaxValue;                          // 最大值

    int m_iMainScaleLabelCount;                 // 主刻度标签个数
};

#endif // CODETABLEWND_H
