#include "Navegador.h"
#include <fstream>
#include <iostream>
#include <string>
#include <experimental/filesystem>

using namespace std;
namespace fs = std::experimental::filesystem;


Navegador::Navegador(std::string endereco) : endereco(endereco)
{
	std::pair<std::string, std::string> parAux;
	std::string arquivo;

	//Buscando remetentes
	for (auto & p : fs::directory_iterator(endereco))								   //esse for roda para cada arquivo na pasta endereço, colocando o nome dele na variavel p
	{
		//busca nomes das pastas, parte 1 do pair
		parAux.first = p.path().string();											   //coloca o que esta em p(nome do arquivo), na string um do parAux
		
		(parAux.first).erase((parAux.first).begin(), (parAux.first).begin() + endereco.size());

		////busca nomes dos respectivos emails, parte 2 do pair
		//testa a existencia do diretorio sent\1
		arquivo = (endereco + parAux.first + "\\sent\\1");
		ifstream teste(arquivo);
		if (teste.good())
		{
			arquivo = (endereco + parAux.first + "\\sent\\1");							  //seta o sent\1
			ifstream email(arquivo);
			for (int i = 0; i < 3; i++)
				getline(email, parAux.second);											  //acha a linha do from e coloca na segunda string do parAux
			(parAux.second).erase((parAux.second).begin(), (parAux.second).begin() + 6);  //apaga o "from:"
		}
		else
		{
			//testa a existencia do diretorio _sent_mail\1
			arquivo = (endereco + parAux.first + "\\_sent_mail\\1");
			teste.open(arquivo);
			if (teste.good())
			{
				arquivo = (endereco + parAux.first + "\\_sent_mail\\1");							  //seta o _sent_mail
				ifstream email(arquivo);
				for (int i = 0; i < 3; i++)
					getline(email, parAux.second);											  //acha a linha do from e coloca na segunda string do parAux
				(parAux.second).erase((parAux.second).begin(), (parAux.second).begin() + 6);  //apaga o "from:"
			}
			else
			{
				//testa a existencia do diretorio sent_items\1
				arquivo = (endereco + parAux.first + "\\sent_items\\1");
				teste.open(arquivo);
				if (teste.good())
				{
					arquivo = (endereco + parAux.first + "\\sent_items\\1");							  //seta o sent_items
					ifstream email(arquivo);
					for (int i = 0; i < 3; i++)
						getline(email, parAux.second);											  //acha a linha do from e coloca na segunda string do parAux
					(parAux.second).erase((parAux.second).begin(), (parAux.second).begin() + 6);  //apaga o "from:"
				}
			}
		}
		remetentes.push_back(parAux);
	}

	//Buscar destinatarios que existem na base de dados
	for (int pessoa = 0; pessoa < remetentes.size(); pessoa++)
	{
		std::string nomePasta[] = { "\\sent\\","\\_sent_mail\\","\\sent_items\\" };
		for (int pasta = 0; pasta < 3; pasta++) {
			std::string enderecoUsuario = (endereco + remetentes[pessoa].first + nomePasta[pasta]);
			fstream teste(endereco + remetentes[pessoa].first + nomePasta[pasta] + "1");			
			if (!(teste.good())) {
				continue;
			}

			/*Busca numero de arquivos nas pastas do usuario*/
			int numArquivos = 0;
			std::string nomeArquivo;

			for (auto & p : fs::directory_iterator(enderecoUsuario)) {								   //esse for roda para cada arquivo na pasta endereço, colocando o nome dele na variavel p
				nomeArquivo = p.path().string();
				nomeArquivo.erase(0, enderecoUsuario.size());
				int numAtual = std::atoi(nomeArquivo.c_str());
				if (numArquivos < numAtual)
					numArquivos = numAtual;
			}
			
			/*Passa por cada arquivo de email para extrair o destinatrio*/
			for (int arquivo = 1; arquivo <= numArquivos; arquivo++)
			{				
				std::vector<std::string> login;	//armazena multiplos destinatarios
				int numDestinatarios = 0;		//contador de multiplos destinatarios
				
				/*Monta o endereco para cada email*/
				std::stringstream numArquivoString;
				numArquivoString << arquivo;
				std::string enderecoArquivo = (endereco + remetentes[pessoa].first + nomePasta[pasta] + numArquivoString.str());
				std::ifstream email(enderecoArquivo);
				std::string line, to = "";
				if (email.is_open()) {
					//Busca a linha que começa com "to:" e armazenar o que vier em seguida(destinatarios)
					for (int t = 0; t < 4; t++)
						getline(email, line);

					for (int c = 4; line[c] != '\0'; c++) {	
						if (line[c] == ' ') {
							if (line[c + 1] == '\0') {
								getline(email, line);
								c = 0;
							}
							continue;
						}
						if (line[c] == ',') {		//Se achar uma virgula, identifica multiplos destinatarios
							login.push_back(to);
							to = "";
							numDestinatarios++;
							continue;
						}						
						to += line[c];					
					}
					if (to != "") {			// Se achar um destinatario, armazena no vetor.
						login.push_back(to);
						numDestinatarios++;					
					}
					email.close();
				}
				
				for (int k = 0; k < numDestinatarios; k++) {
					int w = 0;
					for (w = 0; w < remetentes.size(); w++) {
						if (remetentes[w].second == login[k]) {			//Compara com os remetentes já armazenados pra n armazenar repetido
							break;
						}					
					}			
					int s = 0;					
					for (s = 0; s < destinatarios.size(); s++) {		//Compara com os destinatorios já armazenados pra n armazenar repetido
						if (destinatarios[s] == login[k]) {
							break;
						}
					}
					if (s == destinatarios.size() && w == remetentes.size()) { // Se nenhum dos anteriores foi repetido, armazena um novo destinatario
						destinatarios.push_back(login[k]);
					}			
				}
			}
		}
	}

	//cria o grafo com o tamanho do vetor de remetentes + o tamanho da lista de destinatario
	int tamanho_grafo = remetentes.size() + destinatarios.size();
	grafo = new GrafoM(tamanho_grafo);

	criarVertices();

	criarAdjacencias();
}

Navegador::~Navegador()
{
}

void Navegador::imprimeRemetentes()
{
	for (int i = 0; i < remetentes.size(); i++)
	{
		std::cout << remetentes[i].first << "|" << remetentes[i].second << std::endl;
	}
}

void Navegador::criarVertices()
{
	//criar os vertices a partir do vetor de remetentes e de destinatarios
	for (int i = 0; i < remetentes.size() ; i++)
	{
		grafo->setaInformacao(i, remetentes[i].second);
	}	
	for (int i = 0; i < destinatarios.size(); i++) {
		grafo->setaInformacao(i + remetentes.size(), destinatarios[i] );
	}
}

void Navegador::criarAdjacencias()
{	
	//Busca cada destinatario da base de dados para criar adjacencia

	//Buscar destinatarios que existem na base de dados
	for (int pessoa = 0; pessoa < remetentes.size(); pessoa++)
	{
		std::string nomePasta[] = { "\\sent\\","\\_sent_mail\\","\\sent_items\\" };
		for (int pasta = 0; pasta < 3; pasta++) {
			std::string enderecoUsuario = (endereco + remetentes[pessoa].first + nomePasta[pasta]);
			fstream teste(endereco + remetentes[pessoa].first + nomePasta[pasta] + "1");
			if (!(teste.good())) {
				continue;
			}

			/*Busca numero de arquivos nas pastas do usuario*/
			int numArquivos = 0;
			std::string nomeArquivo;

			for (auto & p : fs::directory_iterator(enderecoUsuario)) {								   //esse for roda para cada arquivo na pasta endereço, colocando o nome dele na variavel p
				nomeArquivo = p.path().string();
				nomeArquivo.erase(0, enderecoUsuario.size());
				int numAtual = std::atoi(nomeArquivo.c_str());
				if (numArquivos < numAtual)
					numArquivos = numAtual;
			}

			/*Passa por cada arquivo de email para extrair o destinatrio*/
			for (int arquivo = 1; arquivo <= numArquivos; arquivo++)
			{
				std::vector<std::string> login;	//armazena multiplos destinatarios
				int numDestinatarios = 0;		//contador de multiplos destinatarios

												/*Monta o endereco para cada email*/
				std::stringstream numArquivoString;
				numArquivoString << arquivo;
				std::string enderecoArquivo = (endereco + remetentes[pessoa].first + nomePasta[pasta] + numArquivoString.str());
				std::ifstream email(enderecoArquivo);
				std::string line, to = "";
				if (email.is_open()) {
					//Busca a linha que começa com "to:" e armazenar o que vier em seguida(destinatarios)
					for (int t = 0; t < 4; t++)
						getline(email, line);

					for (int c = 4; line[c] != '\0'; c++) {
						if (line[c] == ' ') {
							if (line[c + 1] == '\0') {
								getline(email, line);
								c = 0;
							}
							continue;
						}
						if (line[c] == ',') {		//Se achar uma virgula, identifica multiplos destinatarios
							login.push_back(to);
							to = "";
							numDestinatarios++;
							continue;
						}
						to += line[c];
					}
					if (to != "") {			// Se achar um destinatario, armazena no vetor.
						login.push_back(to);
						numDestinatarios++;
					}
					email.close();
				}

				for (int k = 0; k < numDestinatarios; k++) {
					int verticeValido = qualVertice(login[k]);	//Compara com os vertices existente no grafo para criar adjacencia

					if (verticeValido == -1) {
						std::cout << "Vertice invalido" << std::endl;
						exit(1);
					}
					else {
						grafo->criaAdjacencia(pessoa, verticeValido);
					}
				}
			}
		}
	}
}

int Navegador::qualVertice(std::string vertice) {
	return grafo->qualVertice(vertice);
}

int Navegador::nVertices() {
	return grafo->tamanho;
}

int Navegador::nArestas() {
	return grafo->nArestas();
}

void Navegador::maiorGrauEntrada() {
	
	grafo->maiorGrauEntrada();	
	return;
}

void Navegador::maiorGrauSaida() {
	grafo->maiorGrauSaida();	
	return;
}

int Navegador::buscaProfundidade(int de, int para)
{
	bool *visitado = new bool[grafo->tamanho];
	for (int i = 0; i < grafo->tamanho; i++)
	{
		visitado[i] = false;
	}
	int x =	buscaProfundidade_Recursivo(de, para, visitado);
	if (x == 1)
	{
		cout << de << " - Encontrado" << endl;
	}
	else
	{
		cout << "Caminho nao encontrado" << endl;
	}
	
	return x;
}

int Navegador::buscaProfundidade_Recursivo(int de, int para, bool *visitado)
{
	if (de == para)
	{
		return 1;
	}
	else
	{
		if (visitado[de] == false)
		{
			visitado[de] = true;
			for (int i = 0; i < grafo->tamanho; i++)
			{
				if (grafo->matriz[de][i] != INFINITO && visitado[i] == false)
				{
					int x = buscaProfundidade_Recursivo(i, para, visitado);
					if (x == 1)
					{
						std::cout << i << "-";
						return 1;
					}
				}
			}
		}
	}
	return 0;
}

int Navegador::buscaLargura(int de, int para)
{
	bool *visitado = new bool[grafo->tamanho];
	for (int i = 0; i < grafo->tamanho; i++)
	{
		visitado[i] = false;
	}
	list<int> fila;
	fila.push_back(de);
	int x = buscaLargura_Recursivo(para, &fila, visitado);
	if (x == 1)
	{
		cout << para << " - Encontrado" << endl;
	}
	else
	{
		cout << "Caminho nao encontrado" << endl;
	}
	return 1;
}

int Navegador::buscaLargura_Recursivo(int para, list<int> *fila, bool *visitado)
{
	if (fila->empty())
	{
		return 0;
	}
	else
	{
		int x = fila->front();
		fila->pop_front();
		visitado[x] = true;
		if (x == para)
		{
			return 1;
		}
		for (int i = 0; i < grafo->tamanho; i++)
		{
			if (visitado[i] == false && grafo->matriz[x][i] != INFINITO && !iterarFila(x, fila))
			{
				fila->push_front(i);
			}
		}
		x = buscaLargura_Recursivo(para, fila, visitado);
	}
	return 0;
}

bool Navegador::iterarFila(int valor, list<int> *fila)
{
	for (auto it = fila->begin(); it != fila->end(); it++)
	{
		if (*it == valor)
		{
			return true;
		}
	}
	return false;
}

bool Navegador::verticeNaDistancia(int distancia, std::string vertice) {
	
	int numVertice = grafo->qualVertice(vertice);
	if (numVertice == -1) {
		std::cout << "Vertice nao existe" << std::endl;
		return false;
	}

	std::list<int> *lista = grafo->verticesNaDistancia(distancia, numVertice);
	std::cout << " (" << vertice << ") vertices na distancia : " << distancia << std::endl;
	std::cout << "Indice | Vertice" << std::endl;
	for (std::list<int>::iterator it = lista->begin(); it != lista->end(); it++) {
		std::cout << *it << "\t" << (this->grafo)->vertices[*it].nome << std::endl;
	}
	std::cout << std::endl;
	return true;
}