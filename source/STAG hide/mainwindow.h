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
    
private slots:
    void on_btnEsconder_clicked();

    void on_actionAbrir_Imagem_triggered();

    void on_actionSalvar_Imagem_triggered();

    void on_actionAjuda_triggered();

    void on_sbNumBits_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
    QImage imagemOriginal, imagemModificada;
    bool imagemOriginalEscolhida, existeImagemModificada;

    int quantBits;
    int tamHeaderBits;

    void atualizaDimensoesComImagem();
};

#endif // MAINWINDOW_H
