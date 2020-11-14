#!/bin/bash
path=$(pwd)
path=${path:21}
echo $path
echo "" > tmp.txt
cd openflow
#cd tmp
if [ -f .gitignore ]; then
path=$(pwd)
path=${path:21}
  while read line 
  do
	#echo $path"/"$line >> ~/3-2/.gitignore
	echo $path"/"$line >> ../tmp.txt 
  done < .gitignore 
fi
cd ../oflops

if [ -f .gitignore ]; then
path=$(pwd)
path=${path:21}
  while read line 
  do
	#echo $path"/"$line >> ~/3-2/.gitignore
	echo $path"/"$line >> ../tmp.txt 
  done < .gitignore 
fi
cd ../oftest 

if [ -f .gitignore ]; then
path=$(pwd)
path=${path:21}
  while read line 
  do
	#echo $path"/"$line >> ~/3-2/.gitignore
	echo $path"/"$line >> ../tmp.txt 
  done < .gitignore 
fi
cd ..
