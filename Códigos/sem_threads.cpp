// Importando bibliotecas
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>
#include <sys/stat.h>
#include <filesystem>
#include <regex>
#include <chrono>
#include <cstdio>

using namespace std;
namespace fs=filesystem;
namespace cr=chrono;

// Definindo id
int long long id = 1;

// Pega o caminho da pasta que será armazenado os arquivos de dicionário
fs::path caminhoR = std::filesystem::current_path();
string caminhoR_str = caminhoR.string();

fs::path caminhoDoc = "\\Dics\\";
string caminhoDoc_str = caminhoDoc.string();

// Variável que guarda o caminho que será armazenado os arquivos de dicionário
string caminhoD = caminhoR_str+caminhoDoc_str;

// função que verifica se o elemento já está no dicionário
bool verificaElemento(string elemento, string caminho){
	fstream documento;
	documento.open(caminho);

	string linha;

	regex elem(elemento);

	while(getline(documento, linha)){
		if (regex_search(linha, elem)){
			return true;
		}
	}
	return false;
}

// função que verifica o número de colunas que tem o arquivo .csv
int numeroColunas(string nomeArquivo){
	fstream arquivo;
    arquivo.open(nomeArquivo);

    string linha;
	getline(arquivo, linha);

	stringstream linhadoc(linha);
    string elemento;
	int numColunas = 0;
	
	while (getline(linhadoc, elemento, ','))
    {
		numColunas++;
    }
    
    arquivo.close();
    return numColunas;
}

// Escreve o documento que armazenará o dicionário
void escreveDic(string elemento, int n){
	ofstream arquivo;
	string caminho = caminhoD+to_string(n)+"_dic.txt";

	if(!verificaElemento(elemento, caminho)){
		arquivo.open(caminho, ios::app);

		if(arquivo.is_open()){
			arquivo << id << "," << elemento << endl;
		}

		arquivo.close();

        id++;
	}
}

// Lê o arquivo do dicionário e o constroi segundo sua configuração: (k = 1, v = 0) = categorização; (k = 0, v = 1) = descategorização;
map<string, string> constroiDic(int n, int k, int v){
	map<string, string> dic;

	fstream arquivo;
	string caminho = caminhoD+to_string(n)+"_dic.txt";

	arquivo.open(caminho, ios::in);

	string linha;
	string elemento;

	if(arquivo.is_open()){
		while(getline(arquivo, linha)){
			stringstream l(linha);
			string elem_ln[2];
			int i = 0;
			while(getline(l, elemento, ',')){
				elem_ln[i]=elemento;
				i++;
			}
			dic.insert({elem_ln[k], elem_ln[v]});
    	}
	}

	arquivo.close();

	return dic;
}

// Busca o valor da chave
string buscaValor(string elemento, map<string, string> dict){
	auto ident = dict.find(elemento);
	return ident->second;
}

void mesclaArquivos(int n, string arquivoFinal){
    vector<ifstream> arquivoCol;

    ofstream arquivoCat;
    arquivoCat.open(arquivoFinal, ios::app);

    for (int i=1; i <= n; i++){
        arquivoCol.emplace_back(to_string(i)+"_col.txt");
    }

    string linhaCol;

    while(arquivoCol[0].peek() != EOF){
        string linhaArq;
        for (int i=0; i < n; i++){
            getline(arquivoCol[i], linhaCol);
            linhaArq += linhaCol;
        }
        arquivoCat << linhaArq << endl;
    }

	int j = 1;

    for (auto& arq : arquivoCol) {
        arq.close();

		string nomeArq = to_string(j)+"_col.txt";
		remove(nomeArq.c_str());
		j++;
    }
    arquivoCat.close();
}

//as colunas não estão sendo escritas, além da primeira
// Escreve o documento segundo os elementos no dicionário
void escreveDoc(string arquivoNomeOrg, string arquivoFinal, string config){
	map<string, string> dic;
	int k, v;

    // Defini a quantidade de colunas no arquivo
	int tamCol = numeroColunas(arquivoNomeOrg);

	if (config == "categorizar"){
		k = 1;
		v = 0;
	}else if (config == "descategorizar"){
		k = 0;
		v = 1;
	}

	for (int i=1; i <= tamCol; i++){
		dic = constroiDic(i, k, v);

		ofstream arquivoCat;
		string nomeArqCol = to_string(i)+"_col.txt";

		arquivoCat.open(nomeArqCol, ios::app);
			
		// Abri o arquivo para somente leitura
		fstream arquivoOrg;
		arquivoOrg.open(arquivoNomeOrg, ios::in);

		string linha;
		string elemento;		

		// Lê o arquivo original, categoriza os seus elementos e escreve no arquivo final
		while(getline(arquivoOrg, linha)){

			stringstream linhadoc(linha);
			int c = 1;

			while (getline(linhadoc, elemento, ','))
			{
				if (c == i){
					if (i == tamCol){
						arquivoCat << buscaValor(elemento, dic);
						//cout << "LOL: " << i << endl;
					}
					else{
						arquivoCat << buscaValor(elemento, dic) << ",";
					}
					break;
				}
				c++;
			}
			arquivoCat << endl;
		}
		arquivoCat.close();
		arquivoOrg.close();
		dic.clear();
	}

	mesclaArquivos(tamCol, arquivoFinal);

}

// Verifica se ambos os arquivos são iguais ou diferentes
bool comparaArquivos(string nomeArquivoOr, string arquivoNome){
	fstream arquivoOr;
	fstream arquivoDes;

	arquivoOr.open(nomeArquivoOr, ios::in);
	arquivoDes.open(arquivoNome, ios::in);

	string linhaOr;
	string linhaDes;

	if(arquivoOr.is_open()){
		if(arquivoDes.is_open()){
			while(arquivoOr.peek() != EOF){
				getline(arquivoOr, linhaOr);
				getline(arquivoDes, linhaDes);

				regex ln(linhaOr);

				if (!regex_search(linhaDes, ln)){
					arquivoOr.close();
					arquivoDes.close();
					return false;
				}
			}
		}
	}

	arquivoOr.close();
	arquivoDes.close();

	return true;
}


int main ()
{
	int op1;
	// Menu de escolhas
	std::cout << "Escolha uma das opções abaixo:" << endl << endl;
    std::cout << "1 - Categorizar." << endl;
    std::cout << "2 - Descategorizar." << endl;
	std::cout << "0 - Sair." << endl;
    std::cout << "Digite a sua escolha: ";
	std::cin >> op1;

	switch (op1)
	{
	// Faz a categorização
	case 1:
	{
		//string nomeArquivoOrig;
		//std::cout << "Digite o nome do arquivo que quer categorizar: ";
		//cin >> nomeArquivoOrig;

		auto inicio = cr::high_resolution_clock::now();

		// Cria uma pasta para conter os dicionários
		int checaDir;
		checaDir = mkdir("Dics");

		// Checa se a pasta foi feita com êxito
		if (checaDir)
			std::cout << "Erro ao criar o diretório!" << endl;

		// Nome do arquivo ao ser lido
		//string nomeArquivoOrig = "test_date.csv";
		string nomeArquivoOrig = "dataset_00_1000_sem_virg.csv";

		// Defini a quantidade de colunas no arquivo

		fstream arquivo_orig_dics;
		// Abri o arquivo para somente leitura
		arquivo_orig_dics.open(nomeArquivoOrig, ios::in);

		string linha_1;

		// Lê o arquivo e escreve o dicionário de cada respectiva coluna
		while (getline(arquivo_orig_dics, linha_1))
		{
			stringstream linhadoc_1(linha_1);
			string elemento_1;
			int d = 1;

			while (getline(linhadoc_1, elemento_1, ','))
			{
				escreveDic(elemento_1, d);
				d++;
			}
		}

		// Fecha arquivo
		arquivo_orig_dics.close();

		string arquivoNome = "categorizado.csv";
		escreveDoc(nomeArquivoOrig, arquivoNome, "categorizar");
		
		auto fim = cr::high_resolution_clock::now();

		cr::duration<double> tempo = fim - inicio;
		double tempo_s = tempo.count();

		std::cout << "Sucesso, arquivo categorizado!" << endl;
		std::cout << "Tempo de execução: " << tempo_s << endl;


	break;
	}
	// Faz a descategorização
	case 2:
	{
		string op2;

		std::cout << "Escolha uma das opções abaixo:" << endl << endl;
		std::cout << "Você tem o arquivo original não categorizado e gostaria de compará-los?(Apenas letras minúsculas) <y/n>: ";
		std::cin >> op2;

		bool arq = false;

		string arquivoNomeCat = "categorizado.csv";

		string nomeArquivoOr;
		if (op2 == "y"){
			// caso tenha o arquivo original, faz a verificação se os dois batem
			arq = true;

			//std::cout << "Digite o nome do arquivo original não categorizado: ";
			//cin >> nomeArquivoOr;

			nomeArquivoOr = "test_date.csv";
			//nomeArquivoOr = "dataset_00_1000_sem_virg.csv";
		}
		else if (op2 == "n"){
			arq = false;
		}
		
		//string nomeArquivoCat;
		//std::cout << "Digite o nome do arquivo que quer descategorizar: ";
		//cin >> nomeArquivoCat;

		auto inicio = cr::high_resolution_clock::now();

		string arquivoNomeDesc = "descategorizado.csv";
		
		escreveDoc(arquivoNomeCat, arquivoNomeDesc, "descategorizar");
	
		// Compara se o arquivo original é igual ao arquivo descategorizado
		if (arq == true){
			if (comparaArquivos(nomeArquivoOr, arquivoNomeDesc)){
				cout << "Ambos são idênticos!" << endl;
			}
			else{
				cout << "Ambos são diferentes!" << endl;
			}
		}
		

		auto fim = cr::high_resolution_clock::now();

		cr::duration<double> tempo = fim - inicio;
		double tempo_s = tempo.count();

		std::cout << "Sucesso, arquivo descategorizado!" << endl;
		std::cout << "Tempo de execução: " << tempo_s << endl;

		break;
	}

	case 0:
	{
		std::cout << "Saindo..." << endl;
		break;
	}
	
	default:
	{
		std::cout << "Erro, opção inválida!";
		break;
	}

	}

    return EXIT_SUCCESS;
}