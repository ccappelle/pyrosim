#!/bin/sh

cd docs
make html
make latexpdf

cd ../pyrosim-docs/html
git add .
git commit -m "Documentation commit"
git push origin gh-pages