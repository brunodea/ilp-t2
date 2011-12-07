#pragma once

#include <vector>
#include <string>
#include <utility>
#include <tuple>

enum EntryType  {
    ENTRY_PARAMETER, ENTRY_STATIC_LINK, ENTRY_RETURN_ADRESS, ENTRY_DYNAMIC_LINK,
    ENTRY_SAVED_CONTEXT, ENTRY_TEMPORARY, ENTRY_VARIABLES
};


typedef std::tuple<EntryType, size_t, std::string> Entry;

struct StackFrame {
    //parametros, elo estatico, end. retorno, elo dinamico, contexto salvo,
    //temporarios, variaveis

    std::vector<Entry> data; 
};