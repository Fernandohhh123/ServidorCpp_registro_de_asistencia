#!/bin/bash

DIR="$(dirname "$0")"
BIN="$DIR/bin/cerrusv"

if [ ! -f "$BIN" ]; then
    echo "No se encontró el ejecutable en: $BIN"
    exit 1
fi

sudo cp "$BIN" /usr/local/bin/
sudo chmod +x /usr/local/bin/cerrusv

echo "Instalado en /usr/local/bin/cerrusv"
