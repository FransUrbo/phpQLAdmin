# Nothing for you here! This is simply for me, so that I'll can do some
# general CVS stuff easily.
#

DATE    := $(shell date +"%b %e %Y")
TMPDIR  := $(shell tempfile)
VERSION := $(shell cat .version | sed 's@\ .*@@')
INSTDIR := $(TMPDIR)/phpQLAdmin-$(VERSION)
DESTDIR := $(TMPDIR)/phpqladmin-$(VERSION)

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
	  echo -n $(TAG) > .tag; \
	  echo cvs tag: $$TAG; \
	  cvs tag -RF $$TAG; \
	)

install: $(INSTDIR)
	@(echo -n "Instdir:   $(INSTDIR): "; find | cpio -p $(INSTDIR))

excludelist:
	@(cd $(TMPDIR); \
	  find -type d -name CVS -exec find {} \; \
	    | sed 's@\./@@'  > exclude.list; \
	  find -type f -name '.cvsignore' -o -name 'README.cvs' \
	    -o -name '*.new' -o -name '*.orig' -o -name '*~' \
	    -o -name '.#*' | sed 's@\./@@' >> exclude.list; \
	  cat exclude.list | sort | uniq > new; \
	  echo	$(INSTDIR)/Makefile $(INSTDIR)/.version.old \
		$(INSTDIR)/manual $(INSTDIR)/include/config.inc \
		$(INSTDIR)/phpQLadmin.log $(INSTDIR)/doc/README.Monitor \
		$(INSTDIR)/.DEBUG_ME >> new; \
	  mv new exclude.list; \
	  cat exclude.list | xargs rm -rf)

tarball: install excludelist
	@(echo -n "Tarball 1: $(TMPDIR)/phpQLAdmin-$(VERSION).tar.gz: "; \
	  cd $(TMPDIR) && tar -cz --exclude=README.cvs -f phpQLAdmin-$(VERSION).tar.gz phpQLAdmin-$(VERSION); \
	  echo "done."; \
	  echo -n "Tarball 2: $(TMPDIR)/phpQLAdmin-$(VERSION).tar.bz2: "; \
	  cd $(TMPDIR) && tar -cj --exclude=README.cvs --exclude=debian \
		-f phpQLAdmin-$(VERSION).tar.bz2 phpQLAdmin-$(VERSION); \
	  echo "done."; \
	  echo -n "ZIP file:  $(TMPDIR)/phpQLAdmin-$(VERSION).zip: "; \
	  cd $(TMPDIR) && zip -r phpQLAdmin-$(VERSION).zip phpQLAdmin-$(VERSION) > /dev/null; \
	  echo "done.")

debian: install
	@(mv $(INSTDIR) $(DESTDIR); \
	  cd $(DESTDIR); \
	  debuild; \
	  echo "Files is in: "$(DESTDIR))

release: changes tag tarball debian
	@(rcp -x $(TMPDIR)/phpQLAdmin-$(VERSION).tar.gz  aurora:/var/www/phpqladmin/; \
	  rcp -x $(TMPDIR)/phpQLAdmin-$(VERSION).tar.bz2 aurora:/var/www/phpqladmin/; \
	  rcp -x $(TMPDIR)/phpQLAdmin-$(VERSION).zip     aurora:/var/www/phpqladmin/; \
	  rcp -x CHANGES aurora:/var/www/phpqladmin/CHANGES_devel.txt; \
	  rcp -x $(TMPDIR)/*.deb *.dsc *.tar.gz *.changes aurora:/var/www/phpqladmin)

$(INSTDIR):
	@rm -f $(TMPDIR) && mkdir -p $(INSTDIR)

changes: version
	@( \
	  echo "Date: $(DATE)"; \
	  cat CHANGES | sed "s@TO BE ANNOUNCED@Release \($(DATE)\)@" > CHANGES.new; \
	  mv CHANGES.new CHANGES; \
	  cvs commit -m "New release - `cat .version | sed 's@ .*@@'`" CHANGES .version; \
	)

version:
	@(cat .version | sed 's@ (.*@@' > .version.new; \
	  mv .version.new .version)

clean:
	@(find -name '*~' -o -name '.*~' -o -name '.#*' -o -name '#*' | \
	  xargs --no-run-if-empty rm)
