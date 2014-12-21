#include "SelectStmt.hpp"
#include "filter.hpp"
#include <iostream>

namespace Stmt {
    void SelectStmt::Run(Context::Context& ctx) {
        auto tbl = filter(ctx, from, where);
        if (selectAll) {
            for (auto& row : tbl.rows) {
                for (auto& item : row.objs)
                    std::cout << item->toString() << " ";
                std::cout << std::endl;
            }
        } else {
            std::vector<std::vector<TypeDB::pObject>> objects;
            for (auto& item : select)
                objects.push_back(item->Calc(tbl));
            std::size_t size = 0xFFFFFFFF;
            for (auto& item : objects)
                size = size < item.size() ? size : item.size();
            for (std::size_t i = 0; i < size; i++) {
                for (auto& item : objects)
                    std::cout << item[i]->toString() << " ";
                std::cout << std::endl;
            }
        }
    }
}
