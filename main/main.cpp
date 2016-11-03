#include "frontend/Parser.hpp"
#include <string>
#include <iostream>
#include <fstream>

void RunStmt(const std::string& sql, Context::Context& ctx) {
#ifndef DEBUG
    try {
#endif
        auto X = Parser::CreateAST(sql);//Parser::CreateAST()返回Stmt::Stmt*
        X->Run(ctx);//Stmt对象指针运行Run方法
#ifndef DEBUG
    } catch (const char* ex) {
        std::cout << ex << std::endl;
    }
#endif
}

int main(int argc, char** argv) {
    PageDB::Scheduler* pgdb = new PageDB::LRU_Scheduler;  //Scheduler是LRU_Scheduler的超类
    pgdb->StartSchedule();                                //
    Context::Context ctx(pgdb);
    if (argc == 2) {
        std::ifstream input(argv[1]);
        if (!input) {
            std::cout << "File Not Found" << std::endl;
            return 0;
        }
        std::vector<char> sql;
        char c = '\0';
        while (input) {
            int x = input.get();//Extracts a single character from the stream
            if (x == -1)
                break;
            if (c == '\0' && x == ';') {
                RunStmt(std::string(sql.begin(), sql.end()), ctx);
                sql.clear();
            } else {
                if (c == '\0' && x == '\'') //区别单引号内的语句和双引号内的语句
                    c = '\'';
                else if (c == '\0' && x == '"')
                    c = '"';
                else if (c == '\'' && x == '\'')
                    c = '\0';
                else if (c == '"' && x == '"')
                    c = '\0';
                sql.push_back(x);
            }
        }
    } else {//直接从终端获取sql语句
        while (true) {
            std::string sql;
            std::cout << ctx.dbName << " >> ";
            std::getline(std::cin, sql);// 
            if (!std::cin)
                break;
            if (sql.substr(0, 4) == "exit")
                break;
            RunStmt(sql, ctx); //执行sql语句
        }
    }
    pgdb->StopSchedule();
    delete pgdb;
}
