#include "expr.hpp"
#include "Exception/Exception.hpp"

namespace TypeDB {
    std::pair<bool, pObject> Expr::Calc(const TableDesc& desc, const Row& row) {
        throw "Syntax Error";
    }

    std::pair<bool, bool> Expr::CalcBool(const TableDesc& desc, const Row& row) { // 表 行
        throw "Syntax Error";
    }

    Expr::~Expr() {}

    std::pair<bool, pObject> LiteralExpr::Calc(const TableDesc& desc, const Row& row) {
        return std::make_pair(true, Literal);
    }

    std::pair<bool, pObject> ReadExpr::Calc(const TableDesc& desc, const Row& row) {
        auto ret = desc.getObject(row, tbl, name, true);
        if (ret.obj == nullptr)
            return std::make_pair(false, ret);
        else
            return std::make_pair(true, ret);
    }

    std::pair<bool, bool> UnaryExpr::CalcBool(const TableDesc& desc, const Row& row) {
        switch (op) {
            case Not:
                {
                    auto X = l->CalcBool(desc, row);
                    X.second = !X.second;
                    return X;
                }
            case IsNull:
                {
                    auto X = l->Calc(desc, row);
                    Null* ptr = dynamic_cast<Null*>(X.second.obj);
                    return std::make_pair(X.first, ptr != nullptr);
                }
        }
    }

    UnaryExpr::~UnaryExpr() {delete l;}

#define CALC2BOOL(op, def) \
        {\
            auto lhs = l->CalcBool(desc, row), rhs = r->CalcBool(desc, row);\
            if (lhs.first && rhs.first)\
                return std::make_pair(true, lhs.second op rhs.second);\
            else\
                return std::make_pair(false, def);\
        }
#define CALC2OBJ(op, def) \
        {\
            auto lhs = l->Calc(desc, row), rhs = r->Calc(desc, row);\
            if (lhs.first && rhs.first)\
                return std::make_pair(true, lhs.second->op_##op(rhs.second));\
            else\
                return std::make_pair(false, def);\
        }
    std::pair<bool, pObject> BinaryExpr::Calc(const TableDesc& desc, const Row& row) {
        switch (op) {  //双目表达式
            case Plus:  //加
                CALC2OBJ(add, nullptr);
            case Minus: //减
                CALC2OBJ(minus, nullptr);
                break;
            default:
                throw "Syntax Error";
        }
    }

    std::pair<bool, bool> BinaryExpr::CalcBool(const TableDesc& desc, const Row& row) {
        switch (op) {
            case Equal: //相等测试
                CALC2OBJ(eq, false);
            case NotEqual://不等测试
                CALC2OBJ(ne, false);
            case LessThan://小于测试
                CALC2OBJ(lt, false);
            case GreaterThan://大于测试
                CALC2OBJ(gt, false);
            case LessEqual://小于等于测试
                CALC2OBJ(le, false);
            case GreaterEqual://大于等于测试
                CALC2OBJ(ge, false);
            case Like://Like
                CALC2OBJ(like, false);
            case And: //与
                {
                    auto lhs = l->CalcBool(desc, row), rhs = r->CalcBool(desc, row);
                    if (lhs.first && rhs.first)
                        return std::make_pair(true, lhs.second && rhs.second);
                    if (!lhs.first && !rhs.first)
                        return std::make_pair(false, false);
                    if (lhs.first)
                        return std::make_pair(!lhs.second, lhs.second);
                    if (rhs.first)
                        return std::make_pair(!rhs.second, rhs.second);
                }
            case Or: //或
                {
                    auto lhs = l->CalcBool(desc, row), rhs = r->CalcBool(desc, row);
                    if (lhs.first && rhs.first)
                        return std::make_pair(true, lhs.second || rhs.second);
                    if (!lhs.first && !rhs.first)
                        return std::make_pair(false, false);
                    if (lhs.first)
                        return std::make_pair(lhs.second, lhs.second);
                    if (rhs.first)
                        return std::make_pair(rhs.second, rhs.second);
                }
            default: //其他则语法错误
                throw "Syntax Error";
        }
    }

    BinaryExpr::~BinaryExpr() {delete l; delete r;} //删除左表达式  删除右表达式
}
