#include "CodeTableWnd.h"
#include <QPaintEvent>
#include <QFontMetrics>
#include <QMatrix>

extern QString G_FONT_BB3812;

// 定义浮点型的“零值”
#define DoubleZero      0.0001

static const int Spacing_Hor = 40;          // 水平边距
static const int Spacing_Ver = 30;          // 垂直边距
static const int Spacing_Bottom = 5;        // 底部间距

CodeTableWnd::CodeTableWnd(QWidget *parent) :
    QWidget(parent),
    m_color4ArcLine(21, 91, 127),
    m_color4Label(21, 91, 127),
    m_fValue(0),
    m_strLabel(""),
    m_iMainScaleCnt(0),
    m_iSubScaleCnt(0),
    m_fMinValue(0),
    m_fMaxValue(0),
    m_iMainScaleLabelCount(0)
{
    // 初始化range colors
    initRangeColors(m_list4RangeColors);

    // 默认值
    m_fValue = 5.01;
    m_strLabel = tr("HengBai");
    m_font4Label = this->font();
    m_font4Label.setPixelSize(15);

    m_font4Label.setBold(true);
    m_font4Value = this->font();
    m_font4Value.setPixelSize(35);
    m_font4Value.setFamily(G_FONT_BB3812);
    m_font4Value.setBold(true);

    m_iMainScaleCnt = 4;
    m_iSubScaleCnt = 5;
    m_fMinValue = -10;
    m_fMaxValue = 10;
    m_iMainScaleLabelCount = 5;
}

CodeTableWnd::~CodeTableWnd()
{

}

void CodeTableWnd::setRange2Color(const QList<RangeColor> &colors)
{
    m_list4RangeColors = colors;
}

void CodeTableWnd::setArcLineColor(const QColor &color)
{
    m_color4ArcLine = color;
}

void CodeTableWnd::setLabelColor(const QColor &color)
{
    m_color4Label = color;
}

QFont CodeTableWnd::getValueFont() const
{
    return m_font4Value;
}

void CodeTableWnd::setValueFont(const QFont &font, bool update)
{
    m_font4Value = font;

    if (update)
        this->update();
}

qreal CodeTableWnd::getValue() const
{
    return m_fValue;
}

void CodeTableWnd::setValue(qreal value, bool update)
{
    m_fValue = value;

    if (update)
        this->update();
}

QFont CodeTableWnd::getLabelFont() const
{
    return m_font4Label;
}

void CodeTableWnd::setLabelFont(const QFont &font, bool update)
{
    m_font4Label = font;

    if (update)
        this->update();
}

QString CodeTableWnd::getLabel() const
{
    return m_strLabel;
}

void CodeTableWnd::setLabel(const QString &label, bool update)
{
    m_strLabel = label;

    if (update)
        this->update();
}

void CodeTableWnd::getScaleCount(int &iMainScaleCnt, int &iSubScaleCnt)
{
    iMainScaleCnt = m_iMainScaleCnt;
    iSubScaleCnt = m_iSubScaleCnt;
}

void CodeTableWnd::setScaleCount(int iMainScaleCnt, int iSubScaleCnt)
{
    m_iMainScaleCnt = iMainScaleCnt;
    m_iSubScaleCnt = iSubScaleCnt;
}

void CodeTableWnd::getValueRange(qreal &min, qreal &max)
{
    min = m_fMinValue;
    max = m_fMaxValue;
}

void CodeTableWnd::setValueRange(qreal min, qreal max)
{
    m_fMinValue = min;
    m_fMaxValue = max;
}

int CodeTableWnd::getValueMainScaleCount()
{
    return m_iMainScaleLabelCount;
}

void CodeTableWnd::setValueMainScaleCount(int iCnt)
{
    m_iMainScaleLabelCount = iCnt;
}

void CodeTableWnd::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 求矩形框
    int iWidth = this->width() - Spacing_Hor - Spacing_Hor;
    int iHeight = (this->height() - Spacing_Ver - Spacing_Bottom) * 2;
    QRect rect(Spacing_Hor, Spacing_Ver, iWidth, iHeight);
    QPoint center = rect.center();
    if (iWidth > iHeight)
    {
        rect = QRect(center, QSize(iHeight, iHeight));
    }
    else
    {
        rect = QRect(center, QSize(iWidth, iWidth));
    }
    rect.moveCenter(center);

    // 画arc图形
    drawArcGraph(&painter, rect, m_list4RangeColors);

    // 画arc线条
    drawArcLine(&painter, rect);

    // 画刻度
    drawScale(&painter, rect);

    // 画刻度标签
    drawScaleLabel(&painter, rect);

    // 画文本
    drawText(&painter, rect);

    // 画指针
    drawValuePointer(&painter, rect);
}

void CodeTableWnd::drawArcGraph(QPainter *painter, const QRect &rect, const QList<RangeColor> &colors)
{
    // 数据异常
    if (0 == painter)
        return;

    // painter临时状态管理
    LocalPainterStateManage localPainterStateManage(painter);
    Q_UNUSED(localPainterStateManage)

    // 设置画刷和画笔
    QBrush brush = painter->brush();
    brush.setStyle(Qt::SolidPattern);
    painter->setBrush(brush);
    QPen pen = painter->pen();
    pen.setCapStyle(Qt::FlatCap);
    int iPenWidth = rect.width() / 15;
    iPenWidth = (iPenWidth>=1) ? iPenWidth : 1;
    pen.setWidth(iPenWidth);

    // 画arc图形
    int iStartAngle = 0;
    int iSpanAngle = 0;
    foreach (const RangeColor& tmpRangeColor, colors)
    {
        pen.setColor(tmpRangeColor.color);
        painter->setPen(pen);

        iStartAngle = (int)(tmpRangeColor.fStartAngle * 16.0);
        iSpanAngle = (int)((tmpRangeColor.fEndAngle - tmpRangeColor.fStartAngle) * 16.0);
        iSpanAngle = qAbs(iSpanAngle);

        painter->drawArc(rect, iStartAngle, iSpanAngle);
    }
}

void CodeTableWnd::drawArcLine(QPainter *painter, const QRect &rect)
{
    // 数据异常
    if (0 == painter)
        return;

    // painter临时状态管理
    LocalPainterStateManage localPainterStateManage(painter);
    Q_UNUSED(localPainterStateManage)

    // 设置画刷和画笔
    QBrush brush = painter->brush();
    brush.setStyle(Qt::SolidPattern);
    painter->setBrush(brush);
    QPen pen = painter->pen();
    pen.setCapStyle(Qt::FlatCap);
    pen.setWidth(1);
    pen.setColor(m_color4ArcLine);
    painter->setPen(pen);

    // 画arc线条
    QRect arcRect = rect;
    int iStartAngle = 0;
    int iSpanAngle = 180 * 16;
    int iSpacing2ArcGraph = arcRect.width() / 10;
    QPoint center = arcRect.center();
    arcRect.setWidth(arcRect.width() - iSpacing2ArcGraph - iSpacing2ArcGraph);
    arcRect.setHeight(arcRect.height() - iSpacing2ArcGraph - iSpacing2ArcGraph);
    arcRect.moveCenter(center);
    painter->drawArc(arcRect, iStartAngle, iSpanAngle);

    QPoint leftCenter(arcRect.left(), center.y());
    QPoint rightCenter(arcRect.right(), center.y());

    painter->drawLine(leftCenter, rightCenter);
}

void CodeTableWnd::drawScale(QPainter *painter, const QRect &rect)
{
    // 数据异常
    if (0 == painter)
        return;

    // painter临时状态管理
    LocalPainterStateManage localPainterStateManage(painter);
    Q_UNUSED(localPainterStateManage)

    // 计算scale的rect
    int iPenWidth = rect.width() / 15;
    iPenWidth = iPenWidth / 2;
    iPenWidth = (iPenWidth>=1) ? iPenWidth : 1;
    QRect rect4Scale = rect;
    rect4Scale.setTop(rect.top()+iPenWidth);
    rect4Scale.setBottom(rect.bottom()-iPenWidth);
    rect4Scale.setLeft(rect.left()+iPenWidth);
    rect4Scale.setRight(rect.right()-iPenWidth);

    // 对刻度数做限制
    int iMainScaleCnt = m_iMainScaleCnt;
    int iSubScaleCnt = m_iSubScaleCnt;
    if (iMainScaleCnt < 2)
        iMainScaleCnt = 2;
    if (iSubScaleCnt < 2)
        iSubScaleCnt = 2;

    // 设置画刷和画笔
    QBrush brush = painter->brush();
    brush.setStyle(Qt::SolidPattern);
    painter->setBrush(brush);
    QPen pen = painter->pen();
    pen.setCapStyle(Qt::FlatCap);
    pen.setWidth(1);

    // 绘制刻度数
    int iSpacing2ArcGraph = rect.width() / 10;
    iSpacing2ArcGraph = iSpacing2ArcGraph - iPenWidth;
    qreal fMainScaleLen = iSpacing2ArcGraph * 3 / 5;
    qreal fSubScaleLen = iSpacing2ArcGraph - fMainScaleLen;
    qreal fMainScale = 180.0 / iMainScaleCnt;
    qreal fSubScale = fMainScale / iSubScaleCnt;
    qreal fCurrentScale = 0.0;
    QPoint center = rect4Scale.center();
    int iScaleRectLen = rect4Scale.width() / 2;
    while (fCurrentScale <= 180.0)
    {
        qreal fTmpMain = fCurrentScale / fMainScale;
        int iTmpMain = (int)fTmpMain;
        fTmpMain = fTmpMain - iTmpMain;
        // 主刻度
        if (qAbs(fTmpMain) < DoubleZero)
        {
            // painter临时状态管理
            LocalPainterStateManage tmpPainterStateManage(painter);
            Q_UNUSED(tmpPainterStateManage)

            pen.setColor(findColor4Angle2Range(m_list4RangeColors, fCurrentScale));
            painter->setPen(pen);

            QPoint p1(iScaleRectLen+1, 0);
            QPoint p2(iScaleRectLen-fMainScaleLen, 0);

            painter->translate(center);
            painter->rotate(-fCurrentScale);
            painter->drawLine(p1, p2);
        }
        // 非主刻度
        else
        {
            // painter临时状态管理
            LocalPainterStateManage tmpPainterStateManage(painter);
            Q_UNUSED(tmpPainterStateManage)

            pen.setColor(findColor4Angle2Range(m_list4RangeColors, fCurrentScale));
            painter->setPen(pen);

            QPoint p1(iScaleRectLen+1, 0);
            QPoint p2(iScaleRectLen-fSubScaleLen, 0);

            painter->translate(center);
            painter->rotate(-fCurrentScale);
            painter->drawLine(p1, p2);
        }

        fCurrentScale += fSubScale;
    }
}

void CodeTableWnd::drawScaleLabel(QPainter *painter, const QRect &rect)
{
    // 数据异常
    if (0 == painter)
        return;

    // painter临时状态管理
    LocalPainterStateManage localPainterStateManage(painter);
    Q_UNUSED(localPainterStateManage)

    // 设置画刷和画笔
    QBrush brush = painter->brush();
    brush.setStyle(Qt::SolidPattern);
    painter->setBrush(brush);
    QPen pen = painter->pen();
    pen.setCapStyle(Qt::FlatCap);
    pen.setWidth(1);
    pen.setColor(m_color4Label);
    painter->setPen(pen);
    painter->setFont(m_font4Label);

    // 计算rect
    int iPenWidth = rect.width() / 15;
    iPenWidth = iPenWidth / 2;
    iPenWidth = (iPenWidth>=1) ? iPenWidth : 1;
    iPenWidth += 2;     // 文字到图形边框的间距
    QRect rect4Label = rect;
    rect4Label.setTop(rect.top()-iPenWidth);
    rect4Label.setBottom(rect.bottom()+iPenWidth);
    rect4Label.setLeft(rect.left()-iPenWidth);
    rect4Label.setRight(rect.right()+iPenWidth);

    // 绘制标签文本
    int iMainScaleLabelCnt = m_iMainScaleLabelCount;
    if (iMainScaleLabelCnt < 2)
        iMainScaleLabelCnt = 2;
    qreal fScaleMainSpace = 180.0 / (iMainScaleLabelCnt - 1);
    qreal fLabelValueSpace = (m_fMaxValue - m_fMinValue) / (iMainScaleLabelCnt - 1);
    qreal fCurrentScale = 0.0;
    QPoint center = rect4Label.center();
    qreal iScaleRectLen = rect4Label.width() / 2;
    qreal fCurrentLabelValue = m_fMaxValue;
    while (fCurrentScale <= 180.0)
    {
        QMatrix matrix;
        matrix.translate(center.x(), center.y());
        matrix.rotate(-fCurrentScale);

        QPoint point(iScaleRectLen, 0);
        point = matrix.map(point);

        QString strLabelText = QString::number(fCurrentLabelValue, 'f', 0);

        QRect tmpLabelRect;
        if (fCurrentScale >= 0 && fCurrentScale <= 88)
            tmpLabelRect = calcTextRect4BottomLeft(strLabelText, m_font4Label, point);
        else if (fCurrentScale > 88 && fCurrentScale <= 92)
            tmpLabelRect = calcTextRect4BottomCenter(strLabelText, m_font4Label, point);
        else
            tmpLabelRect = calcTextRect4BottomRight(strLabelText, m_font4Label, point);

        painter->drawText(tmpLabelRect, strLabelText);

        fCurrentScale += fScaleMainSpace;
        fCurrentLabelValue -= fLabelValueSpace;
    }
}

void CodeTableWnd::drawText(QPainter *painter, const QRect &rect)
{
    // 数据异常
    if (0 == painter)
        return;

    // painter临时状态管理
    LocalPainterStateManage localPainterStateManage(painter);
    Q_UNUSED(localPainterStateManage)

    // 设置画刷和画笔
    QBrush brush = painter->brush();
    brush.setStyle(Qt::SolidPattern);
    painter->setBrush(brush);
    QPen pen = painter->pen();
    pen.setCapStyle(Qt::FlatCap);
    pen.setWidth(2);

    // 计算文本的rect
    QRect textRect = rect;
    textRect.setBottom(textRect.center().y());
    int iSpacing2Top = textRect.width() / 7;
    textRect.setTop(textRect.top()+iSpacing2Top);
    int iPieceHeight = textRect.height() / 7;
    QRect rect4Label = textRect;
    rect4Label.setTop(textRect.top() + 0 * iPieceHeight);
    rect4Label.setBottom(textRect.top() + 3 * iPieceHeight);
    QRect rect4Value = textRect;
    rect4Value.setTop(textRect.top() + 3 * iPieceHeight);
    rect4Value.setBottom(textRect.top() + 7 * iPieceHeight);

    // 画文本
    pen.setColor(m_color4Label);
    painter->setPen(pen);
    painter->setFont(m_font4Label);
    QRect drawRect4Label = calcTextRect4Center(m_strLabel, m_font4Label, rect4Label.center());
    painter->drawText(drawRect4Label, m_strLabel);

    // 画值
    qreal fValueAngle = m_fMinValue - m_fMaxValue;
    if (qAbs(fValueAngle) < DoubleZero)
        fValueAngle = -1;
    fValueAngle = (m_fValue - m_fMaxValue) * 180.0 / fValueAngle;
    pen.setColor(findColor4Angle2Range(m_list4RangeColors, fValueAngle));
    painter->setPen(pen);
    painter->setFont(m_font4Value);
    QRect defaultRect4Value = calcTextRect4Center(QString("0.00"), m_font4Value, rect4Value.center());
    QString strValue = QString::number(m_fValue, 'f', 2);
    QRect drawRect4Value = calcTextRect4BottomRight(strValue, m_font4Value, defaultRect4Value.bottomRight());
    painter->drawText(drawRect4Value, strValue);
}

void CodeTableWnd::drawValuePointer(QPainter *painter, const QRect &rect)
{
    // 数据异常
    if (0 == painter)
        return;

    // painter临时状态管理
    LocalPainterStateManage localPainterStateManage(painter);
    Q_UNUSED(localPainterStateManage)

    // 设置画刷和画笔
    QBrush brush = painter->brush();
    brush.setStyle(Qt::SolidPattern);
    QPen pen = painter->pen();
    pen.setCapStyle(Qt::FlatCap);
    pen.setWidth(3);

    // 计算颜色值
    qreal fValueAngle = m_fMinValue - m_fMaxValue;
    if (qAbs(fValueAngle) < DoubleZero)
        fValueAngle = -1;
    fValueAngle = (m_fValue - m_fMaxValue) * 180.0 / fValueAngle;
    QColor color = findColor4Angle2Range(m_list4RangeColors, fValueAngle);
    brush.setColor(color);
    painter->setBrush(brush);
    pen.setColor(color);
    painter->setPen(pen);

    // 计算rect
    int iPenWidth = rect.width() / 15;
    iPenWidth = iPenWidth / 2;
    iPenWidth = (iPenWidth>=1) ? iPenWidth : 1;

    QRect rect4Scale = rect;
    rect4Scale.setTop(rect.top()+iPenWidth);
    rect4Scale.setBottom(rect.bottom()-iPenWidth);
    rect4Scale.setLeft(rect.left()+iPenWidth);
    rect4Scale.setRight(rect.right()-iPenWidth);

    QRect arcRect = rect;
    int iSpacing2ArcGraph = rect.width() / 10;
    arcRect.setWidth(rect.width() - iSpacing2ArcGraph - iSpacing2ArcGraph);
    arcRect.setHeight(rect.height() - iSpacing2ArcGraph - iSpacing2ArcGraph);
    arcRect.moveCenter(rect.center());
    QPoint center = arcRect.center();

    QMatrix matrix;
    matrix.translate(center.x(), center.y());
    matrix.rotate(-fValueAngle);

    QPoint point1(rect4Scale.width()/2, 0);
    QPoint point2(arcRect.width()/2, 0);
    point1 = matrix.map(point1);
    point2 = matrix.map(point2);

    painter->drawLine(point1, point2);
    painter->drawEllipse(point2, 5, 5);
}

void CodeTableWnd::initRangeColors(QList<RangeColor> &colors)
{
    QColor color1(111, 12, 7);
    QColor color2(122, 101, 38);
    QColor color3(64, 94, 30);

    RangeColor rangeColor;
    // range1
    rangeColor.fStartAngle = 0;
    rangeColor.fEndAngle = 22.5;
    rangeColor.bStart = true;
    rangeColor.bEnd = false;
    rangeColor.color = color1;
    colors.push_back(rangeColor);
    // range2
    rangeColor.fStartAngle = 22.5;
    rangeColor.fEndAngle = 45;
    rangeColor.bStart = true;
    rangeColor.bEnd = false;
    rangeColor.color = color2;
    colors.push_back(rangeColor);
    // range3
    rangeColor.fStartAngle = 45;
    rangeColor.fEndAngle = 135;
    rangeColor.bStart = true;
    rangeColor.bEnd = true;
    rangeColor.color = color3;
    colors.push_back(rangeColor);
    // range4
    rangeColor.fStartAngle = 135;
    rangeColor.fEndAngle = 157.5;
    rangeColor.bStart = false;
    rangeColor.bEnd = true;
    rangeColor.color = color2;
    colors.push_back(rangeColor);
    // range5
    rangeColor.fStartAngle = 157.5;
    rangeColor.fEndAngle = 180;
    rangeColor.bStart = false;
    rangeColor.bEnd = true;
    rangeColor.color = color1;
    colors.push_back(rangeColor);
}

QColor CodeTableWnd::findColor4Angle2Range(const QList<RangeColor> &colors, qreal fAngle)
{
    foreach (const RangeColor& tmpRangeColor, colors)
    {
        // 等于起始角度
        if (tmpRangeColor.bStart && qAbs(fAngle-tmpRangeColor.fStartAngle) < DoubleZero)
            return tmpRangeColor.color;

        // 介于起始角与结束角之间
        if ((fAngle > tmpRangeColor.fStartAngle && fAngle < tmpRangeColor.fEndAngle) ||
                (fAngle > tmpRangeColor.fEndAngle && fAngle < tmpRangeColor.fStartAngle))
            return tmpRangeColor.color;

        // 等于结束角
        if (tmpRangeColor.bEnd && qAbs(fAngle-tmpRangeColor.fEndAngle) < DoubleZero)
            return tmpRangeColor.color;
    }

    return QColor(0, 0, 0);
}

QRect CodeTableWnd::calcTextRect4Center(const QString &text, const QFont &font, const QPoint &point)
{
    QFontMetrics metrics(font);

    QRect rect(0, 0, metrics.width(text), metrics.height());
    rect.moveCenter(point);

    return rect;
}

QRect CodeTableWnd::calcTextRect4TopRight(const QString &text, const QFont &font, const QPoint &point)
{
    QFontMetrics metrics(font);

    QRect rect(0, 0, metrics.width(text), metrics.height());
    rect.moveTopRight(point);

    return rect;
}

QRect CodeTableWnd::calcTextRect4TopLeft(const QString &text, const QFont &font, const QPoint &point)
{
    QFontMetrics metrics(font);

    QRect rect(0, 0, metrics.width(text), metrics.height());
    rect.moveTopLeft(point);

    return rect;
}

QRect CodeTableWnd::calcTextRect4BottomRight(const QString &text, const QFont &font, const QPoint &point)
{
    QFontMetrics metrics(font);

    QRect rect(0, 0, metrics.width(text), metrics.height());
    rect.moveBottomRight(point);

    return rect;
}

QRect CodeTableWnd::calcTextRect4BottomLeft(const QString &text, const QFont &font, const QPoint &point)
{
    QFontMetrics metrics(font);

    QRect rect(0, 0, metrics.width(text), metrics.height());
    rect.moveBottomLeft(point);

    return rect;
}

QRect CodeTableWnd::calcTextRect4BottomCenter(const QString &text, const QFont &font, const QPoint &point)
{
    QFontMetrics metrics(font);

    QRect rect(0, 0, metrics.width(text), metrics.height());
    rect.moveCenter(point);
    rect.moveBottom(point.y());

    return rect;
}

