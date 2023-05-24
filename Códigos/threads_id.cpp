#include <iostream>
#include <fstream>
#include <omp.h>

using namespace std;

int main(int argc, char *argv[]) {
    ifstream DATASET_CSV;
    // COLUNAS COM DADOS CARTEGORICAS: 2, 3, 4, 6, 7, 8, 9, 17, 18, 20, 23
    // cada var representa o nome de uma coluna
    // possível melhoria: trabalhar com vetores de ifstream para tornar dinamico
    // e verificar quais colunas são categóricas
    ofstream cdtup,	berco,	portoatracacao,	mes, tipooperacao,	tiponavegacaoatracacao,	terminal,
    nacionalidadearmador, origem, destino, naturezacarga, sentido;
;

    DATASET_CSV.open("dataset_00_1000_sem_virg.csv", ios::in);
    char delim = ',';

    //Inicialização das colunas
    cdtup.open("cdtup.csv");	
    berco.open("berco.csv");	
    portoatracacao.open("portoatracacao.csv");
    mes.open("mes.csv");
    tipooperacao.open("tipooperacao.csv");
    tiponavegacaoatracacao.open("tiponavegacaoatracacao.csv");
    terminal.open("terminal.csv");
    origem.open("origemcsv");
    destino.open("destino.csv");
    naturezacarga.open("naturezacargacsv");
    sentido.open("sentido.csv");




    // FUNC: Varrer o arquivo para criar um arquivo csv para cada coluna cartegórica
    string linha;
    int coluna = 0;
    int count = 0;
    // Não utilizei threads para a varredura e escrita por causa da seção crítica do arquivo do DATASET
    // porém pode haver uma melhoria

    /* melhorias para o trecho:
     * verificar quais colunas tem que ser varridas e utilizar o continue para nao ter verificação
     * Utilizar tratamento de seção crítica para a leitura do DataSet 
     * Parlelizar a escrita trocando o WHILE por um FOR e usando o Parallel for colapse(2)(as threads 
        serão divididas por colunas)
     * REQUISITO PARA NOVA VARREDURA: Existir um contador de linha x coluna para utilizar de parametro para o FOR

    */
    if (DATASET_CSV.is_open()){
        while(getline(DATASET_CSV, linha, delim)){
        coluna++;
        // fim da coluna
        if (coluna == 25){
            coluna = 0;
            count++;
            cout << count << endl;
            continue;
        }

        //if(coluna != 2, 3, 4, 6, 7, 8, 9, 17, 18, 20, 23)

        switch (coluna){
        case 2:
            cdtup << linha << endl;
            break;
        case 3:
            berco << linha << endl;
            break;
        case 4:
            portoatracacao << linha << endl;
            break;
        case 6:
            mes << linha << endl;
            break;
        case 7:
            tipooperacao << linha << endl;
            break;    
        case 8:                     
            tiponavegacaoatracacao << linha << endl;
            break;
        case 9:
            terminal << linha << endl;
            break;
        case 17:
            origem << linha << endl;
            break;
        case 18:
            destino << linha << endl;
            break;
        case 20:
            naturezacarga << linha << endl;
            break;
        case 21:
            sentido << linha << endl;
            break;
        default:
            break;
        }
        }

    } else {
        return -1;
    }
    DATASET_CSV.close();
    // FIM DA LEITURA E ESCRITA DE ARQUIVOS

    // FUNC 2: PARALELIZANDO A IDENTIFICAÇÃO DE ID E ESCRITA DE ARQUIVO

    
    int a = 0;
    return 0;
}