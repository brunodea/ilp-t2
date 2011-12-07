#pragma once

#include <vector>
#include <string>
#include <utility>

struct StackFrame {
    //elo estatico
    //elo dinamico
    //area de dados locais
    //tabela de tratadores
    //retorno
    //area de temporarios
    //area de salvamento de contexto

    typedef std::pair<size_t, std::string>  EntryType;
    
    std::vector<EntryType> data; 
};