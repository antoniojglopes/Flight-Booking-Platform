/*****************************************************************/
/*          Grafo direcionado | PROG2 | MIEEC | 2020/21          */
/*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "grafo.h"
#include "heap.h"

void mudar_proridade(heap *h, no_grafo *no, double prio);

grafo *grafo_novo()
{
    grafo *g = (grafo *)malloc(sizeof(grafo));
    g->tamanho = 0;
    g->nos = NULL;

    return g;
}

no_grafo *encontra_no(grafo *g, char *cidade)
{
    if (!g || !cidade)
        return NULL;

    // pesquisa por cidade no vetor de nós
    for (int i = 0; i < g->tamanho; i++)
    {
        if (strcmp(g->nos[i]->cidade, cidade) == 0)
            return g->nos[i];
    }
    return NULL;
}

no_grafo *no_insere(grafo *g, char *cidade)
{
    if (!g || !cidade)
        return NULL;

    // verifica se o nó já existe
    no_grafo *no = encontra_no(g, cidade);
    if (no)
        return NULL;

    // cria o novo nó para o user
    no = (no_grafo *)malloc(sizeof(no_grafo));
    if (!no)
        return NULL;

    // aloca espaço para o campo cidade
    no->cidade = (char *)malloc((strlen(cidade) + 1) * sizeof(char));
    if (!no->cidade)
    {
        free(no);
        return NULL;
    }
    // inicializa todos os campos
    strcpy(no->cidade, cidade);
    no->tamanho = 0;
    no->arestas = NULL;
    no->p_acumulado = 0;
    no->anterior = NULL;
    no->dataatualizada = NULL;

    // insere o nó criado no final do vetor de nós
    g->nos = (no_grafo **)realloc(g->nos, (g->tamanho + 1) * sizeof(no_grafo *));
    if (!g->nos)
    {
        free(no->cidade);
        free(no);
        return NULL;
    }
    // incrementa o tamanho do numero de nós e insere no vetor de apontadores de nós
    g->tamanho++;
    g->nos[g->tamanho - 1] = no;

    return no;
}

// função auxiliar que retorna 1 se existir a aresta para destino ou 0,
// se a aresta não existir, -1 se der erro
int existe_aresta(no_grafo *origem, no_grafo *destino, char *codigo)
{

    if (!origem || !destino)
        return -1;

    //pesquisa em todas as arestas se existe
    for (int i = 0; i < origem->tamanho; i++)
    {

        if ((origem->arestas[i]->destino == destino) && ((strcmp(origem->arestas[i]->codigo, codigo) == 0)))
            return 1;
    }

    return 0;
}

int cria_aresta(no_grafo *origem, no_grafo *destino, char *codigo, char *companhia, data partida, data chegada, float preco, int lugares)
{
    if (!origem || !destino || !codigo || !companhia)
        return -1;

    if (preco < 0 || lugares < 0)
        return -1;

    // verifica se a ligação já existe
    if (existe_aresta(origem, destino, codigo) > 0)
        return -1;

    // cria a nova ligação
    aresta_grafo *ag = (aresta_grafo *)malloc(sizeof(aresta_grafo));
    if (!ag)
        return -1;

    ag->destino = destino;
    ag->preco = preco;
    ag->lugares = lugares;
    // aloca espaço para o código
    ag->codigo = (char *)malloc((strlen(codigo) + 1) * sizeof(char));
    if (!ag->codigo)
    {
        free(ag);
        return -1;
    }
    strcpy(ag->codigo, codigo);
    // aloca espaço para o companhia
    ag->companhia = (char *)malloc((strlen(companhia) + 1) * sizeof(char));
    if (!ag->companhia)
    {
        free(ag->codigo);
        free(ag);
        return -1;
    }
    strcpy(ag->companhia, companhia);

    // inicializa todos os campos
    ag->partida.tm_year = partida.tm_year;
    ag->partida.tm_mon = partida.tm_mon;
    ag->partida.tm_mday = partida.tm_mday;
    ag->partida.tm_hour = partida.tm_hour;
    ag->partida.tm_min = partida.tm_min;
    ag->partida.tm_sec = 0;
    ag->partida.tm_isdst = 0;

    // inicializa todos os campos
    ag->chegada.tm_year = chegada.tm_year;
    ag->chegada.tm_mon = chegada.tm_mon;
    ag->chegada.tm_mday = chegada.tm_mday;
    ag->chegada.tm_hour = chegada.tm_hour;
    ag->chegada.tm_min = chegada.tm_min;
    ag->chegada.tm_sec = 0;
    ag->chegada.tm_isdst = 0;

    // insere a nova ligação no vetor de ligações
    origem->arestas = (aresta_grafo **)realloc(origem->arestas, (origem->tamanho + 1) * sizeof(aresta_grafo *));
    if (!origem->arestas)
    {
        free(ag->codigo);
        free(ag->companhia);
        free(ag);
        return -1;
    }
    origem->tamanho++;
    origem->arestas[origem->tamanho - 1] = ag;

    return 0;
}

no_grafo *no_remove(grafo *g, char *cidade)
{
    if(!g || !cidade)
    return NULL;

    int l;
    no_grafo *no;

    for(int i=0;i<g->tamanho;i++)
    {
        if(strcmp(g->nos[i]->cidade, cidade)==0)
        {    
            no=g->nos[i];
            
            for(int k=0;k<g->tamanho;k++)
            {
                for(int j=0; j<g->nos[k]->tamanho;j++)
                {
                    if(strcmp(no->cidade, g->nos[k]->arestas[j]->destino->cidade)==0)
                    {
                        if(aresta_apaga(g->nos[k]->arestas[j])==-1)
                        return NULL;
                        for(l=j; l<g->nos[k]->tamanho-1; l++)
                        g->nos[k]->arestas[l]=g->nos[k]->arestas[l+1];
                        g->nos[k]->arestas=realloc(g->nos[k]->arestas, sizeof(aresta_grafo)*(g->nos[k]->tamanho-1));
                        g->nos[k]->tamanho--;
                        j--;
                    }
                }
            }
            for (int a=i; a<g->tamanho-1; a++)
            {
                g->nos[a]=g->nos[a+1];
            }
            g->nos=realloc(g->nos, sizeof(no_grafo)*(g->tamanho-1));
            g->tamanho--;
            for(int i=0; i<no->tamanho; i++)
            {
                if(aresta_apaga(no->arestas[i])==-1)
                return NULL;
            }
            free(no->arestas);
            no->arestas=NULL;
            no->tamanho=0;
            return no;
        }
    }
    return NULL;
}

int aresta_apaga(aresta_grafo *aresta)
{
    if(!aresta)
    return -1;
    free(aresta->codigo);
    free(aresta->companhia);
    free(aresta);
    return 0;
}

int no_apaga(no_grafo *no)
{
    if(!no)
    return -1;
    for(int i=0; i<no->tamanho; i++)
    {
        aresta_apaga(no->arestas[i]);
    }
    free(no->arestas);
    free(no->cidade);
    free (no);
    return 0;
}

void grafo_apaga(grafo *g)
{
    for(int i=0; i<g->tamanho; i++)
    {
        no_apaga(g->nos[i]);
    }
    free(g->nos);

    free(g);
}

no_grafo *encontra_voo(grafo *g, char *codigo, int *aresta_pos)
{
    if(!g || !codigo || !aresta_pos)
        return NULL;

    no_grafo *no;

    for(int i=0; i<g->tamanho; i++)
    {   
        for(int j=0; j<g->nos[i]->tamanho; j++)
        {
            if(strcmp(codigo, g->nos[i]->arestas[j]->codigo)==0)
            {
                *aresta_pos=j;
                no=g->nos[i];
                return no;
            }
        }
    }
    return NULL;
}

no_grafo **pesquisa_avancada(grafo *g, char *destino, data chegada, double preco_max, int *n)
{
    if(!g || !destino || !chegada.tm_year || !chegada.tm_mon || !chegada.tm_mday || !preco_max || !n)
        return NULL;

    no_grafo **nos=malloc(sizeof(no_grafo*));
    if(!nos)
        return NULL;

    *n=0;

    for(int i=0; i<g->tamanho; i++)
    {
        for(int j=0; j<g->nos[i]->tamanho; j++)
        {
            if(strcmp(destino, g->nos[i]->arestas[j]->destino->cidade)==0 && g->nos[i]->arestas[j]->chegada.tm_mday==chegada.tm_mday && g->nos[i]->arestas[j]->chegada.tm_mon==chegada.tm_mon && g->nos[i]->arestas[j]->chegada.tm_year==chegada.tm_year && g->nos[i]->arestas[j]->preco<=preco_max)
            {   
                *n+=1;
                nos=realloc(nos, sizeof(no_grafo*)*(*n));
                nos[*n-1]=g->nos[i];
                
            }
        }
    }
    if (*n==0)
    return NULL;

    return nos;
}

no_grafo **trajeto_mais_rapido(grafo *g, char *origem, char *destino, data partida, int *n)
{
    if(!g || !origem || !destino || !n)
        return NULL;

    heap *h=heap_nova(g->tamanho);
    if(!h)
        return NULL;
    no_grafo *no=NULL;
    no_grafo *aux=NULL;
    double t;
    no_grafo **nos=malloc(sizeof(no_grafo*)*g->tamanho);
    if(!nos)
    {
        heap_apaga(h);
        return NULL;
    }
    no_grafo **percursoaux=malloc(sizeof(no_grafo*)*g->tamanho);
    if(!percursoaux)
    {
        heap_apaga(h);
        free(nos);
        return NULL;
    }
    no_grafo **percurso=malloc(sizeof(no_grafo*)*g->tamanho);
    if(!percurso)
    {
        heap_apaga(h);
        free(nos);
        free(percursoaux);
        return NULL; 
    }
    *n=0;
    
    for(int i=0; i<g->tamanho; i++)
    {
        if (strcmp(g->nos[i]->cidade, origem)==0)
        {
            g->nos[i]->p_acumulado=0;
            g->nos[i]->anterior=NULL;
            g->nos[i]->dataatualizada=NULL;
            if(heap_insere(h, g->nos[i], 0)==0)
            {
                free(nos);
                free(percursoaux);
                free(percurso);
                heap_apaga(h);
                return NULL;
            }
            continue;
        }
        g->nos[i]->p_acumulado=99999999;
        g->nos[i]->anterior=NULL;
        g->nos[i]->dataatualizada=NULL;
        if(heap_insere(h, g->nos[i], 99999999)==0)
            {
                free(nos);
                free(percursoaux);
                free(percurso);
                heap_apaga(h);
                return NULL;
            }
    }

    while(h->tamanho!=0)
    {
        no=heap_remove(h);
        if(!no)
        {
            free(nos);
            free(percursoaux);
            free(percurso);
            heap_apaga(h);
            return NULL;
        }
        nos[*n]=no;
        *n+=1;
        for(int j=0; j<no->tamanho;j++)
        {
            int d=0;
            if(no->anterior!=NULL)
            {
                if(mktime(&partida)<=mktime(&no->arestas[j]->partida))
                { 
                    if(mktime(&no->arestas[j]->partida)>=mktime(no->anterior->dataatualizada))
                    {
                        if(mktime(&no->arestas[j]->chegada)>=mktime(&no->arestas[j]->partida))
                        t=difftime(mktime(&no->arestas[j]->chegada),mktime(&no->arestas[j]->partida))+difftime(mktime(&no->arestas[j]->partida),mktime(no->anterior->dataatualizada));
                        d=1;
                    }
                    else
                    continue;
                }
            }

            else if(mktime(&partida)<=mktime(&no->arestas[j]->partida))
            { 
                if(mktime(&no->arestas[j]->chegada)>=mktime(&no->arestas[j]->partida))
                t=difftime(mktime(&no->arestas[j]->chegada),mktime(&no->arestas[j]->partida));
                d=1;
            }

            if(no->arestas[j]->destino->p_acumulado>t+no->p_acumulado && d==1)
            {
                no->arestas[j]->destino->p_acumulado=t+no->p_acumulado;
                no->arestas[j]->destino->anterior=no;
                no->dataatualizada=&no->arestas[j]->chegada;
                mudar_proridade(h, no->arestas[j]->destino, no->arestas[j]->destino->p_acumulado);
            }
        }
    }
    if(*n<=1)
    return NULL;

    for(int m=0; m<*n; m++)
    {
        if(nos[m]!=NULL)
        {
            if(strcmp(nos[m]->cidade, destino)==0)
            {
                aux=nos[m];
                break;
            }
        }
    }
    
    int q=1;
    percursoaux[0]=aux;
    while(aux->anterior!=NULL)
    {
        percursoaux[q]=aux->anterior;
        aux=aux->anterior;
        q++;
    }

    if(strcmp(percursoaux[q-1]->cidade,origem)!=0)
    return NULL;

    int p=q;
    for(int k=0;k<q;k++)
    {
        percurso[k]=percursoaux[p-1];
        p--;
    }

    heap_apaga(h);
    free(nos);
    free(percursoaux);

    *n=q;
    
    return percurso;
}

no_grafo **menos_transbordos(grafo *g, char *origem, char *destino, data partida, int *n)
{  
    if(!g || !origem || !destino || !n)
    return NULL;

    heap *h=heap_nova(g->tamanho);
    if(!h)
        return NULL;
    no_grafo *no=NULL;
    no_grafo *aux=NULL;
    double t;
    no_grafo **nos=malloc(sizeof(no_grafo*)*g->tamanho);
    if(!nos)
    {
        heap_apaga(h);
        return NULL;
    }
    no_grafo **percursoaux=malloc(sizeof(no_grafo*)*g->tamanho);
    if(!percursoaux)
    {
        heap_apaga(h);
        free(nos);
        return NULL;
    }
    no_grafo **percurso=malloc(sizeof(no_grafo*)*g->tamanho);
    if(!percurso)
    {
        heap_apaga(h);
        free(nos);
        free(percursoaux);
        return NULL; 
    }
    *n=0;
    
    for(int i=0; i<g->tamanho; i++)
    {
        if (strcmp(g->nos[i]->cidade, origem)==0)
        {
            g->nos[i]->p_acumulado=0;
            g->nos[i]->anterior=NULL;
            g->nos[i]->dataatualizada=NULL;
            if(heap_insere(h, g->nos[i], 0)==0)
            {
                free(nos);
                free(percursoaux);
                free(percurso);
                heap_apaga(h);
                return NULL;
            }
            continue;
        }
        g->nos[i]->p_acumulado=99999999;
        g->nos[i]->anterior=NULL;
        g->nos[i]->dataatualizada=NULL;
        if(heap_insere(h, g->nos[i], 99999999)==0)
        {
            free(nos);
            free(percursoaux);
            free(percurso);
            heap_apaga(h);
            return NULL;
        }
    }

    while(h->tamanho!=0)
    {
        no=heap_remove(h);
        if(!no)
        {
            free(nos);
            free(percursoaux);
            free(percurso);
            heap_apaga(h);
            return NULL;
        }        
        nos[*n]=no;
        *n+=1;
        for(int j=0; j<no->tamanho;j++)
        {
            if(mktime(&partida) <= mktime(&no->arestas[j]->partida))
            {   
                t=1;
                if(no->arestas[j]->destino->p_acumulado > t+no->p_acumulado)
                {
                    no->arestas[j]->destino->p_acumulado=t+no->p_acumulado;
                    no->arestas[j]->destino->anterior=no;
                    mudar_proridade(h, no->arestas[j]->destino, no->arestas[j]->destino->p_acumulado);
                }
            }
        }
    }

    if(*n<=1)
    return NULL;

    for(int m=0; m<*n; m++)
    {
        if(nos[m]!=NULL)
        {
            if(strcmp(nos[m]->cidade, destino)==0)
            {
                aux=nos[m];
                break;
            }
        }
    }
    
    int q=1;
    percursoaux[0]=aux;
    while(aux->anterior!=NULL)
    {
        percursoaux[q]=aux->anterior;
        aux=aux->anterior;
        q++;
    }

    if(strcmp(percursoaux[q-1]->cidade,origem)!=0)
    return NULL;

    int p=q;
    for(int k=0;k<q;k++)
    {
        percurso[k]=percursoaux[p-1];
        p--;
    }

    heap_apaga(h);
    free(nos);
    free(percursoaux);

    *n=q;
    
    return percurso;
}

aresta_grafo **atualiza_lugares(char *ficheiro, grafo *g, int *n)
{
    if(!ficheiro || !g)
    return NULL;
    
    int p=0, k, a;
    char v[255], le[255], *tok;
    *n=0;
    no_grafo *no;
    aresta_grafo **aresta=malloc(sizeof(aresta_grafo*));
    if(!aresta)
        return NULL;
    FILE *f;
    f=fopen(ficheiro, "r");
    while(fgets(le, 255, f)!=NULL)
    {
        le[strlen(le)-1]='\0';
        tok=strtok(le, ",");
        strcpy(v, tok);
        tok=strtok(NULL, ",");
        p=atoi(tok);
        no=encontra_voo(g, v, &k);
        if(!no)
        {
            free(aresta);
            return NULL;
        }
        if(p==0)
        {   
            *n+=1;
            aresta=realloc(aresta ,sizeof(aresta_grafo*)* (*n));
            aresta[*n-1]=no->arestas[k];
            no->arestas[k]->lugares=p;
            for(int j=0; j<no->tamanho; j++)
            {
                if(strcmp(no->arestas[k]->codigo, v)==0)
                {
                    for(a=k; a<no->tamanho-1; a++)
                    no->arestas[a]=no->arestas[a+1];
                    no->arestas=realloc(no->arestas, sizeof(aresta_grafo)*(no->tamanho-1));
                    no->tamanho--;
                    j--;
                }
            }  
        }
        else
        {
            no->arestas[k]->lugares=p;
        }
    }
    fclose(f);
    return aresta;
}

grafo *grafo_importa(const char *nome_ficheiro)
{
    if (nome_ficheiro == NULL)
        return NULL;

    FILE *f = fopen(nome_ficheiro, "r");
    if (f == NULL)
        return NULL;

    grafo *g = grafo_novo();
    if (g == NULL)
    {
        fclose(f);
        return NULL;
    }
    char str[500] = {0}, codigo[15] = {0}, origem[50] = {0}, destino[50] = {0}, companhia[4] = {0};
    int lugares = 0;
    double preco = 0;
    struct tm partida, chegada;

    char *token;
    no_grafo *no_origem, *no_destino;
    int ret;
    while (fgets(str, 500 * sizeof(char), f) != NULL)
    {
        str[strlen(str) - 1] = '\0';

        token = strtok(str, ",");
        if (!token)
            break;
        strcpy(codigo, token);

        token = strtok(NULL, ",");
        strcpy(origem, token);

        token = strtok(NULL, ",");
        strcpy(destino, token);

        token = strtok(NULL, ",");
        partida.tm_year = atoi(token);

        token = strtok(NULL, ",");
        partida.tm_mon = atoi(token) - 1;

        token = strtok(NULL, ",");
        partida.tm_mday = atoi(token);

        token = strtok(NULL, ",");
        partida.tm_hour = atoi(token);

        token = strtok(NULL, ",");
        partida.tm_min = atoi(token);

        token = strtok(NULL, ",");
        chegada.tm_year = atoi(token);

        token = strtok(NULL, ",");
        chegada.tm_mon = atoi(token) - 1;

        token = strtok(NULL, ",");
        chegada.tm_mday = atoi(token);

        token = strtok(NULL, ",");
        chegada.tm_hour = atoi(token);

        token = strtok(NULL, ",");
        chegada.tm_min = atoi(token);

        token = strtok(NULL, ",");
        preco = atof(token);

        token = strtok(NULL, ",");
        lugares = atoi(token);

        token = strtok(NULL, ",");
        strcpy(companhia, token);

        // procura no grafo um nó com o nome da origem
        no_origem = encontra_no(g, origem);
        // se o nó ainda não existe, cria-o e insere-o no grafo
        if (!no_origem)
        {

            no_origem = no_insere(g, origem);

            if (!no_origem)
            {
                fclose(f);
                return NULL;
            }
        }
        // procura no grafo um nó com o nome do destino
        no_destino = encontra_no(g, destino);

        // se o nó ainda não existe, cria-o e insere-o no grafo
        if (!no_destino)
        {
            no_destino = no_insere(g, destino);

            if (!no_destino)
            {
                fclose(f);
                return NULL;
            }
        }

        if (existe_aresta(no_origem, no_destino, codigo) == 0)
        {

            ret = cria_aresta(no_origem, no_destino, codigo, companhia, partida, chegada, preco, lugares);

            if (ret == -1)
            {
                fclose(f);
                return NULL;
            }
        }
    }

    fclose(f);
    return g;
}

void mudar_proridade(heap *h, no_grafo *no, double prio)
{
    for(int i=0; i<h->tamanho; i++)
    {
        if(h->elementos[i]!=NULL)
        {
            if(strcmp(h->elementos[i]->no->cidade,no->cidade)==0)
            {
                h->elementos[i]->prioridade=prio;
                
                while(i!=1 && h->elementos[i]->prioridade<h->elementos[i/2]->prioridade)
                {
                    elemento *aux = h->elementos[i/2];
                    h->elementos[i/2]=h->elementos[i];
                    h->elementos[i]=aux;
                    i=i/2;
                }
                break;
            }
        }
    }
}
