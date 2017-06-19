#!/bin/sh

git commit -a -m "Documentation update $1"
git push origin master

cd docs
make html

cd ../../pyrosim-docs/html
git add .
git commit -m "Committed Documentation $1"
git push origin gh-pages
