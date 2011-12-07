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

typedef struct ExceptionHandler
{
	std::vector<Entry> m_Locals;
	int m_CallersBasePtr;
	int m_ReturnAddress;
} Handler;

typedef struct ExceptionEntry
{
	std::string m_LabelID;
	int m_HandlerBasePtr;
	
	Handler m_Handler;

    ExceptionEntry(std::string i, int h)
    {
        m_LabelID = i;
        m_HandlerBasePtr = h;
    }

    ExceptionEntry(std::string i, int h, Handler hl)
    {
        m_LabelID = i;
        m_HandlerBasePtr = h;
        m_Handler = hl;
    }

} ExceptionEntry;

struct StackFrame {
    //parametros, elo estatico, end. retorno, elo dinamico, contexto salvo,
    //temporarios, variaveis
    std::vector<ExceptionEntry> dataE;

    std::vector<Entry> data; 
};