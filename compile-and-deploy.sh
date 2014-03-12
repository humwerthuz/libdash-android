#!/bin/bash

cd app && make && cd ../lib && make-jar.sh && cd ..
