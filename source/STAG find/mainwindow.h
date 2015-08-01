#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QImage imagemAnalise;
    
private slots:
    void on_actionAbrir_Imagem_triggered();

    void on_actionAjuda_triggered();

    void on_btnAnalisar_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
