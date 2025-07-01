#!/bin/bash

MOD_NAME="vfs_tracer.ko"
MODULE="${MOD_NAME%.ko}"
SRC_LOG="/var/log/vfs_trace.log"
TIMESTAMP=$(date +"%Y%m%d_%H%M%S")
DST_LOG="/var/log/vfs_trace_backup_$TIMESTAMP.log"

# 1. Carrega o m�dulo
echo "[+] Carregando m�dulo $MOD_NAME..."
sudo insmod "$MOD_NAME"
if [ $? -ne 0 ]; then
    echo "[!] Falha ao carregar o m�dulo."
    exit 1
fi

# 2. Espera 5 minutos
echo "[+] M�dulo carregado. Aguardando 5 minutos para captura..."
sleep 60

# 3. Descarrega o m�dulo
echo "[+] Removendo o m�dulo..."
sudo rmmod "$MODULE"
if [ $? -ne 0 ]; then
    echo "[!] Falha ao remover o m�dulo."
    exit 1
fi

# 4. Faz uma c�pia dos logs
if [ -f "$SRC_LOG" ]; then
    echo "[+] Fazendo backup do log para $DST_LOG..."
    sudo cp "$SRC_LOG" "$DST_LOG"
    echo "[+] Backup conclu�do."
else
    echo "[!] Arquivo de log $SRC_LOG n�o encontrado!"
    exit 1
fi
