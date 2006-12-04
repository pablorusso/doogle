all:
	-cd parser_query  && $(MAKE) stage
	-cd relative_file && $(MAKE) stage
	-cd math_index    && $(MAKE) stage
	-cd indexer       && $(MAKE) stage
	-cd search_engine && $(MAKE) stage
	-cd thin_client   && $(MAKE) stage
	-cd www           && $(MAKE) stage
