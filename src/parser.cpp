#include "parser.hpp"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace parse {

namespace qi = boost::spirit::qi;

enum TypeEnum {
    T_INT,
    T_FLOAT,
    T_CHAR,
    T_BOOLEAN,
    T_VOID,
    T_EXCEPTION
};

struct TypesTable : qi::symbols<char, TypeEnum> {
    TypesTable()
    {
        add("int",       T_INT)
           ("float",     T_FLOAT)
           ("char",      T_CHAR)
           ("boolean",   T_BOOLEAN)
           ("void",      T_VOID)
           ("exception", T_EXCEPTION);
    }
} types_table;

template <typename Iterator>
struct PascalGrammar : qi::grammar<Iterator, qi::space_type, std::string()> {
    typedef qi::space_type Skip;

    PascalGrammar() : PascalGrammar::base_type(p)
    {
        id = qi::lexeme[qi::alpha >> *(qi::alnum)];
        num = qi::uint_;
        tipo_s = *qi::lit('*') >> types_table;

        tipo = types_table | ('*' >> tipo) | ("array" >> seq);
        seq = +('[' >> num >> ']') >> "of" >> tipo;

        // parameter list
        lista_p = *(id >> ':' >> tipo_s >> ';');

        // declaration list
        lista_d = *((id >> ':' >> tipo >> ';') | ("procedure" >> id >> '(' >> lista_p >> ')' >> ':' >> tipo_s >> lista_d >> "begin" >> lista_c >> "end" >> ';'));

        //Rule AnyToken = lexeme[+qi::print] - (qi::lit("begin") | "end");
        //Rule Lixo = *AnyToken;
        //Rule Bloco = Lixo >> -("begin" >> Lixo >> "end") >> Lixo;

        // TODO exception blocks
        lista_c = *(("begin" >> lista_c >> "end") | (qi::lexeme[+qi::print] - "end"));

        p = "program" >> id >> lista_d >> "main" >> '(' >> lista_p >> ')' >> lista_d >> "begin" >> lista_c >> "end" >> qi::lit('.');
    }

#define RULE(type, name) qi::rule<Iterator, Skip, type> name
#define RULE_NT(name) qi::rule<Iterator, Skip> name
    RULE(std::string(), id);
    RULE(unsigned int(), num);
    RULE(TypeEnum(), tipo_s); // TODO descobrir que tipo é esse
    // TODO Especificar tipos para todos esses outros
    RULE(std::string(), tipo);
    RULE(std::string(), seq);
    RULE(std::string(), lista_p);
    RULE(std::string(), lista_d);
    RULE(std::string(), lista_c);
    RULE(std::string(), p);
#undef RULE
#undef RULE_NT
};

PascalGrammar<boost::spirit::istream_iterator> grammar;

void test_parse() {
    //std::string line;
    //std::getline(std::cin, line);

    //std::string val;

    //auto b = std::begin(line);
    //auto e = std::end(line);
  
    //bool r = qi::phrase_parse(b, e, grammar.id[&testf], qi::space, val);
    //std::cout << val << std::endl;
}

void parse_file(const std::string &filename) {
    //namespace spirit = boost::spirit;

    //std::ifstream in(filename.c_str());

    //
    //spirit::istream_iterator begin(in);
    //spirit::istream_iterator end;

    //spirit::qi::parse(begin, end,grammar.);
}

}
