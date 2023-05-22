#include <iostream> 
#include <thread> 
#include <mutex> 

#define QUANT_PROD 1000 //quantidade a produzir
#define BUFFER_SIZE 200 //tamanho do buffer temporário

#define PROD_TIME  10   //tempo de producao
#define CONS1_TIME 100  //tempo de consumo 1
#define CONS2_TIME 100  //tempo de consumo 2
#define PROGR_TIME 1000 //tempo de checagem

std::mutex g_count_mutex;

int g_qtt, g_prod, g_cons1; 
int g_cons2, g_count; 
int g_buffer[BUFFER_SIZE];

void init()
{
    g_count = 0; g_prod = 0;
    g_cons1 = 0; g_cons2 = 0; g_qtt = 0;
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        g_buffer[i] = -1;
    }
}

void produtor() 
{
    std::cout << "  > PRODUTOR " << std::endl;

    while (true)
    {
        g_count_mutex.lock();
        if (g_count < BUFFER_SIZE)
        {
            g_buffer[g_count] = g_count * 2;
            g_prod += g_buffer[g_count];
            g_count++; g_qtt++;
        }
        g_count_mutex.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(PROD_TIME));

        if (g_qtt == QUANT_PROD) break;
    }
}

void consumidor1()
{
    std::cout << "  > CONSUMIDOR 1" << std::endl;

    while (true)
    {
        
        g_count_mutex.lock();
        if (g_count > 0)
        {
            g_cons1 += g_buffer[g_count-1];
            g_buffer[g_count-1] = -1;
            g_count--;
        }
        g_count_mutex.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(CONS1_TIME));

        if ((g_qtt == QUANT_PROD) && ( g_count == 0)) break;
    }
}

void consumidor2()
{
    std::cout << "  > CONSUMIDOR 2" << std::endl;

    while (true)
    {

        g_count_mutex.lock();
        if (g_count > 0)
        {
            g_cons2 += g_buffer[g_count-1];
            g_buffer[g_count-1] = -1;
            g_count--;
        }
        g_count_mutex.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(CONS2_TIME));

        if ((g_qtt == QUANT_PROD) && ( g_count == 0)) break;
    }
}


void progress()
{
    while (true)
    {
        if (g_qtt <= QUANT_PROD)
        {
            std::cout << "  --- qtde produzida: " << g_qtt  << " - " << ((float)g_qtt / (float)QUANT_PROD)*100  << "%" << std::endl;
            std::cout << "  --- ocupacao buffer: " << g_count << " - " << ((float)g_count / (float)BUFFER_SIZE)*100 << "%" << std::endl;
            std::cout << "  .... total produzido: " << g_prod << std::endl;
        } 
        std::cout << "  .... total consumido: " << g_cons1+g_cons2 << std::endl; 
        std::cout << "  +++++++++++++++++++++++++++++++++++ " << std::endl;      

        std::this_thread::sleep_for(std::chrono::milliseconds(PROGR_TIME));
        if ((g_qtt == QUANT_PROD) && ( g_count == 0)) break;
    }
}

int main() 
{
    auto start = std::chrono::steady_clock::now();

    std::cout << "=======================================================" << std::endl;
    std::cout << "SIMULANDO RELACAO PRODUTOR/CONSUMIDOR - BUFFER LIMITADO" << std::endl;
    std::cout << "=======================================================" << std::endl << std::endl;

    std::cout << "Preparando Buffer..." << std::endl; 
    init();

    std::cout << "Qtde a produzir  : " << QUANT_PROD << std::endl;
    std::cout << "Tamanho do buffer: " << BUFFER_SIZE << std::endl;

    std::cout << "Iniciando Threads..." << std::endl;

    std::thread th_produtor(produtor);       
    std::thread th_consumidor1(consumidor1);  
    std::thread th_consumidor2(consumidor2);
    std::thread th_progress(progress);  

    th_produtor.join();
    th_consumidor1.join();
    th_consumidor2.join();
    th_progress.join();

    auto end = std::chrono::steady_clock::now();

    std::cout << "Finalizado!" << std::endl;
    std::cout << "Produzido   : " << g_prod << std::endl;
    std::cout << "Consumidor 1: " << g_cons1 << std::endl;
    std::cout << "Consumidor 2: " << g_cons2 << std::endl;
    std::cout << "Tempo       : " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << "s" << std::endl;

    return 0;
}