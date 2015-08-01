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
    // Aqui vou fazer a ação de esconder a mensagem dada na imagem.
    // O que preciso fazer:
    // 1 - Pegar o tamanho da mensagem, em bits e verificar se cabe na imagem
    // 2 - Lembrar que preciso guardar os 4 primeiros bytes para header (inteiro correspondente ao tamanho)
    // 3 - Finalmente alterar os bits desejados, usando o LSB mais simples possível, ou seja, começando do canto superior esquerdo
    // e avançando linha a linha, da esquerda para a direita.

    // Testa se a mensagem a ser inserida não está vazia.
    if ( ui->MensagemInput->toPlainText().size() == 0 )
    {
        ErrorDialog *errordialog = new ErrorDialog(this, "A mensagem não pode ser vazia!");
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
    // o usuário escolheu utilizar para esconder sua mensagem

    // Se a quantidade disponível for menor que o necessário, mostrar janela de erro
    if (quantBits < countBits)
    {
        QString mensagem = "A mensagem é maior do que o limite possivel de esconder.";
        mensagem += "\nTente aumentar o número de bits, diminuir a mensagem ou trocar a imagem original";
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
    // Pega o endereço da imagem selecionada
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
    // Preenche a label de dimensões da imagem
    QSize tam = imagemOriginal.size();
    QString dimensoes = QString::number(tam.width()) + " x "
            + QString::number(tam.height());
    ui->labelDimensoes->setText( dimensoes );

    ui->sbNumBits->setEnabled(true);
    ui->btnEsconder->setEnabled(true);
}

void MainWindow::on_actionSalvar_Imagem_triggered()
{
    // Salvar a imagem modificada, somente se ela já estiver definida!
    if ( existeImagemModificada )
    {
        // Abrindo o dialog pra escolha do arquivo
        QString path = QFileDialog::getSaveFileName();

        // Salvando a imagem
        imagemModificada.save(path);

        // Exibindo confirmação
        QString mensagem = "Arquivo " + path + " salvo com sucesso!";
        ErrorDialog *errordialog = new ErrorDialog(this, mensagem);
        errordialog->show();

        return;
    }

    // Se a imagem modificada não estiver definida ainda
    QString mensagem = "Imagem modificada ainda não foi definida!\nEsconda algo primeiro!";
    ErrorDialog *errordialog = new ErrorDialog(this, mensagem);
    errordialog->show();
}

void MainWindow::on_actionAjuda_triggered()
{
    QString mensagem = "Seção de Ajuda em construção!\nDesculpe o inconveniente.";
    ErrorDialog *errordialog = new ErrorDialog(this, mensagem);
    errordialog->show();
}

void MainWindow::on_sbNumBits_valueChanged(int arg1)
{
    if (arg1 != 0)
    {
        // Preciso recalcular os limites de informação escondidas com o novo valor pro LSB
        QSize tam = imagemOriginal.size();

        // bits disponíveis = altura * largura * 3 (canais de cores R, G e B)
        int countBits = tam.height() * tam.width() * 3 * arg1;

        // bytes disponíveis = (bits imagem - bits do header) / 8;
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
