# Nothing for you here! This is simply for me, so that I'll can do some
# general CVS stuff easily.
#
diff:
	# Make a unified diff over the changes from the last version...
	@(VERSION=`cat .version.old`; \
	  MAJOR=`expr substr $$VERSION 1 1`; \
	  MINOR=`expr substr $$VERSION 3 1`; \
	  LEVEL=`expr substr $$VERSION 5 2`; \
	  TAG="`echo REL_`echo $$MAJOR`_`echo $$MINOR`_`echo $$LEVEL`"; \
	  echo cvs rdiff: $$TAG; \
	  cvs rdiff -ur $$TAG $(PACKAGE); \
	)

tag:
	@(cp .version .version.old; \
	  VERSION=`cat .version`; \
	  MAJOR=`expr substr $$VERSION 1 1`; \
	  MINOR=`expr substr $$VERSION 3 1`; \
	  LEVEL=`expr substr $$VERSION 5 2`; \
	  NEWLV=`expr $$LEVEL + 1`; \
	  echo "$$MAJOR.$$MINOR.$$NEWLV" > .version; \
	  echo -n "We are now at version "; \
	  cat  < .version; \
	  TAG="`echo REL_`echo $$MAJOR`_`echo $$MINOR`_`echo $$LEVEL`"; \
	  echo cvs tag: $$TAG; \
	  cvs commit -m "New release - $$MAJOR.$$MINOR.$$NEWLV." .version .version.old; \
	  cvs tag -RF $$TAG; \
	)
