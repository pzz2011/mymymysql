#ifndef WDY_0293812012_EXPR
#define WDY_0293812012_EXPR
#include "table.hpp"

namespace TypeDB {
    struct Expr {
        virtual std::pair<bool, pObject> Calc(const TableDesc& desc, const Row& row);
        virtual std::pair<bool, bool> CalcBool(const TableDesc& desc, const Row& row);
        virtual ~Expr();
    };
    struct LiteralExpr : public Expr {
        pObject Literal;
        LiteralExpr(const pObject& l) : Literal(l) {}
        LiteralExpr(pObject&& l) : Literal(std::move(l)) {}
        virtual std::pair<bool, pObject> Calc(const TableDesc& desc, const Row& row);
    };
    struct ReadExpr : public Expr {
        std::string tbl, name;
        ReadExpr(const std::string& _tbl, const std::string& _name) : tbl(_tbl), name(_name) {}
        ReadExpr(const std::string& _name) : tbl(""), name(_name) {}
        virtual std::pair<bool, pObject> Calc(const TableDesc& desc, const Row& row);
    };
    struct UnaryExpr : public Expr { //单目运算符
        Expr *l;                     //表达式指针
        enum OpCode {
            Not,
            IsNull,
        } op;                        //单目运算符类型
        UnaryExpr(Expr* _l, OpCode _op) : l(_l), op(_op) {} //初始化表达式指针 以及 初始化单目运算符类型
        virtual std::pair<bool, bool> CalcBool(const TableDesc& desc, const Row& row); //单目表达式只计算真假
        virtual ~UnaryExpr();
    };
    struct BinaryExpr : public Expr {  //双目运算符
        Expr *l, *r;        //左表达式指针   右表达式指针
        enum OpCode {
            Plus,           // +
            Minus,          // -
            Equal,          // =
            NotEqual,       // !=
            LessThan,       // <
            GreaterThan,    // >
            LessEqual,      // <=
            GreaterEqual,   // >=
            And,            // &&
            Or,             // ||
            Like,           // Like 
        } op;               // 操作符类型
        BinaryExpr(Expr* _l, Expr* _r, OpCode _op) : l(_l), r(_r), op(_op) {} //初始化左表达式指针 右表达式指针 操作符类型
        virtual std::pair<bool, pObject> Calc(const TableDesc& desc, const Row& row); //双目运算符计算值
        virtual std::pair<bool, bool> CalcBool(const TableDesc& desc, const Row& row);//双目运算符计算bool值
        virtual ~BinaryExpr();                                                // 析构函数
    };
}

#endif


// std::pair<bool,pObject>
// std::pari<bool,bool>
