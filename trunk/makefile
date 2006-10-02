all:
	-cd parser_query && $(MAKE) all
	-cd parser_query_test && $(MAKE) all
	-cd thin_client && $(MAKE) all
	-cd www && $(MAKE) all

stage:
	-cd parser_query && $(MAKE) stage
	-cd parser_query_test && $(MAKE) stage
	-cd thin_client && $(MAKE) stage
	-cd www && $(MAKE) stage

clean:
	-cd parser_query_test && $(MAKE) clean
	-cd parser_query && $(MAKE) clean
	-cd thin_client && $(MAKE) clean
	-cd www && $(MAKE) clean

distclean:
	-cd parser_query_test && $(MAKE) distclean
	-cd parser_query && $(MAKE) distclean
	-cd thin_client && $(MAKE) distclean
	-cd www && $(MAKE) distclean

stageclean:
	-cd parser_query_test && $(MAKE) stageclean
	-cd parser_query && $(MAKE) stageclean
	-cd thin_client && $(MAKE) stageclean
	-cd www && $(MAKE) stageclean
