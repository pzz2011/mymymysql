#ifndef WDY_9282100293_SCHEDULER
#define WDY_9282100293_SCHEDULER
#include "file.hpp"
#include "page.hpp"
#include <map>
#include <string>
#include <thread>
#include <chrono>

namespace PageDB {
    //Sleep Seconds
    const auto SCHEDULE_LOOP_DELAY = std::chrono::milliseconds(100);
    class Scheduler {
        std::thread ScheduleThread;
        void ScheduleLoop();
        bool running;
    protected:
        std::map<std::string, File*> fileIndex; //子类可以访问protected数据 文件索引
        std::map<std::pair<File*, int>, PageDesc*> pageIndex;//页索引
        virtual void Schedule() = 0; //纯虚幻书
    public:
        Scheduler() : running(false) {}
        void StartSchedule();
        void StopSchedule();
        bool Running() {
            return running;
        }
        File* OpenFile(const std::string& fn);
        File* InitFile(const std::string& fn);
        PageDesc* GetPage(File* file, int page_id);
        void RemovePage(File* file, int page_id);
        PageSession GetSession(File* file, int page_id);
        PageWriteSession GetWriteSession(File* file, int page_id);
        virtual ~Scheduler();
    };
}

#endif
