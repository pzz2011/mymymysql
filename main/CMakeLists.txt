find_package( Threads )
ADD_EXECUTABLE(mymymysql main.cpp)
TARGET_LINK_LIBRARIES(mymymysql Parser  Stmt  context TypeDB  Exception BTree PageDB ${CMAKE_THREAD_LIBS_INIT})
