CORELIBS = InnerBus

all: subdirs

subdirs:
	for dir in $(CORELIBS); do \
		$(MAKE) all -C $$dir; \
	done	

clean: csubdirs

csubdirs:
	for dir in $(CORELIBS); do \
		$(MAKE) clean -C $$dir; \
	done	
