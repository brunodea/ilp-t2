#include "parser.hpp"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_container.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <iostream>
#include <fstream>
#include <cstdlib>

BOOST_FUSION_ADAPT_STRUCT(
    parse::Param,
    (std::string, name)
    (parse::SType, type)
)

BOOST_FUSION_ADAPT_STRUCT(
    parse::VariableDecl,
    (std::string, id)
    (parse::Type, type)
)

BOOST_FUSION_ADAPT_STRUCT(
    parse::ProcedureDecl,
    (std::string, id)
    (std::vector<parse::Param>, param_list)
    (parse::SType, return_type)
    (parse::DeclList, declarations)
)

BOOST_FUSION_ADAPT_STRUCT(
    parse::Program,
    (std::string, id)
    (parse::DeclList, declarations)
    (std::vector<parse::Param>, main_params)
    (parse::DeclList, main_decls)
)

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

        tipo =
            (
                qi::lit('*')    [phx::bind(&Type::is_pointer, _val) = true]
                | qi::eps       [phx::bind(&Type::is_pointer, _val) = false]
            ) >> -(
                "array" >
                +(
                    '[' >> num[phx::push_back(phx::bind(&Type::array_dimensions, _val), _1)] >> ']'
                ) >> "of"
            ) >> (types_table | tipo)[phx::bind(&Type::type, _val) = _1];

        // parameter list
        lista_p = *(id >> ':' > tipo_s >> ';'); // Adapted with fusion

        // variable declaration
        var_decl = id >> ':' > tipo >> ';'; // Adapted with fusion

        // procedure declaration
        // Adapted with fusion
        proc_decl = "procedure" > id > '(' > lista_p >> ')' > ':' > tipo_s >> lista_d >> "begin" > lista_c >> "end" > ';';

        // declaration list
        lista_d =
            *(
                proc_decl       [phx::push_back(phx::bind(&DeclList::procedures, _val), _1)]
                | var_decl      [phx::push_back(phx::bind(&DeclList::variables,  _val), _1)]
            );

        //Rule AnyToken = lexeme[+qi::print] - (qi::lit("begin") | "end");
        //Rule Lixo = *AnyToken;
        //Rule Bloco = Lixo >> -("begin" >> Lixo >> "end") >> Lixo;

        // code statements
        // TODO exception blocks
        lista_c = *(("begin" > lista_c >> "end") | (qi::lexeme[+qi::print] - "end"));

        // program
        // Adapted with fusion
        p = "program" > id > lista_d > "main" > qi::lit('(') > lista_p > ')' > lista_d > "begin" > lista_c > "end" > qi::lit('.');

        qi::on_error<qi::fail>(p, std::cerr
            << phx::val("Error! Expecting ")
            << qi::_4                               // what failed?
            << phx::val(" here: \"")
            << phx::construct<std::string>(qi::_3, qi::_2)   // iterators to error-pos, end
            << phx::val("\"")
            << std::endl);
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

/*
struct print_type : boost::static_visitor<std::string> {
    std::string operator()(const TypeEnum& te) const
    {
        switch (te) {
        case T_INT:       return "int";
        case T_FLOAT:     return "float";
        case T_CHAR:      return "char";
        case T_BOOLEAN:   return "boolean";
        case T_VOID:      return "void";
        case T_EXCEPTION: return "exception";
        default:          return "???";
        }
    }

    std::string operator()(const Type& t) const
    {
        std::string s;

        if (t.is_pointer)
            s += "*";
        for (auto i = std::begin(t.array_dimensions), end = std::end(t.array_dimensions); i != end; ++i) {
            s += "[";
            s += std::to_string((long long)*i);
            s += "]";
        }

        s += boost::apply_visitor(print_type(), t.type);

        return s;
    }
};
*/

void test_parse() {
    namespace spirit = boost::spirit;

    //Type val;
    Program prog;

    std::cin.unsetf(std::ios::skipws);
    spirit::istream_iterator b(std::cin);
    spirit::istream_iterator e;
  
    bool r = qi::phrase_parse(b, e, grammar, qi::space, prog);

    std::cerr << "aight." << std::endl;
    std::cerr << (b == e) << ", " << r << std::endl;
    
    //std::cerr << print_type()(val) << std::endl;

    std::system("pause");
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
