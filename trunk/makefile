all:
	-cd parser_query && $(MAKE) all
	-cd parser_html && $(MAKE) all
	-cd thin_client && $(MAKE) all
	-cd www && $(MAKE) all

stage:
	-cd parser_query && $(MAKE) stage
	-cd parser_html && $(MAKE) stage
	-cd thin_client && $(MAKE) stage
	-cd www && $(MAKE) stage

clean:
	-cd www && $(MAKE) clean
	-cd thin_client && $(MAKE) clean
	-cd parser_html && $(MAKE) clean
	-cd parser_query && $(MAKE) clean
	
distclean:
	-cd www && $(MAKE) distclean
	-cd thin_client && $(MAKE) distclean
	-cd parser_html && $(MAKE) distclean
	-cd parser_query && $(MAKE) distclean	

stageclean:
	-cd www && $(MAKE) stageclean
	-cd thin_client && $(MAKE) stageclean
	-cd parser_html && $(MAKE) stageclean
	-cd parser_query && $(MAKE) stageclean
