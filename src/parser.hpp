#pragma once

#include <string>
#include <vector>
#include <memory>
#include <iosfwd>

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
    TypeEnum basic_type;
    std::vector<int> array_dimensions;
    bool is_pointer;
    std::unique_ptr<Type> sub_type;
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
    ProcedureDecl main_decl;
};

void test_parse();

void parse(std::istream& in);
void parse_file(const std::string &filename);

}