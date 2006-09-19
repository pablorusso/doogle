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
all: libparser_query.a

# Tool invocations
libparser_query.a: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC Archiver'
	@echo ar -r libparser_query.a $(OBJS) $(USER_OBJS) $(LIBS)
	@ar -r libparser_query.a $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(DEPS)$(OBJS)$(ARCHIVES) libparser_query.a
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include $(ROOT)/makefile.targets
