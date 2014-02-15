ADD_EXECUTABLE(TestReaders TestReaders.cpp TestDataReaders.cpp TestDataTypes.cpp TestSourceReaders.cpp SimpleFile.txt SimpleCSVFile.txt)

TARGET_LINK_LIBRARIES(TestReaders DataReaders)
TARGET_LINK_LIBRARIES(TestReaders DataTypes)
TARGET_LINK_LIBRARIES(TestReaders SourceReaders)

FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/SimpleFile.txt "test 1\ntest 2\n\n")
FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/SimpleCSVFile.txt "ColA,ColB,ColC\n1,A,ABC\n2,B,BCD\n3,C,CDE\n")

ENABLE_TESTING()
ADD_TEST(Test_TestDataReaders TestReaders TestDataReaders)
ADD_TEST(Test_TestDataTypes TestReaders TestDataTypes)
ADD_TEST(Test_TestSourceReaders TestReaders TestSourceReaders)
