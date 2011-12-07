
#include "interface.hpp"
#include "StackFrame.hpp"
#include "parser.hpp"

int main()
{
    StackFrame testedocaralho;
    testedocaralho.data.push_back(Entry(ENTRY_PARAMETER,1,"short Eita"));
    testedocaralho.data.push_back(Entry(ENTRY_PARAMETER,4,"int Eita2"));
    testedocaralho.data.push_back(Entry(ENTRY_PARAMETER,1,"short Eita3"));

    testedocaralho.data.push_back(Entry(ENTRY_VARIABLES,1,"short var1"));
    testedocaralho.data.push_back(Entry(ENTRY_VARIABLES,4,"int var3"));
    testedocaralho.data.push_back(Entry(ENTRY_VARIABLES,1,"short var2"));
    Handler a;
    a.m_ReturnAddress = 10;
    a.m_CallersBasePtr = 20;
    a.m_Locals.push_back(Entry(ENTRY_PARAMETER,1,"short EitaException"));
    a.m_Locals.push_back(Entry(ENTRY_PARAMETER,1,"short Eita2Exception"));
    testedocaralho.dataE.push_back(ExceptionEntry("exception2",56400,a));
    printTable(testedocaralho);
    getchar();

/*int main()
{
    parse::test_parse();*/
    return 0;
}
