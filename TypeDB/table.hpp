#ifndef WDY_9218312393_TABLE
#define WDY_9218312393_TABLE
#include "type.hpp"
#include <vector>
#include <string>
#include "Exception/Exception.hpp"

namespace TypeDB {
    struct ColDesc {
        std::string tbl, name;
        Type* type;
        ColDesc() {}
        ColDesc(const std::string& _name, Type* _type) : name(_name), type(_type) {}
        std::string toString() {
            if (tbl.empty())
                return name;
            else
                return tbl + '.' + name;
        }
    };
    struct Row {     // 行
        std::vector<pObject> objs; //用vector存储objs
        static Row merge(const Row& lhs, const Row& rhs);//行合并
    };
    struct TableDesc {
        std::vector<ColDesc> descs;         //列描述
        std::size_t primaryIndex;           // 主键索引
        TableDesc() : primaryIndex(0) {}    //
        void setPrimary(const std::string& name);
        void setForeign(const std::string& name, const std::string& fT, const std::string& fK);
        int getIndex(const std::string& tbl, const std::string& name, bool force = false) const;
        pObject getObject(const std::vector<Row*>& rows, const std::string& tbl, const std::string& name, bool force = false) const;
        pObject getObject(const Row& rows, const std::string& tbl, const std::string& name, bool force = false) const;
        pObject getPrimary(const Row& row) const;
        static TableDesc merge(const TableDesc& lhs, const TableDesc& rhs);
        bool Test(const Row& row) const;
    };
    struct Table {               //一个表包含一个表描述符
        TableDesc desc;          
        std::vector<Row> rows;   // 以及多个行rows
        std::vector<pObject> getVec(const std::string& tbl, const std::string& name) const;//
    };
}

#endif
