/*****************************************************************/
/*         Tabela de Dispersao | PROG2 | MIEEC | 2020/21         */
/*****************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tabdispersao.h"
#include "grafo.h"



tabela_dispersao *tabela_nova(int capacidade, hash_func *hfunc, sond_func *sfunc)
{
    if(!capacidade || !hfunc || !sfunc)
    return NULL;

    tabela_dispersao *td= (tabela_dispersao*) malloc (sizeof(tabela_dispersao));
    if(!td)
    return NULL;
    td->capacidade=capacidade;
    td->tamanho=0;
    td->nos=malloc(sizeof(no_grafo*)*capacidade);
    if(!td->nos)
    {
        free(td);
        return NULL;
    }
    for(int i=0; i<capacidade; i++)
    {
        td->nos[i]=NULL;
    }
    td->estado_celulas=malloc(sizeof(int*)*capacidade);
    if(!td->estado_celulas)
    {
        free(td);
        free(td->nos);
        return NULL;
    }
    for(int i=0; i<capacidade; i++)
    {
        td->estado_celulas[i]=0;
    }
    td->hfunc=hfunc;
    td->sfunc=sfunc;

    return td;
}

int tabela_adiciona(tabela_dispersao *td, no_grafo *entrada)
{
    if(!td || !entrada)
    return -1;

    int i;

    i=td->hfunc(entrada->cidade, td->capacidade);

    
    if(td->estado_celulas[i]==1)
    {
        if(strcmp(td->nos[i]->cidade, entrada->cidade)==0)
        {
            return -1;
        }
        int k=i;
        for(int j=0;j<=td->capacidade;j++)
        {
            i=td->sfunc(k,j,td->capacidade);
            if(td->estado_celulas[i]==0 || td->estado_celulas[i]==-1)
            {
                td->nos[i]=entrada;
                break;
            }
        }
        td->tamanho++;
        td->estado_celulas[i]=1;
        td->nos[i]=entrada;
        return i;
    }
    if(td->estado_celulas[i]==0 || td->estado_celulas[i]==-1)
    { 
        td->tamanho++;
        td->estado_celulas[i]=1;
        td->nos[i]=entrada;
        return i;        
    }
    
   return -1;
}   

int tabela_remove(tabela_dispersao *td, no_grafo *saida)
{
    if(!td || !saida)
    return -1;

    for(int i=0; i<td->capacidade; i++)
    {
        if(td->estado_celulas[i]==1)
        {
            if(strcmp(td->nos[i]->cidade, saida->cidade)==0)
            {
                td->nos[i]=NULL;
                td->estado_celulas[i]=-1;
                td->tamanho--;
                return 0;
            }
        }
    }
    return -1;

}

void tabela_apaga(tabela_dispersao *td)
{
    free (td->nos);

    free(td->estado_celulas);
    
    free(td);
}

int tabela_existe(tabela_dispersao *td, const char *cidade)
{
    if(!td || !cidade)
    return -1;

    for(int i=0; i<td->capacidade; i++)
    {
        if(td->estado_celulas[i]==1)
        {
            if(strcmp(td->nos[i]->cidade,cidade)==0)
            return i;
        }
    }
    return -1;
}

tabela_dispersao *tabela_carrega(grafo *g, int tamanho)
{
    if(!g || !tamanho)
    return NULL;

    if(g->tamanho>tamanho)
    return NULL;

    tabela_dispersao *td=tabela_nova(tamanho, hash_krm, sond_rh);
    if(!td)
    return NULL;

    for(int i=0; i<g->tamanho; i++)
    {
        if(tabela_adiciona(td, g->nos[i])==-1)
        {
            tabela_apaga(td);
            return NULL;
        }
    }

    return td;

}

/*================================================================================*/
unsigned long sond_rh(int pos, int tentativas, int tamanho)
{

    return (pos + tentativas * tentativas) % tamanho;
}

unsigned long hash_krm(const char *chave, int tamanho)
{
    int c, t = strlen(chave);
    unsigned long hash = 7;

    for (c = 0; c < t; c++)
    {
        hash += (int)chave[c];
    }

    return hash % tamanho;

    return 0;
}
