#ifndef WDY_1920382934_FILE
#define WDY_1920382934_FILE
#include <fstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <mutex>

namespace PageDB {
    struct Location {//位置
        unsigned short Page, Offset;//页，偏移
        Location() {}//
        Location(unsigned short _Page, unsigned short _Offset)
            : Page(_Page), Offset(_Offset) {}
    };
    inline bool operator==(Location lhs, Location rhs) { //位置相等的语义包括同一个页&&同样的偏移
        return lhs.Page == rhs.Page && lhs.Offset == rhs.Offset;
    }
    inline bool operator!=(Location lhs, Location rhs) {
        return !(lhs == rhs);
    }
    struct Page;
    const int PAGE_SIZE = 8192; //页大小
    const int MaxPage = 2045;   //最大页号
    const int MagicNumber = 0x19941102;//magic数
    const int MagicNumberOffset = 0;//0                   [0,1,2,3]
    const int EntryPageOffset = MagicNumberOffset + 4;//4 [4,5]
    const int EofPageOffset = EntryPageOffset + 2;//6     [6,7]
    const int EofOffOffset = EofPageOffset + 2;//8        [8,9]
    const int PageCountOffset = EofOffOffset + 2;//10     [10,11]
    const int PageMapOffset = PageCountOffset + 2;//12    [12,13]
    struct File {
        std::fstream raw;       //文件流
        std::mutex raw_mutex;   //互斥所
        int entryPageID;        //入口id？
        Location eof;           // ifstream, ofstream和fstream分别从类istream，ostream以及iostream派生而来
        std::unordered_map<int, int> pageMap;
        int pageOffset(int vaddr);
        int newPage();          // ofstream是从内存到硬盘，ifstream是从硬盘到内存
        void removePage(int pageid);// c++中有个一个stream类，所有的I/O都是以这个流为基础的，包括我们要认识的文件I/O
        void writebackFileHeaderCore(bool lock = true);//stream这个类有两个重要的运算符
        void writebackFileHeader();// 插入器(<<) 向流输出数据。 比如说系统有一个默认的标准输出流cout，一般情况下就是
        File(const std::string&);//指向显示器，所以cout<<"Write StdOut"<<"\n";就会把字符串"Write StdOut"和换行符"\n"
        Page* loadPage(int page_id);//输出到标准输出流cout中。
        void writePage(int page_id, Page* pg);
        ~File() {               //    析取器(>>) 向流输入数据。 比如说系统有一个默认的标准输入流cin， 一般情况下就是
            raw.close();        // 指键盘 
        }
        void initFile();        // 在C++中，对文件的
        void readFile();
    };
}
#endif


//c++中的文件定位分为读位置和写位置
//file1.seekp(1234,ios::beg);  把文件的 写指针 从文件的开头 后移1234个字节
//file2.seekg(1234，ios::cur); 把文件的 读指针 从文件的当前位置向后移1234字节
//ios::beg 文件开头 
//ios::cur 文件当期位置
//ios::end 文件结尾
//

