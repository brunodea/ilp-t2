#include "parser.hpp"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include <iostream>
#include <fstream>

namespace parse {

namespace qi = boost::spirit::qi;
namespace phx = boost::phoenix;

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
struct PascalGrammar : qi::grammar<Iterator, qi::space_type, Program()> {
    typedef qi::space_type Skip;

    PascalGrammar() : PascalGrammar::base_type(p)
    {
        using qi::_val;
        using qi::_1;

        id = qi::lexeme[qi::alpha >> *(qi::alnum)];
        num = qi::uint_;
        tipo_s =
            qi::eps             [phx::bind(&SType::pointer_indirections, _val) = 0] >>
            *qi::lit('*')       [phx::bind(&SType::pointer_indirections, _val) += 1] >>
            types_table         [phx::bind(&SType::type, _val) = _1];

        //tipo = types_table | ('*' >> tipo) | ("array" >> seq);
        //seq = +('[' >> num >> ']') >> "of" >> tipo;

        // parameter list
        //lista_p = *(id >> ':' >> tipo_s >> ';');

        // variable declaration
        //var_decl = id >> ':' >> tipo >> ';';
        // procedure declaration
        //proc_decl = "procedure" >> id >> '(' >> lista_p >> ')' >> ':' >> tipo_s >> lista_d >> "begin" >> lista_c >> "end" >> ';';
        // declaration list
        //lista_d = *(var_decl | proc_decl);

        //Rule AnyToken = lexeme[+qi::print] - (qi::lit("begin") | "end");
        //Rule Lixo = *AnyToken;
        //Rule Bloco = Lixo >> -("begin" >> Lixo >> "end") >> Lixo;

        // TODO exception blocks
        //lista_c = *(("begin" >> lista_c >> "end") | (qi::lexeme[+qi::print] - "end"));

        //p = "program" >> id >> lista_d >> "main" >> '(' >> lista_p >> ')' >> lista_d >> "begin" >> lista_c >> "end" >> qi::lit('.');
    }

#define RULE(type, name) qi::rule<Iterator, Skip, type> name
#define RULE_NT(name) qi::rule<Iterator, Skip> name
    RULE(std::string(), id);
    RULE(unsigned int(), num);
    RULE(SType(), tipo_s);
    RULE(Type(), tipo);
    RULE(std::string(), seq); // TODO tipo certo
    RULE(std::vector<Param>(), lista_p);
    RULE(VariableDecl(), var_decl);
    RULE(ProcedureDecl(), proc_decl);
    RULE(DeclList(), lista_d);
    RULE_NT(lista_c);
    RULE(Program(), p);
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
