#!/bin/sh
mkdir ../deploy;
mkdir ../deploy/stop;
mkdir ../deploy/search_engine;
mkdir ../deploy/www;
mkdir ../deploy/www/images;
mkdir ../deploy/www/lib;
mkdir ../deploy/www/input;
mkdir ../deploy/apache;
mkdir ../deploy/indexer;
mkdir ../deploy/output;

cp test_cases/stop/*.* ../deploy/stop/
cp search_engine/bin/search_engine.out ../deploy/search_engine/search_engine.out
cp www/images/*.* ../deploy/www/images/
cp www/lib/*.* ../deploy/www/lib/
cp test_cases/input/*.* ../deploy/www/input
cp apache/sites-available/default ../deploy/apache/default
cp indexer/bin/indexer ../deploy/indexer/indexer
cp xhtml.sh ../deploy/xhtml.sh

echo "Fin. Vaya a ../deploy"