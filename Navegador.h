#pragma once
#include <vector>
#include "GrafoM.h"
#include <sstream>
#include <list>

class Navegador
{
private:
	GrafoM *grafo;
	std::string endereco;
	std::vector<std::pair<std::string, std::string>> remetentes;
	std::vector<std::string> destinatarios;

	void criarVertices();
	void criarAdjacencias();	
	int buscaLargura_Recursivo(int para, std::list<int> *fila, bool *visitado);
	int buscaProfundidade_Recursivo(int de, int para, bool *visitado);	
	bool iterarFila(int valor, std::list<int> *fila);
	int qualVertice(std::string vertice);
	void imprimeRemetentes();

public:
	Navegador(std::string endereco);
	~Navegador();

	int buscaProfundidade(int de, int para);
	int buscaLargura(int de,int para);
	int nVertices();
	int nArestas();
	void maiorGrauEntrada();
	void maiorGrauSaida();
	bool verticeNaDistancia(int distancia, std::string vertice);
};

