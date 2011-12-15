#include "interface.hpp"
#include <boost/variant.hpp>

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
    if (n < 0 || n > 0xFFFF)
        return "    h";

    char c[6]; //como tem 4 numeros + h + \0
    sprintf(c,"%04Xh", n);
    return std::string(c);
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

int calculaPadding(int enderecoAtual, size_t alignment)
{
    int sobra = enderecoAtual % alignment;
    if (sobra != 0) {
        int tamanho = alignment - sobra;
        std::cout << printHexa(enderecoAtual)
            <<" |   --- padding ---   |"
            << printHexa(tamanho)
            << "|\n";

        enderecoAtual += tamanho;
    }

    return enderecoAtual;
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
    std::cout << "\n(" << sFrame.name << ")" << std::endl;
    std::cout <<"      +---------------------+-----+ parametros" << std::endl;
    for(int i = 0 ; i < (int)sFrame.data.size() ; i ++)
    {
        if(sFrame.data[i].type == ENTRY_PARAMETER)
        {
            size_t sizeDoItem = sFrame.data[i].size;
            size_t alignDoItem = sFrame.data[i].alignment;

            enderecoAtual = calculaPadding(enderecoAtual, alignDoItem);

            std::cout << printHexa(enderecoAtual) << " |"
                << printNome(sFrame.data[i].name)
                << "|" << printHexa(sizeDoItem)
                << "|"<< std::endl;
            enderecoAtual += sizeDoItem;
        }
    }

    enderecoAtual = calculaPadding(enderecoAtual,4);
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
        if(sFrame.data[i].type == ENTRY_VARIABLES)
        {
            size_t sizeDoItem = sFrame.data[i].size;
            size_t alignDoItem = sFrame.data[i].alignment;

            enderecoAtual = calculaPadding(enderecoAtual, alignDoItem);

            std::cout << printHexa(enderecoAtual) << " |"
                << printNome(sFrame.data[i].name)
                << "|" << printHexa(sizeDoItem)
                << "|"<< std::endl;
            enderecoAtual += sizeDoItem;
        }
    }
    enderecoAtual = calculaPadding(enderecoAtual,4);

    std::cout << printHexa(enderecoAtual) 
        << " +---------------------+-----+ estrutura do handler da excecao" << std::endl;

    for(int i = 0; i < (int)sFrame.dataE.size() ; i++)
    {
        Handler hl = sFrame.dataE[i].m_Handler;
        std::cout << "      +" << printNome(sFrame.dataE[i].m_LabelID)<<"+-----+"<< std::endl;
        //std::cout << "      +return address-------+-----+"<< std::endl;
        std::cout << "      +---------------------+-----+"<< std::endl;
        std::cout << printHexa(enderecoAtual) << " |"
            //<< printNome(printHexa(sFrame.dataE[i].m_Handler.m_ReturnAddress))
            << printNome("return address")
            << "|" << printHexa(4)
            << "|"<< std::endl;
        enderecoAtual += 4;
        //std::cout << "      +Caller's EBP---------+-----+"<< std::endl;
        std::cout << "      +---------------------+-----+"<< std::endl;
        std::cout << printHexa(enderecoAtual) << " |"
            //<< printNome(printHexa(sFrame.dataE[i].m_Handler.m_CallersBasePtr))
            << printNome("Caller's EBP")
            << "|" << printHexa(4)
            << "|"<< std::endl;
        enderecoAtual += 4;
        //std::cout << "      +LocalVariableHandler-+-----+"<< std::endl;
        std::cout << "      +---------------------+-----+"<< std::endl;
        for(int j = 0 ; j < (int)sFrame.dataE[i].m_Handler.m_Locals.size() ; j ++)
        {
            size_t sizeDoItem = sFrame.dataE[i].m_Handler.m_Locals[j].size;
            size_t alignDoItem = sFrame.dataE[i].m_Handler.m_Locals[j].alignment;

            enderecoAtual = calculaPadding(enderecoAtual, alignDoItem);

            std::cout << printHexa(enderecoAtual) << " |"
                << printNome(sFrame.dataE[i].m_Handler.m_Locals[j].name)
                << "|" << printHexa(sizeDoItem)
                << "|"<< std::endl;
            enderecoAtual += sizeDoItem;
        }
        enderecoAtual = calculaPadding(enderecoAtual,4);
    }
    std::cout << printHexa(enderecoAtual) 
        << " +---------------------+-----+" << std::endl;
}

int calcTypeSize(parse::TypeEnum type)
{
    switch(type)
    {
        case parse::T_INT:       return 4;
        case parse::T_FLOAT:     return 4;
        case parse::T_CHAR:      return 1;
        case parse::T_BOOLEAN:   return 1;
        case parse::T_VOID:      return 0;
        case parse::T_EXCEPTION: return 4;
        default:          return  -1;
    }
}

size_t calcTypeSize(const parse::SType& stype)
{
    if (stype.pointer_indirections > 0)
        return 4;
    else
        return calcTypeSize(stype.type);
}

size_t calcTypeSize(const parse::Type& t)
{
    struct size_of_type : boost::static_visitor<size_t>
    {
        size_t operator()(const parse::TypeEnum& te) const
        {
            return calcTypeSize(te);
        }

        size_t operator()(const parse::Type& t) const
        {
            return calcTypeSize(t);
        }
    };

    size_t size = 1;

    for(auto i = std::begin(t.array_dimensions), end = std::end(t.array_dimensions); i != end; ++i) {
        size *= *i;
    }

    if(!t.is_pointer)
        size *= boost::apply_visitor(size_of_type(), t.type);
    else
        size *= 4;

    return size;
}

size_t calcTypeAlignment(const parse::SType& stype)
{
    return calcTypeSize(stype);
}

size_t calcTypeAlignment(const parse::Type& t)
{
    struct alignment_of_type : boost::static_visitor<size_t>
    {
        size_t operator()(const parse::TypeEnum& te) const
        {
            return calcTypeSize(te);
        }

        size_t operator()(const parse::Type& t) const
        {
            return calcTypeAlignment(t);
        }
    };

    if(!t.is_pointer)
        return boost::apply_visitor(alignment_of_type(), t.type);
    else
        return 4;
}

Entry paramToEntry(const parse::Param& param)
{
    return Entry(ENTRY_PARAMETER, calcTypeSize(param.type), calcTypeAlignment(param.type), param.name);
}

Entry varToEntry(const parse::VariableDecl& var)
{
    return Entry(ENTRY_VARIABLES, calcTypeSize(var.type), calcTypeAlignment(var.type), var.id);
}

std::vector<Entry> varDeclListToEntryList(const parse::DeclList& declarations)
{
    std::vector<Entry> entry_list;
    for(auto it = declarations.variables.begin(); it != declarations.variables.end(); ++it)
    {
        entry_list.push_back(varToEntry(*it));
    }

    return entry_list;
}

StackFrame procedureToStackFrame(const parse::ProcedureDecl& procedure)
{
    StackFrame sframe;
    sframe.name = procedure.id;
    for(auto param_it = procedure.param_list.begin(); param_it != procedure.param_list.end(); param_it++)
    {
        sframe.data.push_back(paramToEntry(*param_it));
    }
    std::vector<Entry> v = varDeclListToEntryList(procedure.declarations);
    sframe.data.insert(sframe.data.end(), v.begin(), v.end());
   
    return sframe;
}
void procedureDeclList(const parse::DeclList& declarations)
{
    for(auto it = declarations.procedures.begin(); it != declarations.procedures.end(); ++it)
    {
        printTable(procedureToStackFrame(*it));
        std::cout << std::endl;
    }
}

void printStackFrames(const parse::Program& program)
{
    std::cout << "Program " << program.id << std::endl;

    StackFrame main;
    main.name = "main";
    for(auto it = program.main_params.begin(); it != program.main_params.end(); ++it)
    {
        main.data.push_back(paramToEntry(*it));
    }
    std::vector<Entry> v = varDeclListToEntryList(program.main_decls);
    main.data.insert(main.data.end(), v.begin(), v.end());
    printTable(main);

    procedureDeclList(program.main_decls);
}
