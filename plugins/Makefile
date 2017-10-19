PLUGINS = NetworkingPlugin LifXBulbPlugin FakeDevicesPlugin NestDevicesPlugin CloudConnector

all: subdirs

subdirs:
	for dir in $(PLUGINS); do \
		$(MAKE) all -C $$dir; \
	done	

install: isubdirs

isubdirs:
	for dir in $(PLUGINS); do \
		$(MAKE) install -C $$dir; \
	done	

clean: csubdirs

csubdirs:
	for dir in $(PLUGINS); do \
		$(MAKE) clean -C $$dir; \
	done	
