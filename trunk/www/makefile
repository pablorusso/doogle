#!/bin/bash

-include ../makefile.def

all: build

build: $(LIB)/thin_client.out $(LIB)/result.template

$(LIB)/thin_client.out : depends

$(LIB)/result.template : depends

depends:
	@echo 'Depends...'
	@echo 'Making dir: ' $(MKDIR) $(LIB)
	$(MKDIR) $(LIB)

	$(COPY) $(STAGE)thin_client.out $(LIB)
	$(COPY) $(STAGE)result.template $(LIB)

clean:
	@echo 'Cleaning...'
	@if [ -d $(LIB) ]; then $(RM) $(LIB).; $(RMDIR) $(LIB); fi
	@echo ' '

distclean: clean
	@echo 'Dist Cleaning...'
	@echo ' '

rebuild : clean build

stage: build
