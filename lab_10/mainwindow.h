#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QString>
#include <QMouseEvent>
#include <QEvent>

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
    void on_btn_UploadPhoto_clicked();
    void on_btn_Register_clicked();
    void clearAllFields();

private:
    Ui::MainWindow *ui;
    QString photoPath;

    bool validateData();
    QString encryptPassword(QString password);
    bool isNicknameTaken(QString nickname);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // MAINWINDOW_H