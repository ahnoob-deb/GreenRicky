#!/bin/bash
PROJECT_DIR=GreenRicky
SCRIPT_DIR=$(pwd)
DATE=$(date '+%Y-%m-%d')
BACKUP_DIR=/backup/STICK/PROJECTS/GreenRicky/
NAME=GreenRicky
FILENAME=${NAME}-${DATE}.tar.gz

cd ../..
#du -h ${PROJECT_DIR}
#echo generating backup of ${PROJECT_DIR}: ${FILENAME} ...
tar czfv ${FILENAME} --exclude='bin/*' --exclude='make/obj/*' --exclude='*.o' --exclude='*.d' --exclude='*.x' --exclude='*.out' --exclude='*.mid' --exclude='*~' ${PROJECT_DIR}
mv ${FILENAME} ${BACKUP_DIR}
