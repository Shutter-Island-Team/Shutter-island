REFDIR=.
DOCC=doxygen
HTMLDIR=$(REFDIR)/html

# Rules for Doxygen files
doc: $(REFDIR)/doxyfilerc
	$(DOCC) $(REFDIR)/doxyfilerc

open: doc
	firefox $(HTMLDIR)/index.html &

clean:
	rm -rf $(HTMLDIR)

all:
	$(HTMLDIR)/index.html

.PHONY: all doc
