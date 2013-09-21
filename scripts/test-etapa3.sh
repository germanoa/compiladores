


scripts/compila.sh

for i in `ls -1 input/etapa3/i*.iks |cut -d '/' -f3 |sed -e 's/\..*$//'`
do
    build/main < input/etapa3/$i.iks
    mv output/etapa3/saida.dot output/etapa3/$i.dot
    dot output/etapa3/$i.dot -Tpng -o output/etapa3/$i.png
    display output/etapa3/$i.png
done

rm -f output/etapa3/*

#VAI=$1
#build/main < input/etapa3/$VAI.iks
#mv output/etapa3/saida.dot output/etapa3/$VAI.dot 
#dot output/etapa3/$VAI.dot -Tpng -o output/etapa3/png/$VAI.png
#display output/etapa3/png/$VAI.png &
#display input/etapa3/$VAI.dot.png 
