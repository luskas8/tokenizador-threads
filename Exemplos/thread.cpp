#include <iostream> 
#include <thread>  
 
void f_th1() 
{
    std::cout << "sou a thread 1!" << std::endl;
}

void f_th2(int _par1)
{
    std::cout << "sou a thread 2 com o parametro: " << _par1 << "!" << std::endl;
}

int main() 
{
    std::cout << "criando e executando as threads..." << std::endl;

    std::thread th1(f_th1);      
    std::thread th2(f_th2, 2021);  

    std::cout << "sou a main!" << std::endl;

    th1.join();
    th2.join();

    std::cout << "saindo..." << std::endl;

    return 0;
}