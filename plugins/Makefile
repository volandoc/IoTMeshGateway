PLUGINS = LifXBulbPlugin NetworkingPlugin CloudConnector FakeDevicesPlugin NestDevicesPlugin

all: subdirs

subdirs:
	for dir in $(PLUGINS); do \
		$(MAKE) all -C $$dir; \
	done	

clean: csubdirs

csubdirs:
	for dir in $(PLUGINS); do \
		$(MAKE) clean -C $$dir; \
	done	
