setup_script=$(top_srcdir)/bindings/setup.py
test_script=$(top_srcdir)/bindings/test.py

all:
	CC=g++ python $(setup_script) build

pymodule-sdist:
	python $(setup_script) sdist

install-exec-local:
	python $(setup_script) install

uninstall-local:
	python $(setup_script) uninstall

clean-local:
	python $(setup_script) clean
	rm -rf build

check-local:
	python $(test_script)
