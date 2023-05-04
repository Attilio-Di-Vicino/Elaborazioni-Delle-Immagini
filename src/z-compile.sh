#!/bin/bash

# Definizione della funzione di compilazione con OpenCV
function cv_compile() {
  g++ "$1" $(pkg-config --cflags --libs opencv) -o "$2"
}

# Verifica che sia stato specificato almeno un argomento
if [ $# -lt 1 ]; then
  echo "Usage: ./compile_cv.sh <file.cpp> [<output_file_name>]"
  exit 1
fi

# Estrai il nome del file sorgente dal primo argomento
src_file="$1"

# Estrai il nome dell'eseguibile dal secondo argomento, o usa il nome predefinito "cv" se non viene specificato
if [ $# -lt 2 ]; then
  exec_name="a.out"
else
  exec_name="$2"
fi

# Chiamata alla funzione di compilazione con i due argomenti
cv_compile "$src_file" "$exec_name"
