#pragma once

#include "StackFrame.hpp"
#include <cstdio>
#include <iostream>
#include <string>
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
        << " +---------------------+-----+";
}