<<<<<<< HEAD
#include "interface.hpp"
#include "StackFrame.hpp"

int main()
{
    StackFrame testedocaralho;
    testedocaralho.data.push_back(Entry(ENTRY_PARAMETER,1,"short Eita"));
    testedocaralho.data.push_back(Entry(ENTRY_PARAMETER,4,"int Eita2"));

    testedocaralho.data.push_back(Entry(ENTRY_VARIABLES,1,"short var1"));
    testedocaralho.data.push_back(Entry(ENTRY_VARIABLES,4,"int var2"));
    printTable(testedocaralho);
    getchar();
=======
#include "parser.hpp"

int main()
{
    parse::test_parse();
>>>>>>> a7b072536f8248aa56ef2512de7a7c44973878ad
    return 0;
}
