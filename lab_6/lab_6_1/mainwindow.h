#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool validateData(const QString &name, const QString &desc,
                      const QString &hunger, const QString &price, const QString &weight);

private slots:
    void on_btn_loadTxt_clicked();
    void on_btn_saveJson_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H