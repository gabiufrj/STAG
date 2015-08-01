#include "algoritmos.h"

Algoritmos::Algoritmos()
{
}

Algoritmos::Algoritmos(QImage imagem)
{
    imagemOriginal = imagem;
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

int Algoritmos::defineMascara(int numBits)
{
    // Máscara de Bits == 0xFF - (2^numBits - 1)
    int mascara;

    if (numBits == 1)
    {
        mascara = 0xFE;
    }
    else if (numBits == 2)
    {
        mascara = 0xFC;
    }
    else
    {
        mascara = 0xF8;
    }

    return mascara;
}

QImage Algoritmos::aplicaLSB(int numBits, QString mensagem)
{
    QImage retorno = imagemOriginal;

    int tamMensagem = mensagem.size();

    // Objetivo é esconder bits em cada canal de cor de cada pixel.
    // Ou seja, esconder "numBits" bits em R, G e B de cada pixel, alterando
    // os menos significativos.
    // Maneira mais eficiente de fazer isso é usando uma "Máscara de Bits" e
    // fazendo operações binárias, como ANDs e ORs, bit a bit.

    // BIT MASK
    // Primeiro fazer um AND entre a cor e o hexa 0xFF - (2^numbits - 1)

    // EXEMPLO: numbits = 3, hexa = 0xF8
    // 0xF8           = 1 1 1 1 1 0 0 0
    // R = 128 = 0xFF = 1 0 0 0 0 0 0 0
    // 0xF8 AND R = 1 0 0 0 0 0 0 0 = 0x80
    // Agora e só fazer um OR com os bits que se quer acrescentar
    // se quero esconder 101
    // 101 = 0x5
    // 0x80 OR 0x5 = 1 0 0 0 0 1 0 1

    int mascara = defineMascara(numBits);

    /**
      Passo 1 : Escondendo o tamanho
    **/
    int quantPixels = (sizeof(int) * 8)/(3*numBits);
    if (( (sizeof(int) * 8) % 3*numBits) != 0)
    {
        quantPixels++;
    }

    int pixelLargura = 0;
    int pixelAltura = 0;
    int maxLargura = retorno.size().width();
    int posAtualInt = 31;
    for (int i = 0; i < quantPixels; i++)
    {
        QRgb orgPixel = retorno.pixel(pixelLargura, pixelAltura);
        int vermelho = qRed(orgPixel);
        int azul = qBlue(orgPixel);
        int verde = qGreen(orgPixel);
        int alpha = qAlpha(orgPixel);

        // Altera pixel
        // Altera vermelho
        int flippedBits = pegaBits(tamMensagem, posAtualInt, numBits);
        posAtualInt -= numBits;
        // Aplica a máscara
        vermelho = (vermelho & mascara);
        // Muda LSB
        vermelho = (vermelho | flippedBits);

        if (posAtualInt >= 0)
        {
            // Altera verde
            flippedBits = pegaBits(tamMensagem, posAtualInt, numBits);
            posAtualInt -= numBits;
            // Aplica a máscara
            verde = (verde & mascara);
            // Muda LSB
            verde = (verde | flippedBits);
        }

        if (posAtualInt >= 0)
        {
            // Altera azul
            flippedBits = pegaBits(tamMensagem, posAtualInt, numBits);
            posAtualInt -= numBits;
            // Aplica a máscara
            azul = (azul & mascara);
            // Muda LSB
            azul = (azul | flippedBits);
        }

        QRgb modPixel = qRgba(vermelho, verde, azul, alpha);
        retorno.setPixel(pixelLargura, pixelAltura, modPixel);

        // Muda posição do pixel que eu to pegando no laço
        pixelLargura++;
        if (pixelLargura >= maxLargura)
        {
            pixelAltura++;
            pixelLargura = 0;
        }
    }

    // Escondendo mensagem
    quantPixels = (sizeof(int) * 8)/(3*numBits);
    if (( (sizeof(int) * 8) % 3*numBits) != 0)
    {
        quantPixels++;
    }

    for (int j = 0; j < tamMensagem; j++)
    {
        int temp = mensagem.at(j).toAscii();
        posAtualInt = 31;
        for(int i = 0; i < quantPixels; i++)
        {
            QRgb orgPixel = retorno.pixel(pixelLargura, pixelAltura);
            int vermelho = qRed(orgPixel);
            int azul = qBlue(orgPixel);
            int verde = qGreen(orgPixel);

            // Altera pixel
            // Altera vermelho
            int flippedBits = pegaBits(temp, posAtualInt, numBits);
            posAtualInt -= numBits;
            // Aplica a máscara
            vermelho = (vermelho & mascara);
            // Muda LSB
            vermelho = (vermelho | flippedBits);

            if (posAtualInt >= 0)
            {
                // Altera verde
                flippedBits = pegaBits(temp, posAtualInt, numBits);
                posAtualInt -= numBits;
                // Aplica a máscara
                verde = (verde & mascara);
                // Muda LSB
                verde = (verde | flippedBits);
            }

            if (posAtualInt >= 0)
            {
                // Altera azul
                flippedBits = pegaBits(temp, posAtualInt, numBits);
                posAtualInt -= numBits;
                // Aplica a máscara
                azul = (azul & mascara);
                // Muda LSB
                azul = (azul | flippedBits);
            }

            QRgb modPixel = qRgba(vermelho, verde, azul, 0);
            retorno.setPixel(pixelLargura, pixelAltura, modPixel);

            // Muda posição do pixel que eu to pegando no laço
            pixelLargura++;
            if (pixelLargura >= maxLargura)
            {
                pixelAltura++;
                pixelLargura = 0;
            }
        }
    }

    return retorno;
}
