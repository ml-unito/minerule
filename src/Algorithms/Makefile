SOURCES=$(shell ls *.cpp)
OBJECTS=$(SOURCES:%.cpp=$(OBJDIR)/%.o)
CXXFLAGS+=-I$(LIBGISTHOME)

all:$(OBJECTS)

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
