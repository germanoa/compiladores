rm -f output/etapa7/saida*
FILEDIR=output/etapa7/saida
for i in `ls iloc_parser/input/teste/`; do 
  for j in `seq 1 10`; do 
    for k in `seq 1 10`; do 
      echo "#################" >> $FILEDIR-$i;
      echo "window $j" >> $FILEDIR-$i;
      echo "count $k" >> $FILEDIR-$i;
      echo "## BEFORE:" >> $FILEDIR-$i;
      cat iloc_parser/input/teste/$i >> $FILEDIR-$i;
      echo "## AFTER:" >> $FILEDIR-$i;
      build/main iloc_parser/input/teste/$i $j $k >> $FILEDIR-$i;
      echo "#################" >> $FILEDIR-$i;
      sed '/^ *$/d' $FILEDIR-$i > /tmp/file;
      cp /tmp/file $FILEDIR-$i;
    done;
  done;
done
