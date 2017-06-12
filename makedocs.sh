#!/bin/sh

cd docs
make html

cd ../../pyrosim-docs/html
git add .
git commit -m "Committed Documentation $1"
git push origin gh-pages
