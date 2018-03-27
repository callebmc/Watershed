#include <stdio.h>
#include <stdlib.h>
#include "pdi.h"

#define A "0.bmp"
#define THD_BIN 0.10
#define SUBIDA 1


void binarizaCalleb (Imagem* in, Imagem* out, float threshold)
{
    if (in->largura != out->largura || in->altura != out->altura || in->n_canais != out->n_canais)
    {
        printf ("ERRO: binariza: as imagens precisam ter o mesmo tamanho e numero de canais.\n");
        exit (1);
    }

    int row, col, canal;

    for(canal = 0; canal < in->n_canais; canal++)
    {
        for(row = 0; row < in->altura; row++)
        {
            for(col = 0; col < in->largura; col++)
            {
                if(in->dados[canal][row][col] <= threshold)
                    out->dados[canal][row][col] = 0;
                else
                    out->dados[canal][row][col] = -1;
            }
        }
    }
}

void floodfillWS(Imagem* in, int row, int col)
{



    if(in->dados[0][row][col]==-1)
    {
        in->dados[0][row][col]=0;
        if(col-1>0 )
        {
            floodfillWS(in, row, col-1);
        }
        if(col+1<in->largura )
        {
            floodfillWS(in, row, col+1);
        }
        if(row-1>0 )
        {
            floodfillWS(in, row-1, col);
        }
        if(row+1<in->altura )
        {
            floodfillWS(in, row+1, col);
        }
        if(row-1>0 && col-1>0 )
        {
            floodfillWS(in, row-1, col-1);
        }
        if(row-1>0 && col+1<in->largura )
        {
            floodfillWS(in, row-1, col+1);
        }
        if(row+1<in->altura && col-1>0 )
        {
            floodfillWS(in, row+1, col-1);
        }
        if(row+1<in->altura && col+1<in->largura )
        {
            floodfillWS(in, row+1, col+1);
        }
        if(col-2>0 )
        {
            floodfillWS(in, row, col-2);
        }

        if(col+2<in->largura )
        {
            floodfillWS(in, row, col+2);
        }

        if(row-2>0)
        {
            floodfillWS(in, row-2, col);
        }

        if(row+2<in->altura )
        {
            floodfillWS(in, row+2, col);
        }

    }

}


int main()
{

    Imagem* img = abreImagem(A, 1);

    Imagem* aux_dx = criaImagem(img->largura, img->altura, 1);
    Imagem* aux_dy = criaImagem(img->largura, img->altura, 1);
    Imagem* aux_mag = criaImagem(img->largura, img->altura, 1);
    Imagem* fin = criaImagem(img->largura, img->altura, 1);

    computaGradientes(img, 1, aux_dx, aux_dy, aux_mag, fin);

    salvaImagem(aux_mag, "GRADIENTE.bmp");
    binariza(img, img, THD_BIN);
    salvaImagem(img, "SAIDA.bmp");

    int row, col;
    float a=0.02;
    float b=0;

    //TRANSFORMADA DA DISTANCIA
    while(a<=10)
    {
        for(row = 0; row < img->altura; row++)
        {
            for(col = 0; col < img->largura; col++)
            {
                if(img->dados[0][row][col] == b)
                {
                    if(row-1>=0 && img->dados[0][row-1][col] == -1 )
                    {
                        img->dados[0][row-1][col]=a;
                    }
                    if(row+1<img->altura && img->dados[0][row+1][col] == -1)
                    {

                        img->dados[0][row+1][col]=a;
                    }
                    if(col-1>=0 && img->dados[0][row][col-1] == -1 )
                    {
                        img->dados[0][row][col-1]=a;
                    }
                    if(col+1<img->largura && img->dados[0][row][col+1] == -1)
                    {
                        img->dados[0][row][col+1]=a;
                    }
                }
            }
        }
        a=a+0.02;
        b=b+0.02;
    }
    salvaImagem(img, "hmm.bmp");

    //ACHA MÁXIMOS LOCAIS PARA SEED

    Imagem* mascara=criaImagem(img->largura, img->altura, 1);
    for(row = 0; row < img->altura; row++)
    {
        for(col = 0; col < img->largura; col++)
        {
            mascara->dados[0][row][col]=0;
        }
    }

    Lista* p=criar();

    for(row = 0; row < img->altura; row++)
    {
        for(col = 0; col < img->largura; col++)
        {
            if(row-1>0 && col-1>0 && col+1<img->largura && row+1<img->altura &&
                    img->dados[0][row][col]>0 && img->dados[0][row][col]>=img->dados[0][row-1][col] &&
                    img->dados[0][row][col]>=img->dados[0][row+1][col] && img->dados[0][row][col]>=img->dados[0][row][col-1] &&
                    img->dados[0][row][col]>=img->dados[0][row][col+1] && img->dados[0][row][col]>=img->dados[0][row-1][col-1] &&
                    img->dados[0][row][col]>=img->dados[0][row-1][col+1] && img->dados[0][row][col]>=img->dados[0][row+1][col-1] &&
                    img->dados[0][row][col]>=img->dados[0][row+1][col+1])
            {
                mascara->dados[0][row][col] = -1;
            }
        }
    }

    Imagem* mask=criaImagem(img->largura, img->altura, 1);
    for(row = 0; row < img->altura; row++)
    {
        for(col = 0; col < img->largura; col++)
        {
            mask->dados[0][row][col]=0;
        }
    }

    float label=1;
    for(row = 0; row < img->altura; row++)
    {
        for(col = 0; col < img->largura; col++)
        {
            if(mascara->dados[0][row][col]==-1)
            {
                mask->dados[0][row][col]=label;
                p=inserir(p, label, row, col, aux_mag->dados[0][row][col]);
                floodfillWS(mascara, row, col);
                label++;
            }
        }
    }
    //imprimir(p);
    salvaImagem(mask, "Mascara2.bmp");

    //TRANSBORDAR
    Imagem* visitado=criaImagem(img->largura, img->altura, 1);
    for(row = 0; row < img->altura; row++)
    {
        for(col = 0; col < img->largura; col++)
        {
            visitado->dados[0][row][col]=0;
        }
    }


    Lista* k;
    float menor;
    int l, c;
    while(p)
    {
        menor=men(p);
        for(k=p; k!=NULL && k->gradiente!=menor; k=k->next);
        row = k->cord_x;
        col = k->cord_y;
        label = k->label;
        visitado->dados[0][row][col]=1;
        p=remover(p, menor);
        for(l=row-1; l<=row+1;l++){
            for(c=col-1; c<=col+1;c++){
                if(l>0 && c>0 && c<img->largura && l<img->altura){
                    if(mask->dados[0][l][c]==0 && visitado->dados[0][l][c]==0){
                        mask->dados[0][l][c]=label;
                        p=inserir(p, label, l, c, aux_mag->dados[0][l][c]);
                    }
                    if(mask->dados[0][l][c]!=label && visitado->dados[0][l][c]==0){
                        mask->dados[0][l][c]=-5;
                    }

                }

            }

        }
    }
    imprimir(p);
    salvaImagem(mask, "Mascara.bmp");

    int red=1, blue=0, green=0;
    Imagem* queijo=abreImagem(A, 3);
    for(row=0;row<img->altura;row++){
        for(col=0;col<img->largura;col++){
            if(mask->dados[0][row][col]==-5){
                queijo->dados[0][row][col]=red;
                queijo->dados[1][row][col]=blue;
                queijo->dados[2][row][col]=green;
            }
        }
    }
    salvaImagem(queijo, "FINAL.bmp");

    return 0;
}
