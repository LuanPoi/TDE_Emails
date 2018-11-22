#include "GrafoM.h"
#include <iostream>
#include <vector>



GrafoM::GrafoM(int tamanho1) : tamanho(tamanho1)
{
	matriz = new int*[tamanho1];

	for (int i = 0; i < tamanho1; i++) {
		matriz[i] = new int[tamanho1];
	}

	for (int i = 0; i < tamanho; i++) {
		for (int j = 0; j < tamanho; j++) {
			matriz[i][j] = INFINITO;
		}
	}

	vertices = new No[tamanho];
}


GrafoM::~GrafoM()
{
}

void GrafoM::criaAdjacencia(int i, int j) {

	if (j == -1) {
		std::cout << "Usuario nao existe" << std::endl;
		return;
	}
	if (matriz[i][j] == INFINITO)
		matriz[i][j] = 1;
	else
		matriz[i][j]++;
	return;
}

void GrafoM::removeAdjacencia(int i, int j) {
	matriz[i][j] = INFINITO;
}

void GrafoM::imprimeAdjacencias() {
	for (int i = 0; i < tamanho; i++) {
		for (int j = 0; j < tamanho; j++) {
			if (matriz[i][j] == INFINITO)
				std::cout << 0 << " ";
			else
				std::cout << matriz[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

void GrafoM::setaInformacao(int i, std::string V) {
	vertices[i].setNome(V);
}

int GrafoM::adjacentes(int i, int *adj) {
	int k = 0;
	for (int j = 0; j < tamanho; j++) {
		if (matriz[i][j] != INFINITO) {
			adj[k] = j;
			k++;
		}

	}

	for (int j = k; j < tamanho; j++) {
		adj[j] = INFINITO;
	}
	return k;
}

bool** GrafoM::fechamento()
{

	// inicialização da matriz de fechamento
	bool **fechamento = new bool*[tamanho];

	for (int i = 0; i < tamanho; i++) {
		fechamento[i] = new bool[tamanho];
	}

	for (int i = 0; i < tamanho; i++) {
		for (int j = 0; j < tamanho; j++) {
			if (matriz[i][j] != INFINITO) {
				fechamento[i][j] = true;
			}
			else {
				fechamento[i][j] = false;
			}
		}
	}

	std::cout << "Antes:" << std::endl;
	for (int i = 0; i < tamanho; i++) {
		for (int j = 0; j < tamanho; j++) {
			std::cout << fechamento[i][j] << " ";
		}
		std::cout << std::endl;
	}

	system("pause");
	
	//algoritmo de Warshall

	int i, j, k;
	for (k = 0; k < tamanho; k++)
	{
		for (i = 0; i < tamanho; i++)
		{
			if (fechamento[i][k])
				for (j = 0; j < tamanho; j++)
					fechamento[i][j] = fechamento[i][j] || fechamento[k][j];
		}
	}

	std::cout << "Depois:" << std::endl;
	for (int i = 0; i < tamanho; i++) {
		for (int j = 0; j < tamanho; j++) {
			std::cout << fechamento[i][j] << " ";
		}
		std::cout << std::endl;
	}
	
	return fechamento;
}

int GrafoM::melhorCaminho(int s, int t)
{	
	bool MEMBRO = true;
	bool NAOMEMBRO = false;
	
	int *distancia = new int[tamanho];
	int *caminho = new int[tamanho];
	bool *perm = new bool[tamanho];
	int corrente, k = s, dc, j = 0;
	int menordist, novadist;
	//inicialização
	for (int i = 0; i < tamanho; ++i) {
		perm[i] = NAOMEMBRO;
		distancia[i] = INFINITO;
		caminho[i] = -1;
	}
	perm[s] = MEMBRO;
	distancia[s] = 0;
	corrente = s;
	while (corrente != t) {
		menordist = INFINITO;
		dc = distancia[corrente];
		for (int i = 0; i < tamanho; i++) {
			if (!perm[i]) {
				novadist = dc + matriz[corrente][i];
				if (novadist < distancia[i]) {
					
					distancia[i] = novadist;
					caminho[i] = corrente;
				}
				if (distancia[i] < menordist) {
					menordist = distancia[i];
					k = i;
				}
			}
		}
		corrente = k;
		perm[corrente] = MEMBRO;
	}
	std::cout << "Melhor";
	imprimeCaminho(s, t, caminho);
	return distancia[t];

}

void GrafoM::imprimeCaminho(int s, int t, int *caminho) {
	int i = t;
	std::cout << t << "-";
	while (i != s) {
		std::cout << caminho[i] << "-";
		i = caminho[i];
	}
	std::cout << std::endl;

	return;
}

int GrafoM::qualVertice(std::string vertice) {
	int i = 0;
	while ((vertices[i].nome != vertice) && (i < tamanho)) {
		i++;
	
	}
		

	if (i == tamanho) 
		return -1;
	else
		return i;
}

int GrafoM::nArestas() {
	int nArestas = 0;
	for (int i = 0; i < tamanho; i++) {
		for (int j = 0; j < tamanho; j++) {
			if (matriz[i][j] != INFINITO)
				nArestas++;
		}
	}
	return nArestas;
}

std::vector<std::pair<std::string, int>> GrafoM::maiorGrauSaida() {
	std::vector<std::pair<std::string, int>> maiorGrau;
	std::pair<std::string, int> p;
	p.second = 0;
	for (int i = 0; i < 20; i++) {
		maiorGrau.push_back(p);
	}

	for (int indice = 0; indice < tamanho; indice++) {
		int grauAtual = 0;
		for (int adjacente = 0; adjacente < tamanho; adjacente++) {
			if (matriz[indice][adjacente] != INFINITO) {
				grauAtual++;
			}
		}
		for (int posicao = 0; posicao < 20; posicao++) {
			if (grauAtual > maiorGrau[posicao].second) {
				std::string verticeAtual = vertices[indice].nome;
				for (int troca = posicao; troca < 20; troca++) {
					std::string verticeAux = maiorGrau[troca].first;
					int grauAux = maiorGrau[troca].second;

					maiorGrau[troca].first = verticeAtual;
					maiorGrau[troca].second = grauAtual;

					verticeAtual = verticeAux;
					grauAtual = grauAux;
				}
			}
		}
	}
	std::cout << "Os 20 usuarios com mais saida:" << std::endl;
	std::cout << "Grau | Vertice" << std::endl;
	for (int i = 0; i < 20; i++) {
		std::cout << maiorGrau[i].second << "\t" << maiorGrau[i].first << std::endl;
	}
	return maiorGrau;
}

std::vector<std::pair<std::string, int>> GrafoM::maiorGrauEntrada() {
	std::vector<std::pair<std::string, int>> maiorGrau;
	std::pair<std::string, int> p;
	p.second = 0;
	for (int i = 0; i < 20; i++) {
		maiorGrau.push_back(p);
	}

	for (int adjacente = 0; adjacente < tamanho; adjacente++) {
		int grauAtual = 0;
		for (int indice = 0; indice < tamanho; indice++) {
			if (matriz[indice][adjacente] != INFINITO) {
				grauAtual++;
			}
		}
		for (int posicao = 0; posicao < 20; posicao++) {
			if (grauAtual > maiorGrau[posicao].second) {
				std::string verticeAtual = vertices[adjacente].nome;
				for (int troca = posicao; troca < 20; troca++) {
					std::string verticeAux = maiorGrau[troca].first;
					int grauAux = maiorGrau[troca].second;

					maiorGrau[troca].first = verticeAtual;
					maiorGrau[troca].second = grauAtual;

					verticeAtual = verticeAux;
					grauAtual = grauAux;
				}
			}
		}
	}
	std::cout << "Os 20 usuarios com maior entrada:" << std::endl;
	std::cout << "Grau | Vertice" << std::endl;
	for (int i = 0; i < 20; i++) {
		std::cout << "\t\t" << maiorGrau[i].second << "\t" << maiorGrau[i].first << std::endl;
	}
	return maiorGrau;
}

std::list<int> *GrafoM::*verticesNaDistancia(int distancia, int vertice) {
	
	//system("pause");

	std::list<int> *lista = new std::list<int>();

	verticesNaDistancia_Recursivo(vertice, 1, distancia, lista, -1, vertice);

	return lista;
}

void GrafoM::verticesNaDistancia_Recursivo(int vertice, int nivel, int distancia, std::list<int> *lista, int verticeAnterior, int de) {
	if (nivel == distancia) {
		for (int i = 0; i < tamanho; i++) {
			if (i != de) {
				if (i != verticeAnterior) {
					if (matriz[vertice][i] != INFINITO) {
						bool repetido = false;
						for (std::list<int>::iterator it = lista->begin(); it != lista->end(); it++) {
							if (*it == i) {
								repetido = true;
							}
						}
						if (repetido == false) {
							lista->push_back(i);
						}
					}
				}
			}
		}
		return;
	}
	for (int i = 0; i < tamanho; i++) {
		if (i != de) {
			if (i != verticeAnterior) {
				if (matriz[vertice][i] != INFINITO) {
					verticeAnterior = vertice;
					verticesNaDistancia_Recursivo(i, nivel + 1, distancia, lista, verticeAnterior, de);
				}
			}
		}
	}
}