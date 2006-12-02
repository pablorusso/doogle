#include "Indexer.h"

Indexer::Indexer()
{
	_lexical  = new ArchivoLexico( "lexical.dat", ESCRIBIR | LEER );
	_document = new ArchivoDocumentos( "documents.dat" );
	_docLex   = new ArchivoDocLexico( "documentLexicalTmp.dat" );

	// TODO: Parametrizar empiricamente...
	DeltaCosineForEqual = 0.2;
}

int Indexer::fileFilter( const struct dirent *entry )
{
	if ( (strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0) )
		return 0;
	else
		return -1;
}

void Indexer::buildLeaders()
{
	// ver como se calcula esto
	int maxLeaders   = docCount / 2;
	int maxFollowers = docCount / 2;

	DocumentLexical leaders("leaders.dat");
	DocumentLexical followers("followers.dat");
	DocumentLexical withoutLeader("noleader.dat");

	docLex.StartRead();
	// El primero siempre es lider (porque vienen ordenados por cantidad de lexico distinto)
	if ( DocumentLexicalData item = docLex.Read() )
		learders.AddRecord( item );

	// Para cada documento
	while ( DocumentLexicalData item = docLex.Read() )
	{
		DocumentLexicalData bestLeader = NULL;
		bool isFollower = false;
		float lessRank  = 0;


		leaders.StartRead();
		// Para cada lider
		while ( DocumentLexicalData leader = leaders.Read() && ! isFollower )
		{
			// Comparo usando el ranking del coseno el documento con cada lider
			LexicalPair []leaderPairList = leader.GetLexicalPairList();
			LexicalPair []itemPairList   = item.GetLexicalPairList();
			float rank = cosineRank( itemPairList, item.GetNorm(), item.GetLexicalPairCount(), leaderPairList, leader.GetNorm(), leader.GetLexicalPairCount() );

			if ( rank <= DELTA_EQUAL )
			{
				// Si el rankeo del coseno es suficientemente bueno, lo considero seguidor sin ver que pasa mas adelante
				item.SetLeader( leader.GetId() );
				followers.AddRecord( item );

				leader.SetFollowersQuantity( leader.GetFollowersQuantity() + 1 );
				leaders.updateRecord( leader );

				isFollower = true;
			}
			else
			{
				// Si no es tan bueno, me fijo si al menos es el mejor que encontre y me lo guardo
				if ( rank < lessRank )
				{
					lessRank = rank;
					bestLeader = leader;
				}
			}
		}

		if ( ! isFollower )
		{
			// Si no es seguidor natural de nadie

			if ( bestLeader.GetFollowersQuantity < maxFollowers )
			{
				// Si el leader mas cercano todavia acepta seguidores, entonces lo hago seguidor de este
				item.SetLeader( bestLeader.GetId() );
				followers.AddRecord( item );
				bestLeader.SetFollowersQuantity( bestLeader.GetFollowersQuantity() + 1 );
				leaders.updateRecord( bestLeader );
			}
			else
			{
				if ( leaders.GetCount() < maxLeaders )
					// Si aun tengo espacio para liders, lo hago lider
					leaders.AddRecord( item );
				else
					// Si no puede ser ni seguidor ni lider, entonces no encaja con nada y lo mando al otro archivo
					withoutLeader.AddRecord( item );
			}
		}
	}
}

void Indexer::buildLexical( string path )
{
	int count, i;
	struct direct **files = NULL;
	int fileFilter( const struct dirent *entry );

	cout << endl << "Listando contenido de la carperta: " << path << endl;
    count = scandir ( path.c_str(), &files, fileFilter, NULL);
	if ( count <= 0 )
	{
		cout << "No hay archivos en la carpeta";
		exit(0);
	}

	ArchivoDocLexico docLex( "documentLexicalTmp.dat", ESCRIBIR );
	for ( i=0; i < count; ++i )
	{
		string fileName = path + "/" + files[i]->d_name;

		// Me fijo si es una carpeta
		bool isAFile = true;
		 if( chdir( fileName.c_str() ) == 0 )
			 isAFile = false;

		cout << "Importando lexico desde " << fileName << ": ";
		if ( isAFile )
		{
			DocumentData docData( fileName );
			/*
			docData
			std::string ruta;
        	double      norma;
        	long        cantTermDistintos;
			*/

			ArchivoLexico lex( "lexicalPartition.dat", ESCRIBIR );
			try
			{
				ParserWrapper parser ( fileName, lex );
				parser.Parse();

				std::cout << "Lexico importado con exito" << endl;
			}
			catch ( std::string ex )
			{
				std::cout << ex << endl;
			}

			LexicalPair []mergedItems = lexical.MergeWith( lex );

			int count = lex->GetCount();
			DocumentLexicalData docLexData( docData.GetId(), count );
			/*
			// id - peso typedef map<int, long> LexicalPair;
			// id - offset typedef map<int, long> Seguidores;
			class DocLexicoData
			{
				public:
					int  id;
					Seguidores  seguidores;
					LexicalPair terminos;
			};
			*/
			for( int i = 0; i < count; ++i )
				docLexData.AddLexicalPair( mergedItems[i] );
			docLex.AddRecord( docLexData );
		}
		else
		{
			cout << "Es un directorio, navegando hacia adentro..." << endl;
			parsearPath( fileName );
		}
	}

	docLex.sortByQuantity();
}

void Indexer::buildIndex( string path )
{
	buildLexical( path );
	buildLeaders();
}

Indexer::~Indexer()
{
	delete _lexical;
	delete _document;
	delete _docLex;
}
