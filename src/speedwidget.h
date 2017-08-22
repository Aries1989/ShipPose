#ifndef SPEEDWIDGET_H
#define SPEEDWIDGET_H

#include <QWidget>

namespace Ui {
class SpeedWidget;
}

class SpeedWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SpeedWidget(QWidget *parent = 0);
    ~SpeedWidget();

    void setValueHor(qreal value);
    void setValueVer(qreal value);
    void setSpeed(qreal speed);

protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    Ui::SpeedWidget *ui;
};

#endif // SPEEDWIDGET_H
