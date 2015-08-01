#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "errordialog.h"
#include "algoritmos.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    imagemOriginalEscolhida = false;
    existeImagemModificada = false;

    tamHeaderBits = 32;
    quantBits = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnEsconder_clicked()
{
    // Aqui vou fazer a a��o de esconder a mensagem dada na imagem.
    // O que preciso fazer:
    // 1 - Pegar o tamanho da mensagem, em bits e verificar se cabe na imagem
    // 2 - Lembrar que preciso guardar os 4 primeiros bytes para header (inteiro correspondente ao tamanho)
    // 3 - Finalmente alterar os bits desejados, usando o LSB mais simples poss�vel, ou seja, come�ando do canto superior esquerdo
    // e avan�ando linha a linha, da esquerda para a direita.

    // Testa se a mensagem a ser inserida n�o est� vazia.
    if ( ui->MensagemInput->toPlainText().size() == 0 )
    {
        ErrorDialog *errordialog = new ErrorDialog(this, "A mensagem n�o pode ser vazia!");
        errordialog->show();

        return;
    }

    /**
      Passos 1 e 2
    **/
    // Contabiliza os bytes totais da mensagem
    int countBytes = 0;

    // Preciso considerar o TAMANHO do HEADER em bytes
    countBytes += (tamHeaderBits/8);

    // Cada char da mensagem possui 1 byte
    countBytes += ui->MensagemInput->toPlainText().size();

    // Transformando em bits
    int countBits = countBytes * 8;

    // Para ver se cabe na imagem, preciso descobrir quantos pixels ela tem e quantos bits
    // o usu�rio escolheu utilizar para esconder sua mensagem

    // Se a quantidade dispon�vel for menor que o necess�rio, mostrar janela de erro
    if (quantBits < countBits)
    {
        QString mensagem = "A mensagem � maior do que o limite possivel de esconder.";
        mensagem += "\nTente aumentar o n�mero de bits, diminuir a mensagem ou trocar a imagem original";
        ErrorDialog *errordialog = new ErrorDialog(this, mensagem);
        errordialog->show();

        return;
    }

    /**
      Passo 3 - Esconder a mensagem
    **/
    Algoritmos alg = Algoritmos(imagemOriginal);
    imagemModificada = alg.aplicaLSB( ui->sbNumBits->value(),
                                      ui->MensagemInput->toPlainText() );

    QImage imagem = imagemModificada.scaled(241, 201);
    ui->labelImgMod->setPixmap( QPixmap::fromImage(imagem) );
    existeImagemModificada = true;
}

void MainWindow::on_actionAbrir_Imagem_triggered()
{
    // Pega o endere�o da imagem selecionada
    QString imagePath = QFileDialog::getOpenFileName();

    // Cria um objeto imagem e carrega do arquivo selecionado
    QImage imagem;
    imagemOriginal.load( imagePath );

    // Redimensiona para o tamanho das labels!
    // Mantenho a imagemOriginal sem modificar!
    imagem = imagemOriginal.scaled(241, 201);

    // Seta imagemOriginalEscolhida pra eu poder calcular propriedades
    imagemOriginalEscolhida = true;
    atualizaDimensoesComImagem();

    // Exibe a imagem na tela
    ui->labelImgOriginal->setPixmap( QPixmap::fromImage(imagem) );
}

void MainWindow::atualizaDimensoesComImagem()
{
    // Preenche a label de dimens�es da imagem
    QSize tam = imagemOriginal.size();
    QString dimensoes = QString::number(tam.width()) + " x "
            + QString::number(tam.height());
    ui->labelDimensoes->setText( dimensoes );

    ui->sbNumBits->setEnabled(true);
    ui->btnEsconder->setEnabled(true);
}

void MainWindow::on_actionSalvar_Imagem_triggered()
{
    // Salvar a imagem modificada, somente se ela j� estiver definida!
    if ( existeImagemModificada )
    {
        // Abrindo o dialog pra escolha do arquivo
        QString path = QFileDialog::getSaveFileName();

        // Salvando a imagem
        imagemModificada.save(path);

        // Exibindo confirma��o
        QString mensagem = "Arquivo " + path + " salvo com sucesso!";
        ErrorDialog *errordialog = new ErrorDialog(this, mensagem);
        errordialog->show();

        return;
    }

    // Se a imagem modificada n�o estiver definida ainda
    QString mensagem = "Imagem modificada ainda n�o foi definida!\nEsconda algo primeiro!";
    ErrorDialog *errordialog = new ErrorDialog(this, mensagem);
    errordialog->show();
}

void MainWindow::on_actionAjuda_triggered()
{
    QString mensagem = "Se��o de Ajuda em constru��o!\nDesculpe o inconveniente.";
    ErrorDialog *errordialog = new ErrorDialog(this, mensagem);
    errordialog->show();
}

void MainWindow::on_sbNumBits_valueChanged(int arg1)
{
    if (arg1 != 0)
    {
        // Preciso recalcular os limites de informa��o escondidas com o novo valor pro LSB
        QSize tam = imagemOriginal.size();

        // bits dispon�veis = altura * largura * 3 (canais de cores R, G e B)
        int countBits = tam.height() * tam.width() * 3 * arg1;

        // bytes dispon�veis = (bits imagem - bits do header) / 8;
        int countBytes = (countBits - tamHeaderBits)/8;

        quantBits = countBits;

        ui->labelBits->setText( QString::number(countBits - tamHeaderBits) + " bits" );
        ui->labelCaracteres->setText( QString::number(countBytes) + " caracteres" );
    }
    else
    {
        quantBits = 0;
        ui->labelBits->setText( "0 bits" );
        ui->labelCaracteres->setText( "0 caracteres" );
    }
}
