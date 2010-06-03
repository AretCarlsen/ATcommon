ECHO = echo
RM = rm

CXX = g++
MFLAGS =

CXXFLAGS += -Wall -std=c++0x $(DEBUG) -fno-implicit-templates
CXXFLAGS += $(DEFS) $(IPATH)

# -frepo

.DEFAULT:
	@$(MAKE) all

.cpp.o: force
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(ECHO) cleaning up in .
	-$(RM) -f *.o *.rpo core $(TARGET)

fresh: clean all

.PHONY: all clean fresh force


