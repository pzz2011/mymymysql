#include "frontend/Parser.hpp"
#include "context/context.hpp"

int main() {
    PageDB::Scheduler* pgdb = new PageDB::LRU_Scheduler;
    pgdb->StartSchedule();
    Context::Context ctx(pgdb);
    std::remove("test.db");
    std::remove("test:wdy.db");
    std::remove("test:wdy.idx");
    std::remove("test:wy.db");
    std::remove("test:wy.idx");
    std::remove("test:happy.db");
    std::remove("test:happy.idx");
    TypeDB::TableDesc desc;
    TypeDB::ColDesc col_desc1, col_desc2, col_desc3;
    col_desc1.type = new TypeDB::IntType(3);
    col_desc2.type = new TypeDB::StringType(3);
    col_desc3.type = new TypeDB::StringType(3);
    col_desc1.name = "W";
    col_desc2.name = "D";
    col_desc3.name = "Y";
    desc.descs.push_back(col_desc1);
    desc.descs.push_back(col_desc2);
    desc.descs.push_back(col_desc3);
    desc.primaryIndex = 0;
    TypeDB::Table tbl;
    tbl.desc = desc;
    TypeDB::Row row1, row2, row3;
    row1.objs.push_back(new TypeDB::Int(1));
    row1.objs.push_back(new TypeDB::String("wdy"));
    row1.objs.push_back(new TypeDB::String("abc"));
    row2.objs.push_back(new TypeDB::Int(2));
    row2.objs.push_back(new TypeDB::String("wy"));
    row2.objs.push_back(new TypeDB::String("lo"));
    row3.objs.push_back(new TypeDB::Int(3));
    row3.objs.push_back(new TypeDB::String("XYZ"));
    row3.objs.push_back(new TypeDB::String("ve"));
    tbl.rows.push_back(row1);
    tbl.rows.push_back(row2);
    tbl.rows.push_back(row3);
    ctx.InitTable("wdy", desc);
    ctx.Insert("wdy", tbl);
    ctx.InitTable("wy", desc);
    ctx.Insert("wy", tbl);
    ctx.InitTable("happy", desc);
    ctx.Insert("happy", tbl);
    Parser::CreateAST("select wdy.W, D from wdy")->Run(ctx);
    std::cout << std::endl;
    Parser::CreateAST("Select * from wdy")->Run(ctx);
    std::cout << std::endl;
    Parser::CreateAST("select SUM(wdy.W), avg(W), min(W), max(W), sum(D) from wdy")->Run(ctx);
    std::cout << std::endl;
    Parser::CreateAST("select * from wdy where wdy.W = 1 or wdy.D = \"wy\"")->Run(ctx);
    std::cout << std::endl;
    Parser::CreateAST("select * from wdy, wy where wdy.W = wy.W")->Run(ctx);
    std::cout << std::endl;
    Parser::CreateAST("select * from wdy, wy where wdy.W = wy.W and wdy.W = 1")->Run(ctx);
    std::cout << std::endl;
    Parser::CreateAST("select * from wdy, wy, happy where wdy.W = wy.W and wdy.W = happy.W")->Run(ctx);
    std::cout << std::endl;
    Parser::CreateAST("insert into wdy values (0, \"XYZ\", \"ABC\")")->Run(ctx);
    std::cout << std::endl;
    Parser::CreateAST("Select * from wdy")->Run(ctx);
    std::cout << std::endl;
    Parser::CreateAST("update wdy set D = D + \"...\" where W = 1")->Run(ctx);
    Parser::CreateAST("Select * from wdy")->Run(ctx);
    std::cout << std::endl;
    Parser::CreateAST("delete from wdy where W = 1")->Run(ctx);
    Parser::CreateAST("Select * from wdy")->Run(ctx);
    std::cout << std::endl;
    try {
        Parser::CreateAST("insert into wdy values (0, \"XYZ\", \"ABC\")")->Run(ctx);
    } catch (const char* str) {
        std::cout << str << std::endl;
    }
    try {
        Parser::CreateAST("insert into wdy values (0, 1, \"XYZ\", \"ABC\")")->Run(ctx);
    } catch (const char* str) {
        std::cout << str << std::endl;
    }
    try {
        Parser::CreateAST("insert into wdy values (0, 1, \"ABC\")")->Run(ctx);
    } catch (const char* str) {
        std::cout << str << std::endl;
    }
    try {
        Parser::CreateAST("update wdy set D = 1")->Run(ctx);
    } catch (const char* str) {
        std::cout << str << std::endl;
    }
    try {
        Parser::CreateAST("update wdy set W = 1")->Run(ctx);
    } catch (const char* str) {
        std::cout << str << std::endl;
    }
    pgdb->StopSchedule();
    delete pgdb;
}
