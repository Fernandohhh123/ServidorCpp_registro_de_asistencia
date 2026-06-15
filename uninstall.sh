#!/bin/bash

# Este archivo es para desisntalar el
# softwre y sus archivos asociados

# Borrando el programa
if [ ! -f "/usr/local/bin/cerrusv" ]; then
    echo "No exite el prgrama /usr/local/bin/cerrusv"
else
    rm /usr/local/bin/cerrusv
fi

if [ ! -f "/etc/cerrusv.cfg" ]; then
    echo "No existe el archivo de configuracion /etc/cerrusv.cfg"
else
    rm /etc/cerrusv.cfg
fi

echo "Programa desinstalado correctamente"
