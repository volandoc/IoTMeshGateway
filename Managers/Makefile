MANAGERS := $(sort $(dir $(wildcard */)))

all: subdirs

subdirs:
	for dir in $(MANAGERS); do \
		$(MAKE) all -C $$dir; \
	done	

clean: csubdirs

csubdirs:
	for dir in $(MANAGERS); do \
		$(MAKE) clean -C $$dir; \
	done	
