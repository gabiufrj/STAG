#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "algoritmos.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->btnAnalisar->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbrir_Imagem_triggered()
{
    // Pega o endereço da imagem selecionada
    QString imagePath = QFileDialog::getOpenFileName();

    // Cria um objeto imagem e carrega do arquivo selecionado
    QImage imagem;
    imagemAnalise.load( imagePath );

    // Redimensiona para o tamanho das labels!
    // Mantenho a imagemAnalise sem modificar!
    imagem = imagemAnalise.scaled(241, 201);

    // Seta imagemOriginalEscolhida pra eu poder calcular propriedades
    //imagemOriginalEscolhida = true;
    //atualizaDimensoesComImagem();

    // Exibe a imagem na tela
    ui->labelImagem->setPixmap( QPixmap::fromImage(imagem) );
    ui->btnAnalisar->setEnabled(true);
}

void MainWindow::on_actionAjuda_triggered()
{
    QString mensagem = "Seção de Ajuda em construção!\nDesculpe o inconveniente.";
    //ErrorDialog *errordialog = new ErrorDialog(this, mensagem);
    //errordialog->show();
}

void MainWindow::on_btnAnalisar_clicked()
{
    // Preciso ver a opção de algoritmo que foi marcada e executar o
    // algoritmo correspondente na imagem

    // Primeira coisa a fazer é descobrir o tamanho!
    Algoritmos alg = Algoritmos(imagemAnalise);
    int numBits = ui->sbNumBits->value();

    int tam = alg.pegaTamanhoLsbSimples(numBits);

    QString tamanho = QString::number(tam);
    ui->leTamMensagem->setText( tamanho );

    QString mensagem = alg.pegaMensagemLsbSimples(tam, numBits);
    ui->tbMensagemRecuperada->setText(mensagem);
}
