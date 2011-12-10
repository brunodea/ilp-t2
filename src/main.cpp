
#include "interface.hpp"
#include "StackFrame.hpp"
#include "parser.hpp"

int main()
{
    parse::test_parse();
    /*
    StackFrame stackframe1;
    stackframe1.data.push_back(Entry(ENTRY_PARAMETER,1,"short var1"));
    stackframe1.data.push_back(Entry(ENTRY_PARAMETER,1,"short var2"));

    stackframe1.data.push_back(Entry(ENTRY_VARIABLES,1,"short var3"));
    stackframe1.data.push_back(Entry(ENTRY_VARIABLES,4,"int var4"));
    Handler a;
    a.m_ReturnAddress = 10;
    a.m_CallersBasePtr = 20;
    a.m_Locals.push_back(Entry(ENTRY_PARAMETER,1,"short h"));
    a.m_Locals.push_back(Entry(ENTRY_PARAMETER,4,"int j"));

    Handler b;
    b.m_ReturnAddress = 30;
    b.m_CallersBasePtr = 40;
    b.m_Locals.push_back(Entry(ENTRY_PARAMETER,4,"int x"));
    b.m_Locals.push_back(Entry(ENTRY_PARAMETER,4,"int y"));
    
    stackframe1.dataE.push_back(ExceptionEntry("exception1",41250,b));
    stackframe1.dataE.push_back(ExceptionEntry("exception2",56400,a));

    
    printTable(stackframe1);
    getchar();
    */

    return 0;
}
