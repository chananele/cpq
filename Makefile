OUT			= ./out
SRC			= ./src
INCLUDE 	= ./include
GENERATOR	= ./generators

CXX		 = g++
CXXFLAGS = -I $(INCLUDE) -std=c++11

FILES   = scanner parser driver cpq
HEADERS = $(patsubst %, $(INCLUDE)/%.hh, parser driver)
SOURCES = $(patsubst %, $(SRC)/%.cpp, $(FILES))
OBJECTS = $(patsubst %, $(OUT)/%.o, $(FILES))

cpq : $(OBJECTS)
	$(CXX) -o $@ $^ $(CXXFLAGS)

$(OUT)/%.o : $(SRC)/%.cpp $(HEADERS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(SRC)/scanner.cpp : $(GENERATOR)/scanner.l
	flex -L -o $@ $<

$(INCLUDE)/parser.hh $(SRC)/parser.cpp : $(GENERATOR)/parser.yy
	@# Output goes to the include directory because most of the files that are
	@# generated are headers. Afterwards we move the source file to the appropriate
	@# location.
	bison -l --output=$(INCLUDE)/parser.cpp --defines=$(INCLUDE)/parser.hh $<
	mv $(INCLUDE)/parser.cpp $(SRC)/parser.cpp

.PHONY : look clean

look :
	@echo SOURCES: $(SOURCES)
	@echo HEADERS: $(HEADERS)
	@echo OBJECTS: $(OBJECTS)

clean:
	rm $(patsubst %, $(INCLUDE)/%.hh, location position stack parser)
	rm $(patsubst %, $(SRC)/%.cpp, parser scanner)
	rm out/*
