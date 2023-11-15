/*****************************************************************/
/*    Estrutura nova a implementar | PROG2 | MIEEC | 2020/21     */
/*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stnova.h"

estrutura *st_nova()
{
    estrutura *st = (estrutura*) malloc(sizeof(estrutura));
    if(!st)
    return NULL;

    st->raiz = NULL;

    return st;
}

int st_importa_grafo(estrutura *st, grafo *g)
{
    if(!st || !g)
    return -1;

    for(int i=0; i<g->tamanho; i++)
    {
        if(st_insere(st, g->nos[i])==-1)
        {
            st_apaga(st);
            return -1;
        }
    }

    return 0;
}

char *st_pesquisa(estrutura *st, char *origem, char *destino)
{
    if(!st || !origem || !destino)
        return NULL;
    int mini=0;
    double preco_min=99999999;
    no_grafo *nog;

    nog=st_pesquisa_no(st->raiz, origem);
    if(!nog)
    return NULL;
    for(int i=0; i<nog->tamanho; i++)
    {   
        if(strcmp(nog->arestas[i]->destino->cidade,destino)==0)
        {
            if(preco_min>nog->arestas[i]->preco)
            {
                preco_min=nog->arestas[i]->preco;
                mini=i;
            }
        }
    }

    return nog->arestas[mini]->codigo;
}

int st_apaga(estrutura *st)
{
    if (!st)
    return -1;
    while(st->raiz != NULL)
    {
        st->raiz = st_remove(st->raiz, st->raiz->no);
    }

    free(st);
    return 0;
}

int st_insere(estrutura *st, no_grafo *nog)
{
    if(!st || !nog)
        return -1;

    st->raiz = st_insere_no(st->raiz, nog);
    return 0;
}

int st_altura(no_avl *noa)
{
    if (!noa)
        return -1;
    return noa->altura;
}

int max(int a, int b)
{
    return (a > b)? a : b;
}

no_avl* avl_novo_no(no_grafo *nog)
{
    no_avl *raiz = (no_avl*) malloc(sizeof(no_avl));
    raiz->no=nog;
    raiz->esquerda = NULL;
    raiz->direita  = NULL;
    raiz->altura = 0;
    return raiz;
}

no_avl* roda_direita(no_avl *y)
{
    no_avl *x = y->esquerda;
    no_avl *T2 = x->direita;

    x->direita = y;
    y->esquerda = T2;

    y->altura = max(st_altura(y->esquerda), st_altura(y->direita))+1;
    x->altura = max(st_altura(x->esquerda), st_altura(x->direita))+1;

    return x;
}

no_avl* roda_esquerda(no_avl *x)
{
    no_avl *y = x->direita;
    no_avl *T2 = y->esquerda;

    y->esquerda = x;
    x->direita = T2;

    x->altura = max(st_altura(x->esquerda), st_altura(x->direita))+1;
    y->altura = max(st_altura(y->esquerda), st_altura(y->direita))+1;

    return y;
}

int calc_balanceamento(no_avl *N)
{
    if (!N)
        return 0;
    return st_altura(N->direita) - st_altura(N->esquerda);
}

no_grafo* st_pesquisa_no(no_avl* noa, char *origem)
{
    if(!noa)
        return NULL;

    if(strcmp(origem, noa->no->cidade) < 0)
        return st_pesquisa_no(noa->esquerda, origem);

    else if(strcmp(origem, noa->no->cidade) > 0)
        return st_pesquisa_no(noa->direita, origem);

    else
        return noa->no;
}

no_avl* st_insere_no(no_avl *noa, no_grafo *nog)
{
    if (!noa)
    return avl_novo_no(nog);

    if (strcmp(nog->cidade, noa->no->cidade) < 0)
        noa->esquerda  = st_insere_no(noa->esquerda, nog);
    else if(strcmp(nog->cidade, noa->no->cidade) > 0)
        noa->direita = st_insere_no(noa->direita, nog);
    else 
    {
        return noa;
    }

    noa->altura = max(st_altura(noa->esquerda), st_altura(noa->direita)) + 1;

    int balance = calc_balanceamento(noa);

    if (balance > 1) {
    	if (calc_balanceamento(noa->direita) < 0) {
			noa->direita = roda_direita(noa->direita);
			return roda_esquerda(noa);
    	} else {
    		return roda_esquerda(noa);
    	}
    }
    else if (balance < -1) {
    	if (calc_balanceamento(noa->esquerda) > 0) {
			noa->esquerda = roda_esquerda(noa->esquerda);
			return roda_direita(noa);
    	} else {
    		return roda_direita(noa);
    	}
    }
    return noa;
}

no_avl* st_no_valormin(no_avl* no)
{
    no_avl* curr = no;

    while (curr->esquerda != NULL)
        curr = curr->esquerda;

    return curr;
}

no_avl* st_remove(no_avl* noa, no_grafo *nog)
{
    if (!noa)
    return noa;

    if ( strcmp(nog->cidade, noa->no->cidade) < 0 )
        noa->esquerda = st_remove(noa->esquerda, nog);

    else if( strcmp(nog->cidade, noa->no->cidade) > 0 )
        noa->direita = st_remove(noa->direita, nog);

    else
    {
        if( (noa->esquerda == NULL) || (noa->direita == NULL) )
        {
            no_avl *temp = noa->esquerda ? noa->esquerda : noa->direita;

            if(!temp)
            {
                temp = noa;
                noa = NULL;
            }
            else
            {
                noa->no=temp->no;
                noa->esquerda = temp->esquerda;
                noa->direita = temp->direita;
                noa->altura = temp->altura;
            }
            free(temp);
        }
        else
        {
            no_avl* temp = st_no_valormin(noa->direita);

            noa->no =temp->no;

            noa->direita = st_remove(noa->direita, temp->no);
        }
    }

    if (!noa)
      return noa;

    noa->altura = max(st_altura(noa->esquerda), st_altura(noa->direita)) + 1;

    int balance = calc_balanceamento(noa);

    if (balance > 1) {
        if (calc_balanceamento(noa->direita) < 0) {
            noa->direita = roda_direita(noa->direita);
            return roda_esquerda(noa);
        } else {
            return roda_esquerda(noa);
        }
    }
    else if (balance < -1) {
        if (calc_balanceamento(noa->esquerda) > 0) {
            noa->esquerda = roda_esquerda(noa->esquerda);
            return roda_direita(noa);
        } else {
            return roda_direita(noa);
        }
    }
    return noa;
}