#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QBrush>
#include <QColor>
#include <QVector>
#include <algorithm>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Pizza {
public:
    QString name;
    QString desc;
    double hunger, price, weight;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool isValidPizza(const QJsonObject &obj);
    Pizza parsePizza(const QJsonObject &obj);

private slots:
    void on_btn_load_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H