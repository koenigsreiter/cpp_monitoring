EXECUTABLES = plantuml pandoc
K := $(foreach exec,$(EXECUTABLES),\
        $(if $(shell which $(exec)), "$(exec) is installed",$(error "No $(exec) in PATH. Please install $(exec) first")))
FILENAME = koenigsreiter
FILES = $(FILENAME).md images.md

all: check umls tex pdf

tex: umls
	pandoc -s -o $(FILENAME).tex $(FILES)

pdf: umls
	pandoc -s -o $(FILENAME).pdf $(FILES)

check:
	mkdir -p compiled_umls

umls:
	plantuml -o ../compiled_umls UMLs/*

clean:
	test -d compiled_umls && rm -rf compiled_umls
	test -f $(FILENAME).tex && rm $(FILENAME).tex
	test -f $(FILENAME).pdf && rm $(FILENAME).pdf