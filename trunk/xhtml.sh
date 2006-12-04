#!/bin/sh



# Cantidad maxima de terminos por documento
MAX_TERMINOS_DOC=200;

# cuantos documentos generar.
CANT_DOCS=100;

# Cuantos terminos distintos se usan del diccionario.
# si es mayor que los terminos del diccionario no limita nada.
CANT_TERMINOS_TOTALES=10000
#10000

#De donde se sacan las palabras random. Este path es de gentoo
#En debian hay algo en /usr/share/dict y sino apt-get install wenglish
DICCIONARIO="/usr/share/dict/spanish";





CANT_TERMINOS=`wc -l $DICCIONARIO | cut -f1 -d " "`;
p='p'


if [ $CANT_TERMINOS -gt $CANT_TERMINOS_TOTALES ]
then
  CANT_TERMINOS=$CANT_TERMINOS_TOTALES
fi
echo "utilizando $CANT_TERMINOS terminos para generar los documentos"


for i in `seq 1 $CANT_DOCS` ;
do
        echo "Generando el documento $i.html";
        #echo "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\"" > $i.html;
        #echo "\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">" >> $i.html;
        echo "<html><head><title>" >> $i.html;
        let nrolinea=($RANDOM%$CANT_TERMINOS)+1;

	#echo "linea $nrolinea palabra: "
	#echo `sed -n $nrolinea$p $DICCIONARIO | sed 's/#$//' | tr -d "'áéíóúñÑÁÉÍÓÚüÜ" `;

	echo `sed -n $nrolinea$p $DICCIONARIO | sed 's/#$//' | tr "'áéíóúñÑÁÉÍÓÚüÜ" "'aeiounNAEIOUuU" ` | tr -d "'" >> $i.html;
        echo "</title></head><body><div>" >> $i.html;

        let CANT_TERMINOS_DOC=$RANDOM%$MAX_TERMINOS_DOC;

        for j in `seq 1 $CANT_TERMINOS_DOC` ;
        do
                let nrolinea=($RANDOM%$CANT_TERMINOS)+1;

		#echo "linea $nrolinea palabra: "
		#echo `sed -n $nrolinea$p $DICCIONARIO | sed 's/#$//' | tr -d "'áéíóúñÑÁÉÍÓÚüÜ" `;

		echo `sed -n $nrolinea$p $DICCIONARIO | sed 's/#$//' | tr "'áéíóúñÑÁÉÍÓÚüÜ" "'aeiounNAEIOUuU" | tr -d "'" ` >> $i.html;
        done
        echo "</div></body></html>" >> $i.html;

done
