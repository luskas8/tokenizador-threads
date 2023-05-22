#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <typeinfo>

using namespace std;

vector<pair<int,string>> ordenaDict(map<int,string> id_dict){
    vector<pair<int,string>> id_vec_pair;

    copy(id_dict.begin(), id_dict.end(), back_inserter<vector<pair<int,string>>>(id_vec_pair));

    sort(id_vec_pair.begin(), id_vec_pair.end(), [](const pair<int,string> &l, const pair<int,string> &r){
        if (l.first != r.first){
            return l.first < r.first;
        }

        return l.second < r.second;
    });

    return id_vec_pair;
}

bool verificaElemento(string elemento, map<int,string> id_dict){
	for (auto const &i : id_dict){
		if (i.second == elemento){
			return true;
		}
	}
	return false;
}

int main ()
{
    map<int,string> id_dict;
    int id = 1;

    ifstream arquivo("test_date.csv");
    string linha;

    while (getline(arquivo, linha))
    {
        stringstream linhadoc(linha);
        string elemento;
        while (getline(linhadoc, elemento, ','))
        {
			if(verificaElemento(elemento, id_dict)){
				continue;
			}
            else{
				id_dict.insert({id, elemento});
            	id++;
			}
        }
    }

	arquivo.close();
    
    vector<pair<int,string>> id_vector_dict = ordenaDict(id_dict);

	/*
    for (auto const &pair: id_vector_dict){
        cout << pair.first << " : " << pair.second << endl;
    }
	*/

	//Cria arquivo que está o dicionário de ids
	ofstream arquivo_dic;

    arquivo_dic.open("dict.txt");

    for (auto const &i: id_vector_dict){
        arquivo_dic << i.first << "," << i.second << endl;
    }

	arquivo_dic.close();


	//Lê o arquivo do dicionáiro e o constroi em um novo
	ifstream arquivo_di("dict.txt", ios::in);
    string ln;
	string elem;
	map <int, string> new_id_dict;

    while(getline(arquivo_di, ln)){
		//cout << ln << endl;
		stringstream l(ln);

		vector <string> elem_ln;

		while(getline(l, elem, ',')){
			elem_ln.push_back(elem);
		}

		new_id_dict.insert({stol(elem_ln[0]), elem_ln[1]});
    }

	arquivo_di.close();


	vector<pair<int,string>> new_id_vector_dict = ordenaDict(new_id_dict);

    for (auto &i : new_id_vector_dict){
        cout << i.first << " : " << i.second << endl;
    }


	//escrevendo em um arquivo a tabela só com os ids

	//arquivo para ler 
	ifstream arq_original("test_date.csv", ios::in);

	//arquivo para escrever
	ofstream arq_id;
	arq_id.open("id_tabela.txt");

    string ln1;
	string elem1;

    while(getline(arq_original, ln1)){
		//cout << ln << endl;
		stringstream l(ln1);
		vector <string> elem_ln;

		while(getline(l, elem1, ',')){
			for (auto const &i: id_vector_dict){
				if (elem1 == i.second){
					arq_id << i.first << ",";
				}
    		}
		}
		arq_id << endl;
    }

    
	arq_id.close();
	arq_original.close();


    return EXIT_SUCCESS;
}