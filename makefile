all:
	-cd parser_query  && $(MAKE) all
	-cd relative_file && $(MAKE) all
	-cd math_index    && $(MAKE) all
	-cd indexer       && $(MAKE) all
	-cd thin_client   && $(MAKE) all
	-cd www           && $(MAKE) all

build:
	-cd parser_query  && $(MAKE) build
	-cd relative_file && $(MAKE) build
	-cd math_index    && $(MAKE) build
	-cd indexer       && $(MAKE) build
	-cd thin_client   && $(MAKE) build
	-cd www           && $(MAKE) build

stage:
	-cd parser_query  && $(MAKE) stage
	-cd relative_file && $(MAKE) stage
	-cd math_index    && $(MAKE) stage
	-cd indexer       && $(MAKE) stage
	-cd thin_client   && $(MAKE) stage
	-cd www           && $(MAKE) stage

clean:
	-cd www           && $(MAKE) clean
	-cd thin_client   && $(MAKE) clean
	-cd indexer       && $(MAKE) clean
	-cd math_index    && $(MAKE) clean
	-cd relative_file && $(MAKE) clean
	-cd parser_query  && $(MAKE) clean

distclean:
	-cd www           && $(MAKE) distclean
	-cd thin_client   && $(MAKE) distclean
	-cd indexer       && $(MAKE) distclean
	-cd math_index    && $(MAKE) distclean
	-cd relative_file && $(MAKE) distclean
	-cd parser_query  && $(MAKE) distclean
