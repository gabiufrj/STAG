#include "algoritmos.h"

Algoritmos::Algoritmos()
{
}

Algoritmos::Algoritmos(QImage img)
{
    imagem = img;
}

int Algoritmos::descobreBitDaPosicao(int num, int pos)
{
    int mascara = 1 << pos;
    int retorno = (num & mascara);
    retorno = retorno >> pos;
    return retorno;
}

int Algoritmos::pegaBits(int num, int posIni, int numBits)
{
    if (posIni < 0)
    {
        return 0;
    }

    int resultado = 0;

    for(int i = 0; i < numBits; i++)
    {
        if ((posIni - i) < 0 )
        {
            break;
        }

        int temp = descobreBitDaPosicao(num, posIni - i);
        temp = temp << (numBits - i - 1);
        resultado += temp;
    }

    return resultado;
}

int Algoritmos::pegaTamanhoLsbSimples(int numBits, int pixelIniLargura, int pixelIniAltura)
{
    // Descubro quantos pixels preciso ler pra ter o tamanho
    int tamInt = (sizeof(int) * 8);
    int quantPixels = tamInt/(numBits * 3);

    if (tamInt % (numBits * 3) != 0)
    {
        quantPixels++;
    }

    int resultado = 0;

    // Laço pra pegar os 32 bits que formam um inteiro
    int pixelLargura = pixelIniLargura;
    int pixelAltura = pixelIniAltura;
    int maxLargura = imagem.size().width();
    int posLeitura = 31;
    for (int i = 0; i < quantPixels; i++)
    {
        QRgb orgPixel = imagem.pixel(pixelLargura, pixelAltura);
        int vermelho = qRed(orgPixel);
        int azul = qBlue(orgPixel);
        int verde = qGreen(orgPixel);

        // Pega informação do Vermelho
        int flippedBits = pegaBits(vermelho, (numBits - 1), numBits);
        resultado += (flippedBits << posLeitura);
        posLeitura -= numBits;

        if (posLeitura >= 0)
        {
            // Pega informação do Verde
            flippedBits = pegaBits(verde, (numBits - 1), numBits);
            resultado += (flippedBits << posLeitura);
            posLeitura -= numBits;
        }

        if (posLeitura >= 0)
        {
            // Pega informação do Azul
            flippedBits = pegaBits(azul, (numBits - 1), numBits);
            resultado += (flippedBits << posLeitura);
            posLeitura -= numBits;
        }

        pixelLargura++;
        if (pixelLargura >= maxLargura)
        {
            pixelLargura = 0;
            pixelAltura++;
        }
    }

    return resultado;
}

QString Algoritmos::pegaMensagemLsbSimples(int tamanho, int numBits, int pixelIniLargura, int pixelIniAltura)
{
    QString mensagem("");

    // Descubro quantos pixels preciso ler pra ter um inteiro
    int tamInt = (sizeof(int) * 8);
    int quantPixels = tamInt/(numBits * 3);

    if (tamInt % (numBits * 3) != 0)
    {
        quantPixels++;
    }

    int maxLargura = imagem.size().width();
    int pixelLargura = pixelIniLargura + quantPixels;
    int pixelAltura = pixelIniAltura;

    if (pixelLargura >= maxLargura)
    {
        pixelLargura -= maxLargura;
        pixelAltura++;
    }


    for (int j = 0; j < tamanho; j++)
    {
        int resultado = 0;

        // Laço pra pegar os 32 bits que formam um inteiro
        int posLeitura = 31;
        for (int i = 0; i < quantPixels; i++)
        {
            QRgb orgPixel = imagem.pixel(pixelLargura, pixelAltura);
            int vermelho = qRed(orgPixel);
            int azul = qBlue(orgPixel);
            int verde = qGreen(orgPixel);

            // Pega informação do Vermelho
            int flippedBits = pegaBits(vermelho, (numBits - 1), numBits);
            resultado += (flippedBits << posLeitura);
            posLeitura -= numBits;

            if (posLeitura >= 0)
            {
                // Pega informação do Verde
                flippedBits = pegaBits(verde, (numBits - 1), numBits);
                resultado += (flippedBits << posLeitura);
                posLeitura -= numBits;
            }

            if (posLeitura >= 0)
            {
                // Pega informação do Azul
                flippedBits = pegaBits(azul, (numBits - 1), numBits);
                resultado += (flippedBits << posLeitura);
                posLeitura -= numBits;
            }

            pixelLargura++;
            if (pixelLargura >= maxLargura)
            {
                pixelLargura = 0;
                pixelAltura++;
            }
        }

        QChar temp(resultado);
        mensagem.append(temp);
    }

    return mensagem;
}
