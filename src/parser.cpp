#include "parser.hpp"

#include <boost/spirit/include/qi.hpp>

#include <iostream>
#include <string>
#include <vector>

namespace parse {

namespace qi = boost::spirit::qi;

enum Types {
    T_INT,
    T_FLOAT,
    T_CHAR,
    T_BOOLEAN,
    T_VOID
};

struct TypesTable : qi::symbols<char, Types> {
    TypesTable()
    {
        add("int",     T_INT)
           ("float",   T_FLOAT)
           ("char",    T_CHAR)
           ("boolean", T_BOOLEAN)
           ("void",    T_VOID);
    }
} types_table;

struct Grammar {
    typedef std::string::iterator It;
    typedef qi::space_type Skip;

    Grammar()
    {
        id = qi::lexeme[qi::alpha >> *(qi::alnum)];
        num = qi::uint_;
        tipo_s = types_table | ('*' >> tipo_s);

        tipo = types_table | ('*' >> tipo) | ("array" >> seq) | "exception";
        seq = +('[' >> num >> ']') >> "of" >> tipo;

        lista_p = *(id >> ':' >> tipo_s >> ';');

        lista_d = -((id >> ':' >> tipo >> ';' >> lista_d) | ("unit" >> id >> '(' >> lista_p >> ')' >> ':' >> tipo_s));

        //Rule AnyToken = lexeme[+qi::print] - (qi::lit("begin") | "end");
        //Rule Lixo = *AnyToken;
        //Rule Bloco = Lixo >> -("begin" >> Lixo >> "end") >> Lixo;

        lista_c = *qi::lexeme[+qi::print] - "end.";

        p = "program" >> id >> lista_d >> "main" >> '(' >> lista_p >> ')' >> lista_d >> "begin" >> lista_c >> "end.";
    }

#define RULE(type, name) qi::rule<It, Skip, type> name
#define RULE_NT(name) qi::rule<It, Skip> name
    RULE(std::string(), id);
    RULE(unsigned int(), num);
    RULE(Types(), tipo_s); // TODO descobrir que tipo é esse
    // TODO Especificar tipos para todos esses outros
    RULE_NT(tipo);
    RULE_NT(seq);
    RULE_NT(lista_p);
    RULE_NT(lista_d);
    RULE_NT(lista_c);
    RULE_NT(p);
#undef RULE
#undef RULE_NT
};

Grammar grammar;

void test_parse() {
    std::string line;
    std::getline(std::cin, line);

    std::string val;

    auto b = std::begin(line);
    auto e = std::end(line);

    bool r = qi::phrase_parse(b, e, grammar.id, qi::space, val);

    std::cout << val << std::endl;
}

}
