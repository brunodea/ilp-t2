#pragma once

#include <string>
#include <vector>
#include <memory>
#include <iosfwd>
#include <boost/variant.hpp>
#include <boost/variant/recursive_wrapper.hpp>
#include <exception>

namespace parse {

enum TypeEnum {
    T_INT,
    T_FLOAT,
    T_CHAR,
    T_BOOLEAN,
    T_VOID,
    T_EXCEPTION
};

struct SType {
    int pointer_indirections;
    TypeEnum type;
};

// Parameter List
struct Param {
    std::string name;
    SType type;
};

struct Type {
    std::vector<int> array_dimensions;
    bool is_pointer;
    boost::variant<
        TypeEnum,
        boost::recursive_wrapper<Type>
    > type;
};

struct VariableDecl;
struct ProcedureDecl;

struct DeclList {
    std::vector<ProcedureDecl> procedures;
    std::vector<VariableDecl> variables;
};

struct VariableDecl {
    std::string id;
    Type type;
};

struct ProcedureDecl {
    std::string id;
    std::vector<Param> param_list;
    SType return_type;
    DeclList declarations;
    // TODO? command list
};

struct Program {
    std::string id;
    DeclList declarations;

    std::vector<Param> main_params;
    DeclList main_decls;
    // TODO? command list
};


void test_parse();
bool parse(Program* dest, std::istream& in);
void parse_file(const std::string &filename);

}