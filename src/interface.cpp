#include "interface.hpp"

/*
+-----------------+-----+ parameters
0000h | short eggs      |0002h|
0002h | --- padding --- |0002h|
0004h | int spam        |0004h|
0008h | int arg         |0004h|
+-----------------+-----+
000Ch | elo estatico    |0004h|
0010h | end. retorno    |0004h|
0014h | elo dinamico    |0004h|
0024h | contexto salvo  |0010h|
0034h | temporarios     |0010h|
+-----------------+-----+ variaveis
0044h | int var1        |0004h|
0048h | int* ptr1       |0004h|
004Ch +-----------------+-----+
*/

std::string printHexa(int n)
{
    std::string s = "";
    char c[6]; //como tem 4 numeros + h + \0
    if(n < 16)
    {
        sprintf(c,"000%xh",n);
        s.append(c);
    }else if(n < 256){
        sprintf(c,"00%xh",n);
        s.append(c);
    }else if(n < 4096){
        sprintf(c,"0%xh",n);
        s.append(c);
    }else{
        sprintf(c,"%xh",n);
        s.append(c);
    }
    return s;
}

std::string printNome(std::string s) // até 21 caracteres da pra imprimir
{
    if( s.size() < 22)
    {
        int n = 21 - (int)s.size();
        while(n > 0)
        {
            s.append(" ");
            n--;
        }
    }else{
        s = s.substr(0,18);
        s.append("...");
    }
    return s;
}

void imprimeExceptions(std::vector<ExceptionEntry> dE, int* e)
{
    for(int j = 0 ; j < (int)dE.size() ; j++)
    {
        std::cout << printHexa(*e) << " |"
            << printNome(dE[j].m_LabelID)
            << "|" << printHexa(4)
            << "|"<< std::endl;
        *e += 4;
        std::cout << printHexa(*e) << " |"
            << printNome(printHexa(dE[j].m_HandlerBasePtr))
            << "|" << printHexa(4)
            << "|"<< std::endl;
        *e += 4;
    }
}

void printTable(StackFrame sFrame) 
{
    int enderecoAtual = 0;
    std::cout <<"      +---------------------+-----+ parametros" << std::endl;
    for(int i = 0 ; i < (int)sFrame.data.size() ; i ++)
    {
        if(std::get<0> (sFrame.data[i]) == ENTRY_PARAMETER)
        {
            int sizeDoItem = (int)std::get<1> (sFrame.data[i]);
            if(enderecoAtual % sizeDoItem != 0)
            {
                int iPadding = 4 - enderecoAtual % sizeDoItem;
                std::cout << printHexa(enderecoAtual)
                    <<" |   --- padding ---   |" 
                    << printHexa(iPadding)
                    << "|" << std::endl;
                enderecoAtual += iPadding;
            }
            std::cout << printHexa(enderecoAtual) << " |"
                << printNome(std::get<2> (sFrame.data[i]))
                << "|" << printHexa(sizeDoItem)
                << "|"<< std::endl;
            enderecoAtual += sizeDoItem;
        }
    }
    std::cout <<"      +---------------------+-----+ excecoes" << std::endl;//IMPRIMIR AS EXCEÇÕES

    imprimeExceptions(sFrame.dataE, &enderecoAtual);

    std::cout <<"      +---------------------+-----+" << std::endl;
    std::cout << printHexa(enderecoAtual)
        << " |elo estatico         |"
        << printHexa(4)
        << "|" << std::endl;
    enderecoAtual += 4;


    std::cout << printHexa(enderecoAtual) 
        << " |end. retorno         |"
        << printHexa(4)
        << "|" << std::endl;
    enderecoAtual += 4;

    std::cout << printHexa(enderecoAtual)
        << " |elo dinamico         |"
        << printHexa(4)
        << "|" << std::endl;
    enderecoAtual += 4;

    std::cout << printHexa(enderecoAtual)
        << " |contexto salvo       |"
        << printHexa(10)
        << "|" << std::endl;
    enderecoAtual += 10;

    std::cout << printHexa(enderecoAtual)
        << " |temporarios          |"
        << printHexa(10)
        << "|" << std::endl;
    enderecoAtual += 10;

    std::cout <<"      +---------------------+-----+ variaveis" << std::endl;
    for(int i = 0 ; i < (int)sFrame.data.size() ; i ++)
    {
        if(std::get<0> (sFrame.data[i]) == ENTRY_VARIABLES)
        {
            int sizeDoItem = (int)std::get<1> (sFrame.data[i]);
            if(enderecoAtual % sizeDoItem != 0)
            {
                int iPadding = 4 - enderecoAtual % sizeDoItem;
                std::cout << printHexa(enderecoAtual)
                    <<" |   --- padding ---   |" 
                    << printHexa(iPadding)
                    << "|" << std::endl;
                enderecoAtual += iPadding;
            }
            std::cout << printHexa(enderecoAtual) << " |"
                << printNome(std::get<2> (sFrame.data[i]))
                << "|" << printHexa(sizeDoItem)
                << "|"<< std::endl;
            enderecoAtual += sizeDoItem;
        }
    }
    std::cout << printHexa(enderecoAtual) 
        << " +---------------------+-----+ estrutura do handler da excecao" << std::endl;

    for(int i = 0; i < (int)sFrame.dataE.size() ; i++)
    {
        Handler hl = sFrame.dataE[i].m_Handler;
        std::cout << "      +" << printNome(sFrame.dataE[i].m_LabelID)<<"+-----+"<< std::endl;
        if(enderecoAtual % 4 != 0)
        {
            int iPadding = 4 - enderecoAtual % 4;
            std::cout << printHexa(enderecoAtual)
                <<" |   --- padding ---   |" 
                << printHexa(iPadding)
                << "|" << std::endl;
            enderecoAtual += iPadding;
        }
        std::cout << "      +return address-------+-----+"<< std::endl;
        std::cout << printHexa(enderecoAtual) << " |"
            << printNome(printNome(printHexa(sFrame.dataE[i].m_Handler.m_ReturnAddress)))
            << "|" << printHexa(4)
            << "|"<< std::endl;
        enderecoAtual += 4;
        std::cout << "      +Caller's EBP---------+-----+"<< std::endl;
        std::cout << printHexa(enderecoAtual) << " |"
            << printNome(printNome(printHexa(sFrame.dataE[i].m_Handler.m_CallersBasePtr)))
            << "|" << printHexa(4)
            << "|"<< std::endl;
        enderecoAtual += 4;
        std::cout << "      +LocalVariableHandler-+-----+"<< std::endl;
        for(int j = 0 ; j < (int)sFrame.dataE[i].m_Handler.m_Locals.size() ; j ++)
        {
            int sizeDoItem = (int)std::get<1> (sFrame.dataE[i].m_Handler.m_Locals[j]);
            if(enderecoAtual % sizeDoItem != 0)
            {
                int iPadding = 4 - enderecoAtual % sizeDoItem;
                std::cout << printHexa(enderecoAtual)
                    <<" |   --- padding ---   |" 
                    << printHexa(iPadding)
                    << "|" << std::endl;
                enderecoAtual += iPadding;
            }
            std::cout << printHexa(enderecoAtual) << " |"
                << printNome(std::get<2> (sFrame.dataE[i].m_Handler.m_Locals[j]))
                << "|" << printHexa(sizeDoItem)
                << "|"<< std::endl;
            enderecoAtual += sizeDoItem;
        }



    }
    std::cout << printHexa(enderecoAtual) 
        << " +---------------------+-----+" << std::endl;
}