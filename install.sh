#!/bin/bash

DIR="$(dirname "$0")"
BIN="$DIR/bin/cerrusv"
CFG_FILE="cerrusv.cfg"

if [ ! -f "$BIN" ]; then
    echo "No se encontró el ejecutable en: $BIN"
    exit 1
fi

echo "Instalando ejecutable"
cp "$BIN" /usr/local/bin/
chmod +x /usr/local/bin/cerrusv

if [ ! -f "$CFG_FILE" ]; then
    echo "No existe el archvo de configuracion"
else
    echo "Copiando archivo de configuracion"
    cp cerrusv.cfg /etc/
fi



echo "Instalado en /usr/local/bin/cerrusv"
