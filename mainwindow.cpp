#include "mainwindow.hh"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      height_(), weight_()
{
    ui->setupUi(this);

    connect(ui->countButton, &QPushButton::clicked, this, &MainWindow::count_bmi);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::count_bmi()
{
    weight_ = ui->weightLineEdit->text();
    height_ = ui->heightLineEdit->text();
    ui->resultLabel->setText(weight_);

    std::string weight = weight_.toUtf8().constData();
    std::string height = height_.toUtf8().constData();

    float weight1 = stoi(weight);
    float height1 = stoi(height);

    float bmi = weight1/(height1*height1*0.0001);

    ui->resultLabel->setNum(bmi);

    if (bmi < 18.5){
        ui->infoTextBrowser->setText("You are underweight.");
    } else if (bmi >= 18.5 and bmi <= 25) {
        ui->infoTextBrowser->setText("Your weight is normal.");
    } else {
        ui->infoTextBrowser->setText("You are overweight.");
    }

}


/*void MainWindow::on_weightLineEdit_editingFinished()
{
    weight_ = ui->weightLineEdit->text();
}

void MainWindow::on_heightLineEdit_editingFinished()
{
    height_ = ui->heightLineEdit->text();
}*/


