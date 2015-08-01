#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include "QImage.h"

class Algoritmos
{
    public:
        Algoritmos();
        Algoritmos(QImage img);

        int pegaTamanhoLsbSimples(int numBits, int pixelIniLargura = 0, int pixelIniAltura = 0);
        QString pegaMensagemLsbSimples(int tamanho, int numBits, int pixelIniLargura = 0, int pixelIniAltura = 0);

    private:
        QImage imagem;

        int descobreBitDaPosicao(int num, int pos);
        int pegaBits(int num, int posIni, int numBits);
};

#endif // ALGORITMOS_H
