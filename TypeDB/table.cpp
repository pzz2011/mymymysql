#include "table.hpp"
#include "Exception/Exception.hpp"

namespace TypeDB {
    Row Row::merge(const Row& lhs, const Row& rhs) {
        Row ret;
        ret.objs.reserve(lhs.objs.size() + rhs.objs.size());
        ret.objs.insert(ret.objs.end(), lhs.objs.begin(), lhs.objs.end());
        ret.objs.insert(ret.objs.end(), rhs.objs.begin(), rhs.objs.end());
        return std::move(ret);
    }
    int TableDesc::getIndex(const std::string& tbl, const std::string& name, bool force) const {
        int result = -1;
        if (tbl == "") {
            for (std::size_t i = 0; i < descs.size(); i++) {
                if (descs[i].name == name) {
                    if (result == -1)
                        result = i;
                    else
                        throw "ColumnMultiFound";
                }
            }
        } else {
            for (std::size_t i = 0; i < descs.size(); i++) {
                if (descs[i].tbl == tbl && descs[i].name == name) {
                    result = i;
                    break;
                }
            }
        }
        if (result == -1) {
            if (force)
                return -1;
            else
                throw "ColumnNotFound";
        }
        return result;
    }
    pObject TableDesc::getObject(const std::vector<Row*>& rows, const std::string& tbl, const std::string& name, bool force) const {
        auto idx = getIndex(tbl, name, force);
        if (idx == -1)
            return nullptr;
        for (auto& row : rows)
            if (idx < (int)row->objs.size())
                return row->objs[idx];
            else
                idx -= row->objs.size();
        throw "Column Not Found";
    }
    pObject TableDesc::getObject(const Row& row, const std::string& tbl, const std::string& name, bool force) const {
        auto idx = getIndex(tbl, name, force);
        if (idx == -1)
            return nullptr;
        return row.objs[idx];
    }
    void TableDesc::setForeign(const std::string& name, const std::string& fT, const std::string& fK) {
        for (std::size_t i = 0; i < descs.size(); i++) {
            if (descs[i].name == name) {
                descs[i].type->foreignTable = fT;
                descs[i].type->foreignKey = fK;
                return;
            }
        }
        throw "Column Not Found";
    }
    void TableDesc::setPrimary(const std::string& name) { //根据name设置主键索引
        for (std::size_t i = 0; i < descs.size(); i++) {
            if (descs[i].name == name) {
                primaryIndex = i;
                return;
            }
        }
        //TODO
        throw "Column Not Found";
    }
    pObject TableDesc::getPrimary(const Row& row) const { //获取
        return row.objs[primaryIndex];                    //
    }
    TableDesc TableDesc::merge(const TableDesc& lhs, const TableDesc& rhs) {
        TableDesc ret;
        ret.primaryIndex = lhs.primaryIndex;
        ret.descs.reserve(lhs.descs.size() + rhs.descs.size());
        ret.descs.insert(ret.descs.end(), lhs.descs.begin(), lhs.descs.end());
        ret.descs.insert(ret.descs.end(), rhs.descs.begin(), rhs.descs.end());
        return std::move(ret);
    }
    bool TableDesc::Test(const Row& row) const {
        if (row.objs.size() != descs.size())
            return false;
        for (std::size_t i = 0; i < descs.size(); i++) {
            if (!descs[i].type->Test(row.objs[i]))
                return false;
        }
        return true;
    }
    std::vector<pObject> Table::getVec(const std::string& tbl, const std::string& name) const {
        std::size_t index = desc.getIndex(tbl, name);
        std::vector<pObject> ret;
        for (auto row : rows)
            ret.push_back(row.objs[index]);
        return std::move(ret);
    }
}
