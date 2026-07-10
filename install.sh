#!/bin/bash

DIR="$(dirname "$0")"
BIN="$DIR/bin/cerrusv"
CFG_FILE="cerrucfg.toml"
PATH_CFG_FILE="/etc/cerrucfg.toml"
SERVICEF_PATH="/etc/systemd/system/"
SERVICEF="cerrusv.service"

echo "### Instalando... ###"

if [ ! -f "$BIN" ]; then
    echo "[ x ] No se encontro el ejecutable en: $BIN"
    exit 1
fi

echo "Instalando ejecutable"
cp "$BIN" /usr/local/bin/
#chmod +x /usr/local/bin/cerrusv

if [ ! -f "$CFG_FILE" ]; then
    echo "[ x ] No existe el archvo de configuracion"

elif [ ! -f "$PATH_CFG_FILE" ]; then
    echo "Copiando archivo de configuracion"
    cp "$CFG_FILE" "$PATH_CFG_FILE"
else
    echo "Archivo de configuracion existente en: $PATH_CFG_FILE"
fi

echo "Binario instalado en /usr/local/bin/cerrusv"

echo "Instalndo archivo de servicio..."

if [ ! -f "$SERVICEF" ]; then
	echo "[ x ] No existe el archivo de servicio"
else
	cp "$SERVICEF" "$SERVICEF_PATH"
	echo "Archivo de servicio instalado"
fi

echo "Listo!"
