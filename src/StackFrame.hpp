#pragma once

#include <vector>
#include <string>
#include <utility>

struct StackFrame {
    //parametros, elo estatico, end. retorno, elo dinamico, contexto salvo,
    //temporarios, variaveis

    typedef std::pair<size_t, std::string>  EntryType;
    
    std::vector<EntryType> data; 
};