#pragma once
#include "No.h"
#include <vector>
#include <list>
#define INFINITO 99999999.0

class GrafoM
{
//private:
public:
	int tamanho;	//n° de vertices
	int **matriz;	// tamanho X tamanho
	No *vertices;


	GrafoM(int tamanho);
	~GrafoM();

	void criaAdjacencia(int i, int j);  // cria uma adjacência entre i e j com custo no grafo G;
	void removeAdjacencia(int i, int j);  // remove a adjacência entre i e j no grafo G;
	void imprimeAdjacencias();  // imprime a matriz de adjacências do grafo G
	void setaInformacao(int i, std::string V);  // atualiza a informação do nó i com o valor V (que deve ser uma string) no grafo G
	int adjacentes(int i, int *adj);  // retorna o número de adjacentes ao vértice i no grafo G e os armazena no vetor adj

	bool** fechamento();
	int melhorCaminho(int s, int t);
	void imprimeCaminho(int s, int t, int *caminho);

	int qualVertice(std::string vertice);

	int nArestas();

	std::vector<std::pair<std::string, int>> maiorGrauEntrada();
	std::vector<std::pair<std::string, int>>  maiorGrauSaida();

	std::list<int> verticesNaDistancia(int distancia, int vertice);
	void verticesNaDistancia_Recursivo(int vertice, int nivel, int distancia, std::list<int> *lista, int verticeAnterior, int de);

};



