#include "MainWidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    this->resize(800, 400);

    ui->widget->setValueHor(ui->slider_hor->value());
    ui->widget->setValueVer(ui->slider_ver->value());
    ui->widget->setSpeed(ui->slider_speed->value());

    ui->widget->setStyleSheet(QString("SpeedWidget#widget{ \
                                      border-image: url(:/background.png); \
                                      }"));
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::on_slider_hor_valueChanged(int value)
{
    ui->widget->setValueHor(value);
}

void MainWidget::on_slider_ver_valueChanged(int value)
{
    ui->widget->setValueVer(value);
}

void MainWidget::on_slider_speed_valueChanged(int value)
{
    ui->widget->setSpeed(value);
}
