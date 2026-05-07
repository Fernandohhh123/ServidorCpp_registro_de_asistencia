#!/bin/bash

DIR="$(dirname "$0")"
BIN="$DIR/bin/cerrusv"

if [ ! -f "$BIN" ]; then
    echo "No se encontró el ejecutable en: $BIN"
    exit 1
fi

echo "Instalando ejecutable"
cp "$BIN" /usr/local/bin/
chmod +x /usr/local/bin/cerrusv

echo "Copiando archivo de configuracion"
cp cerrusv.cfg /etc/

echo "Instalado en /usr/local/bin/cerrusv"
