#ifndef WDY_9812310210_TYPE
#define WDY_9812310210_TYPE
#include <string>
#include "object.hpp"

namespace TypeDB {
    enum TypeEnum : unsigned char {
        IntEnum,
        StringEnum,
        NullEnum,
        ErrorTypeEnum,
    };
    struct Type {
        int desc;
        bool null_;
        std::string foreignTable; //foreignTable
        std::string foreignKey;   //foreignKey 外
        TypeEnum type;
        Type(int _desc = 0, bool _null_ = true, TypeEnum _type = ErrorTypeEnum) : desc(_desc), null_(_null_), type(_type) {}
        virtual void Jump(const char*& buf) = 0;
        virtual Object* CreateAndJump(const char*& buf) = 0;
        virtual bool Test(Object* obj);
        virtual std::string toString();
        virtual ~Type(){}
    };
    struct NullType {
        static Null* none;
        static Null* Create() {
            return none;
        }
    };
    struct IntType : public Type {
        static Int* Create(int x) {
            return new Int(x);
        }
        IntType(int _desc, bool _null_ = true) : Type(_desc, _null_, IntEnum) {}
        virtual void Jump(const char*& buf);
        virtual Object* CreateAndJump(const char*& buf);
        virtual std::string toString();
        virtual bool Test(Object* obj);
    };
    struct StringType : public Type {
        static String* Create(const std::string& x) {
            return new String(x);
        }
        StringType(int _desc, bool _null_ = true) : Type(_desc, _null_, StringEnum) {}
        virtual void Jump(const char*& buf);
        virtual Object* CreateAndJump(const char*& buf); //？？
        virtual std::string toString();
        virtual bool Test(Object* obj);
    };
    void initTypes();
    typedef Type* (*TypeCreator)(int _desc, bool _null_);
    extern TypeCreator typeCreators[ErrorTypeEnum];
}

#endif
