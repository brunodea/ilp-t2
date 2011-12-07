
#include "interface.hpp"
#include "StackFrame.hpp"
#include "parser.hpp"

int main()
{
    StackFrame testedocaralho;
    testedocaralho.data.push_back(Entry(ENTRY_PARAMETER,1,"short Eita"));
    testedocaralho.data.push_back(Entry(ENTRY_PARAMETER,4,"int Eita2"));

    testedocaralho.data.push_back(Entry(ENTRY_VARIABLES,1,"short var1"));
    testedocaralho.data.push_back(Entry(ENTRY_VARIABLES,4,"int var2"));
    testedocaralho.dataE.push_back(ExceptionEntry("exception1",32));
    testedocaralho.dataE.push_back(ExceptionEntry("exception2",56400));
    printTable(testedocaralho);
    getchar();

/*int main()
{
    parse::test_parse();*/
    return 0;
}
