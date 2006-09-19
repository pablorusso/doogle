#!/bin/bash

-include ../makefile.def

OUTPUT_FILE := libparser_query.a

CPP_SRCS += \
$(SRC)/Query.cpp \
$(SRC)/QueryParser.cpp

DEPS += \
${addprefix ./, \
$(OUTPUT)Query.d \
$(OUTPUT)QueryParser.d \
}

OBJS += \
./$(OUTPUT)Query.o \
./$(OUTPUT)QueryParser.o

all: build

build: $(OUTPUT_FILE)

$(OUTPUT_FILE): $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC Archiver'
	@echo ar -r $(OUTPUT)$(OUTPUT_FILE) $(OBJS) $(USER_OBJS) $(LIBS)
	@ar -r $(OUTPUT)$(OUTPUT_FILE) $(OBJS) $(USER_OBJS) $(LIBS)
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

clean:
	@echo 'Cleaning...'
	@if [ -d $(OUTPUT) ]; then $(RM) $(OUTPUT).; $(RMDIR) $(OUTPUT); fi
	@echo ' '

distclean: clean
	@echo 'Dist Cleaning...'
	@if [ -d $(DOC) ]; then $(RM) $(DOC).; $(RMDIR) $(DOC); fi
	@if [ -d $(LIB) ]; then $(RM) $(LIB).; $(RMDIR) $(LIB); fi
	@echo ' '

rebuild : clean build

stage: build
	@if [ $$stage ]; then \
		$(COPY) $(OUTPUT)$(OUTPUT_FILE) $$stage; \
		$(COPY) $(SRC)*.h $$stage; \
	else \
		read -p "Ingrese el stage: " stage; \
		$(COPY) $(OUTPUT)$(OUTPUT_FILE) $$stage; \
		$(COPY) $(SRC)*.h $$stage; \
	fi
