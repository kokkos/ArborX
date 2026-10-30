SPHINXBUILD ?= sphinx-build

.PHONY: html clean

html: clean
	$(SPHINXBUILD) -b html ./source/ ./generated_docs/ -W --keep-going
	python3 ./source/edit_button_handler.py

clean:
	rm -rf ./generated_docs/
