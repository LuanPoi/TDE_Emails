#include <iostream>
#include "Navegador.h"
#include <experimental/filesystem>
#include <fstream>
#include <stdlib.h>
#include <vector>

using namespace std;
namespace fs = std::experimental::filesystem;

int main() {
	cout << "--------------------------#1----------------------------------" << endl;

	Navegador *navegador = new Navegador("C:\\Users\\Andromeda\\TDE_Emails\\TDE_Emails\\Enron\\"); //Colocar aqui o endereço da pasta Enron.

	cout << "--------------------------#2----------------------------------" << endl;

	std::cout << "Numero de vertices: " << navegador->nVertices() << std::endl;
	
	std::cout << "Numero de arestas: " << navegador->nArestas() << std::endl;

	navegador->maiorGrauSaida();
	
	navegador->maiorGrauEntrada();

	cout << "--------------------------#3----------------------------------" << endl;
	
	navegador->buscaProfundidade(0, 11);

	cout << "--------------------------#4----------------------------------" << endl;

	navegador->buscaLargura(0, 11);

	cout << "--------------------------#5----------------------------------" << endl;
	
	navegador->verticeNaDistancia(1, "matthew.fleming@enron.com");

	return 0;
}