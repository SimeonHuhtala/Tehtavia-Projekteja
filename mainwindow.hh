#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void count_bmi();

    //void on_weightLineEdit_editingFinished();

    //void on_heightLineEdit_editingFinished();

    //void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;

    QString height_;
    QString weight_;
};
#endif // MAINWINDOW_HH
