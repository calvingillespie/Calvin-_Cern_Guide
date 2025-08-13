#!/bin/env bash

echo "Installing python dependencies..."
pip install plotly --user
pip install chart_studio --user
pip install python-dateutil --user
pip install cx_oracle --user
pip install psycopg2-binary --user

# package dependecy "decorator" doesn't get installed properly on lxplus, workaround:
if [[ $HOSTNAME == *"lxplus"* ]]; then
    echo "Installing dependencies for lxplus..."
    wget https://pypi.python.org/packages/bb/e0/f6e41e9091e130bf16d4437dabbac3993908e4d6485ecbc985ef1352db94/decorator-4.1.2.tar.gz
    tar -xf decorator-4.1.2.tar.gz
    cd decorator-4.1.2/
    python setup.py install --user
    cd -
    rm decorator-4.1.2.tar.gz
fi

chmod +x *py cgi-bin/*py

# needed for simple httpd listen.py
ln -s ./html/index.html index.html
ln -s ./html/res res
