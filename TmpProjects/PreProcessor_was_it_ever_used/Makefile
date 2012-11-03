SOURCES=$(shell ls *.cpp)
OBJECTS=$(SOURCES:%.cpp=$(OBJDIR)/%.o)

all:$(OBJECTS)

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
