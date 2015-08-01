#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include "QImage.h"

class Algoritmos
{
    public:
        Algoritmos();
        Algoritmos(QImage imagem);

        QImage aplicaLSB( int numBits, QString mensagem );

    private:
        QImage imagemOriginal;

        int defineMascara(int numBits);
        int descobreBitDaPosicao(int num, int pos);
        int pegaBits(int num, int posIni, int numBits);
};

#endif // ALGORITMOS_H
