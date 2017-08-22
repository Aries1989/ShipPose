#include "SpeedWidget.h"
#include "ui_speedwidget.h"

extern QString G_FONT_BB3812;

SpeedWidget::SpeedWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SpeedWidget)
{
    ui->setupUi(this);

    QFont font = ui->label_valueVer->font();
    font.setFamily(G_FONT_BB3812);
    ui->label_valueVer->setFont(font);

    ui->wnd_tableHor->setLabel(tr("hengyao"));
    ui->wnd_tableVer->setLabel(tr("zongyao"));
}

SpeedWidget::~SpeedWidget()
{
    delete ui;
}

void SpeedWidget::setValueHor(qreal value)
{
    ui->wnd_tableHor->setValue(value);
}

void SpeedWidget::setValueVer(qreal value)
{
    ui->wnd_tableVer->setValue(value);
}

void SpeedWidget::setSpeed(qreal speed)
{
    QString strSpeed = QString::number(speed, 'f', 1);
    ui->label_valueVer->setText(strSpeed);
}

void SpeedWidget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);
}
