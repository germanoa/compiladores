#!/bin/bash
# Este script deve ser executado a partir 
# do diretório raiz do projeto
# Ele recompila o projeto e executa a partir de um 
# dado arquivo iloc em /input/etapa5/

clear

echo "####################################################"
echo "#   ESTE SCRIPT IRA COMPILAR E EXECUTAR A ETAPA 5  #"
echo "#    CODIGO ILOC A PARTIR DE UM PROGRAMA EM IKS    #"
echo "####################################################"
echo " " 

if [$1 == ""]; then
	echo " "
	echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
	echo "!     É PRECISO ESPECIFICAR UM ARQUIVO ILOC        !"
	echo "!            CONTIDO EM input/etapa5/              !"
	echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
	echo " " 
else
	if [ ! -f input/etapa5/$1 ]; then
		echo " "
		echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
		echo "!     O ARQUIVO NÃO FOI ENCONTRADO NO DIRETÓRIO    !"
		echo "!  input/etapa5/. DIGITE APENAS O NOME DO ARQUIVO  !"
		echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
		echo " " 
	
	else
		echo " "
		echo "####################################################"
		echo "# O arquivo de entrada é $1 "
		echo "####################################################"
		echo " " 

		CMAKEFLAGS=" -g3 -DCMAKE_BUILD_TYPE=Debug "

		rm -rf build
		mkdir build
		cd build
		cmake $CMAKEFLAGS ..
		make
		cd ..

		clear

		echo " "
		echo "####################################################"
		echo "#              Programa $1                    #"
		echo "####################################################"
		echo " "
	
		cat input/etapa5/$1

		echo " "
		echo "####################################################"
		echo "#              Gerando o código iloc               #"
		echo "####################################################"
		echo " "
	
		build/main < input/etapa5/$1
		build/main < input/etapa5/$1 > saida.iloc
	
		echo " "
		echo "####################################################"
		echo "#         Mostrando a reresentação da AST          #"
		echo "####################################################"
		echo " "
		
		dot ast_graph.dot -Tpng -o ast_graph.png
		display ast_graph.png
	fi	
fi
	
	


