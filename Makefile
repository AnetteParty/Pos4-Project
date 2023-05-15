# Sample Makefile for wx applications
WX_CONFIG := wx-config
WX_CXXFLAGS := $(shell $(WX_CONFIG) --cxxflags)
#WX_LIBS := $(shell $(WX_CONFIG) --libs std,propgrid)
WX_LIBS := $(shell $(WX_CONFIG) --libs)
LDFLAGS = -lsqlite3

CXXFLAGS = -g

APPLICATION := main
OBJECTS := MainWindow.o SecondWindow.o ThirdWindow.o

all: $(APPLICATION)

$(APPLICATION): $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(LDFLAGS) $(WX_LIBS) $(LIBS)

$(OBJECTS): %.o: %.cpp
	$(CXX) -c -o $@ $(WX_CXXFLAGS) $(CXXFLAGS) $<

.PHONY: clean
clean:
	find . -name '*~' -o -name '*.o' -o -name $(APPLICATION) | xargs rm
