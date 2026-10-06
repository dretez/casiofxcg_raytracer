.PHONY: all upload push test clean dev-setup

all:
	fxsdk build-cg

upload:
	fxsdk build-cg && fxlink -s *.g3a

push:
	fxsdk build-cg-push -s && fxlink -iw

test:
	fxsdk build-cg && fxlink -s Raytracer.g3a && fxlink -iw

clean:
	rm -r build-cg

dev-setup:
	export PROJECT_ROOT="$(abspath .)" && \
	python3 tools/devenv_setup.py
