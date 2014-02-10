TestObjFiles = TestDataTypes.o TestSourceReaders.o TestDataReaders.o
ObjFiles = CSVDataReader.o DataReader.o LineDataReader.o CSVDataType.o FileReader.o RandomCSVSimpleReader.o

Flags=-g

test: $(TestObjFiles) $(ObjFiles)
	g++ $(Flags) -o TestAll TestAll.cpp $(TestObjFiles) $(ObjFiles)

TestDataTypes.o:
	g++ $(Flags) -c TestDataTypes.cpp

TestSourceReaders.o: SimpleFile
	g++ $(Flags) -c TestSourceReaders.cpp

TestDataReaders.o: SimpleCSVFile SimpleFile
	g++ $(Flags) -c TestDataReaders.cpp

SimpleFile:
	echo "test 1\ntest 2\n" > SimpleFile.txt

SimpleCSVFile:
	echo "ColA,ColB,ColC\n1,A,ABC\n2,B,BCD\n3,C,CDE\n" > SimpleCSVFile.txt

CSVDataReader.o:
	g++ $(Flags) -c ../src/DataReaders/CSVDataReader.cpp

DataReader.o:
	g++ $(Flags) -c ../src/DataReaders/DataReader.cpp

LineDataReader.o:
	g++ $(Flags) -c ../src/DataReaders/LineDataReader.cpp

CSVDataType.o:
	g++ $(Flags) -c ../src/DataTypes/CSVDataType.cpp

FileReader.o:
	g++ $(Flags) -c ../src/SourceReaders/FileReader.cpp

RandomCSVSimpleReader.o:
	g++ $(Flags) -c ../src/SourceReaders/RandomCSVSimpleReader.cpp

clear:
	rm $(ObjFiles)
	rm $(TestObjFiles)
	rm SimpleFile.txt
	rm SimpleCSVFile.txt