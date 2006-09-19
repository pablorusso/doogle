################################################################################
# Automatically-generated file. Do not edit!
################################################################################

ROOT := ..

-include $(ROOT)/makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include $(SUBDIRS:%=%/subdir.mk)
-include objects.mk
ifneq ($(strip $(DEPS)),)
-include $(DEPS)
endif

-include $(ROOT)/makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: parser_query_test

dependents:
	-cd /home/pablo/facultad/datos/src/parser_query/Debug && $(MAKE) all

# Tool invocations
parser_query_test: $(OBJS) $(USER_OBJS) /home/pablo/facultad/datos/src/parser_query/Debug/libparser_query.a
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	@echo g++ -L../../parser_query/Debug/ -oparser_query_test $(OBJS) $(USER_OBJS) $(LIBS)
	@g++ -L../../parser_query/Debug/ -oparser_query_test $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(DEPS)$(OBJS)$(EXECUTABLES) parser_query_test
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
/home/pablo/facultad/datos/src/parser_query/Debug/libparser_query.a:

-include $(ROOT)/makefile.targets
