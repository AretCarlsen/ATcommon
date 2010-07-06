
clean:
	@$(ECHO) $(MSG_CLEAN)
	-$(RM) -f *.list *.binary
# $(OBJS)

fresh: clean all

.PHONY: all clean fresh force program

# Warning level
WARN = 1
# BSTC optimizations
OPTIM = -Or

%.binary: %.spin
	bstc -b $(LIBS) -w$(WARN) -l $< $(OPTIM) $(LIBS) -o $*

