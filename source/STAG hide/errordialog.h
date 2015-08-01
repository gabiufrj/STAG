#ifndef ERRORDIALOG_H
#define ERRORDIALOG_H

#include <QDialog>

namespace Ui {
class ErrorDialog;
}

class ErrorDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ErrorDialog(QWidget *parent = 0);
    explicit ErrorDialog(QWidget *parent = 0, QString mensagem = "");
    ~ErrorDialog();
    
private slots:
    void on_buttonBox_accepted();

private:
    Ui::ErrorDialog *ui;
};

#endif // ERRORDIALOG_H
