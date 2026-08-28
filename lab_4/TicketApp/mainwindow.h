#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>

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
    void on_resetButton_clicked();
    void on_saveButton_clicked();

private:
    Ui::MainWindow *ui;

    // Группа для радио-кнопок (Да/Нет)
    QButtonGroup *baggageGroup;

    // Методы валидации
    bool validateFromTo(const QString& text, QString& errorMsg);
    bool validateDateTime(const QString& text, QString& errorMsg);
    bool validateSeat(const QString& text, QString& errorMsg);
};

#endif // MAINWINDOW_H