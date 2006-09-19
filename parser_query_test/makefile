#!/bin/bash

-include ../makefile.def

OUTPUT_FILE := parser_query_test.out
CPP_SRCS    += $(SRC)main.cpp
DEPS 	    += ${ addprefix ./, $(OUTPUT)main.d }
OBJS 	    += $(OUTPUT)main.o
LIBS 	    := -lparser_query
INC  	    := -I$(LIB)

all: build

build: $(OUTPUT_FILE)

$(OUTPUT_FILE): $(LIB)/libparser_query.a $(USER_OBJS) $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	@echo g++ -L$(LIB) -o$(OUTPUT)$(OUTPUT_FILE) $(OBJS) $(USER_OBJS) $(LIBS)
	@g++ -L$(LIB) -o$(OUTPUT)$(OUTPUT_FILE) $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

$(OUTPUT)%.o: $(SRC)%.cpp
	@echo 'Building file: $<'
	@echo 'Making dir: ' $(MKDIR) $(OUTPUT)
	$(MKDIR) $(OUTPUT)
	@echo 'Invoking: GCC C++ Compiler'
	@echo g++ -O0 -g3 -Wall -c -fmessage-length=0 -o$@ $<
	@g++ $(INC) -O0 -g3 -Wall -c -fmessage-length=0 -o$@ $< && \
	echo -n $(@:%.o=%.d) $(dir $@) > $(@:%.o=%.d) && \
	g++ -MM -MG -P -w $(INC) -O0 -g3 -Wall -c -fmessage-length=0  $< >> $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '

$(LIB)/libparser_query.a : depends

depends:
	@echo 'Depends...'
	@echo 'Making dir: ' $(MKDIR) $(LIB)
	$(MKDIR) $(LIB)

	$(COPY) $(STAGE)libparser_query.a $(LIB)
	$(COPY) $(STAGE)Query.h $(LIB)
	$(COPY) $(STAGE)QueryParser.h $(LIB)

clean:
	@echo 'Cleaning...'
	@if [ -d $(OUTPUT) ]; then $(RM) $(OUTPUT).; $(RMDIR) $(OUTPUT); fi
	@echo ' '

distclean: clean
	@echo 'Dist Cleaning...'
	@if [ -d $(DOC) ]; then $(RM) $(DOC)*.*; $(RMDIR) $(DOC); fi
	@if [ -d $(LIB) ]; then $(RM) $(LIB)*.*; $(RMDIR) $(LIB); fi
	@echo ' '

rebuild : clean build

stage: build
	$(COPY) $(OUTPUT)$(OUTPUT_FILE) $(STAGE)
