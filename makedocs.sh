#!/bin/sh

DATE=$(date +%Y-%m-%d)
echo "Documentation update $DATE"

cd docs
make html

cd ../../pyrosim-docs/html
git add .
git commit -m "Documentation update $DATE"
git push origin gh-pages
