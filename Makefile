# Nothing for you here! This is simply for me, so that I'll can do some
# general CVS stuff easily.
#

TMPDIR  := $(shell tempfile)
VERSION := $(shell cat .version)
INSTDIR := $(TMPDIR)/phpQLAdmin-$(VERSION)

# Make a unified diff over the changes from the last version...
diff:
	@(VERSION=`cat .version.old`; \
	  MAJOR=`expr substr $$VERSION 1 1`; \
	  MINOR=`expr substr $$VERSION 3 1`; \
	  LEVEL=`expr substr $$VERSION 5 2`; \
	  TAG="REL_`echo $$MAJOR`_`echo $$MINOR`_`echo $$LEVEL`"; \
	  echo cvs rdiff: $$TAG; \
	  cvs rdiff -ur $$TAG phpQLAdmin; \
	)

tag:
	@(VERSION=`cat .version | sed 's@ .*@@'`; \
	  MAJOR=`expr substr $$VERSION 1 1`; \
	  MINOR=`expr substr $$VERSION 3 1`; \
	  LEVEL=`expr substr $$VERSION 5 2`; \
	  OLDLV=`expr $$LEVEL - 1`; \
	  echo "$$MAJOR.$$MINOR.$$OLDLV" > .version.old; \
	  echo "$$MAJOR.$$MINOR.$$LEVEL" > .version; \
	  echo -n "We are now at version "; \
	  cat  < .version; \
	  TAG="REL_`echo $$MAJOR`_`echo $$MINOR`_`echo $$LEVEL`"; \
	  echo cvs tag: $$TAG; \
	  cvs tag -RF $$TAG; \
	  cvs commit -m "New release - $$MAJOR.$$MINOR.$$NEWLV." .version .version.old; \
	)

install: $(INSTDIR)
	@(echo -n "Instdir: $(INSTDIR): "; \
	  find | cpio -p $(INSTDIR); \
	  rm $(INSTDIR)/Makefile $(INSTDIR)/.version.old; \
	  echo -n "Tarball 1: $(TMPDIR)/phpQLAdmin-$(VERSION).tar.gz: "; \
	  cd $(TMPDIR) && tar czf phpQLAdmin-$(VERSION).tar.gz phpQLAdmin-$(VERSION); \
	  echo "done."; \
	  echo -n "Tarball 2: $(TMPDIR)/phpQLAdmin-$(VERSION).tar.bz2: "; \
	  cd $(TMPDIR) && tar cjf phpQLAdmin-$(VERSION).tar.bz2 phpQLAdmin-$(VERSION); \
	  echo "done."; \
	)

$(INSTDIR):
	@rm -f $(TMPDIR) && mkdir -p $(INSTDIR)
