include ../makefile_cons.def

all: build

build: $(LIB)/thin_client.out $(LIB)/result.template
$(LIB)/thin_client.out : depends
$(LIB)/result.template : depends

depends:
	@echo 'Compilando dependencias...'
	@echo ' '	
	@echo 'Creando directorio de dependencias: ' $(MKDIR) $(LIB)
	@$(MKDIR) $(LIB)
	@echo 'Cropiando dependencias binarias: ' $(COPY) $(STAGE)thin_client.out $(LIB)
	@$(COPY) $(STAGE)thin_client.out $(LIB)
	@echo 'Cropiando dependencias con htmls: ' $(COPY) $(STAGE)result.template $(LIB)
	@$(COPY) $(STAGE)result.template $(LIB)

rebuild : clean build

stage: build


clean:
	@echo 'Limpienza...'
	@echo ' '	
	@echo 'Borrando carpeta de salida:'
	@if [ -d $(LIB) ]; then echo $(RM) $(LIB)*.*; $(RM) $(LIB)*.*; fi
	@if [ -d $(LIB) ]; then echo $(RMDIR) $(LIB); $(RMDIR) $(LIB); fi
	@echo ' '
	@echo 'Limpieza realizada con exito'

distclean: clean
	@echo 'Limpiando la distribucion...'
	@echo ' '
	@echo 'La distribucion se limpio con exito'

stageclean: distclean
	@echo 'Limpiando stage...'
	@echo ' '	
	@echo 'El stage se limpio con exito'

rebuild : clean build

