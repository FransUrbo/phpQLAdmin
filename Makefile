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
	  cvs commit -m "New release - $$MAJOR.$$MINOR.$$LEVEL." \
		.version .version.old CHANGES; \
	  cvs tag -RF $$TAG; \
	)

install: $(INSTDIR)
	@(echo -n "Instdir:   $(INSTDIR): "; find | cpio -p $(INSTDIR))

tarball: install
	@(rm -Rf $(INSTDIR)/Makefile $(INSTDIR)/.version.old \
		$(INSTDIR)/manual $(INSTDIR)/include/config.inc \
		$(INSTDIR)/phpQLadmin.log; \
	  cd $(INSTDIR) && find -type d -name CVS -o -name '.cvsignore' -o -name '*~' | \
		xargs rm -rf; \
	  echo -n "Tarball 1: $(TMPDIR)/phpQLAdmin-$(VERSION).tar.gz: "; \
	  cd $(TMPDIR) && tar -cz --exclude=README.cvs -f phpQLAdmin-$(VERSION).tar.gz phpQLAdmin-$(VERSION); \
	  echo "done."; \
	  echo -n "Tarball 2: $(TMPDIR)/phpQLAdmin-$(VERSION).tar.bz2: "; \
	  cd $(TMPDIR) && tar -cj --exclude=README.cvs --exclude=debian \
		-f phpQLAdmin-$(VERSION).tar.bz2 phpQLAdmin-$(VERSION); \
	  echo "done.")

debian: install
	@(mv $(INSTDIR) $(DESTDIR); \
	  cd $(DESTDIR); \
	  debuild; \
	  echo "Files is in: "$(DESTDIR))

release: changes tag tarball debian
	@(mv -v $(TMPDIR)/phpQLAdmin-$(VERSION).tar.gz /var/www/phpqladmin/; \
	  mv -v $(TMPDIR)/phpQLAdmin-$(VERSION).tar.bz2 /var/www/phpqladmin/; \
	  cat /var/www/phpqladmin/index.html.in | \
		sed -e "s@%VERSION%@$(VERSION)@g" -e "s@%CVSTAG%@`cat .tag`@g" \
		> /var/www/phpqladmin/index.html.out; \
	  mv /var/www/phpqladmin/index.html.out /var/www/phpqladmin/index.html; \
	  cp -v CHANGES /var/www/phpqladmin/CHANGES.txt; \
	  cp -v $(DESTDIR)/../*.deb /var/www/phpqladmin; \
	  rm .tag)

$(INSTDIR):
	@rm -f $(TMPDIR) && mkdir -p $(INSTDIR)

changes:
	@( \
	  echo "Date: $(DATE)"; \
	  cat CHANGES | sed "s@TO BE ANNOUNCED@Release \($(DATE)\)@" > CHANGES.new; \
	  mv CHANGES.new CHANGES; \
	)
