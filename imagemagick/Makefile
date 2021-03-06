# Makefile to build and run the examples using KLEE.
#
# Copyright 2016, 2017 National University of Singapore

IMAGEMAGICK_DIR=${CURDIR}/ImageMagick-7.0.5-9
IMAGEMAGICK_LLVM_DIR=${IMAGEMAGICK_DIR}/obj-llvm
IMAGEMAGICK_COV_DIR=${IMAGEMAGICK_DIR}/obj-cov
IMAGEMAGICK_BITCODE_DIR=${IMAGEMAGICK_LLVM_DIR}/utilities/.libs
IMAGEMAGICK_BITCODE=magick-whole.bc
IMAGEMAGICK_TARGET=${IMAGEMAGICK_BITCODE_DIR}/${IMAGEMAGICK_BITCODE}
# Note that we are excluding magick itself and magick-script
IMAGEMAGICK_UTILS=animate \
	compare \
	composite \
	conjure \
	convert \
	display \
	identify \
	import \
	mogrify \
	montage \
	stream

# The following is for computing coverage from the test files within
# the start of the execution up to, and not including the specified
# time.  Empty means no max time set for computing coverage of test,
# so all tests will be considered in computing coverage.
IMAGEMAGICK_COV_MAX_TIME=

CFLAGS_LLBMC_IMAGEMAGICK=-g -I${IMAGEMAGICK_LLVM_DIR}/lib -I${IMAGEMAGICK_DIR}/src -I${IMAGEMAGICK_DIR}/lib
LLBMC_OPTIONS=--no-div-by-zero-checks \
	--no-overflow-checks \
	--no-shift-checks \
	--no-memcpy-disjoint-checks \
	--ignore-missing-function-bodies \
	--function-name=main \
	--max-loop-iterations=5 \
	--max-function-call-depth=10

BASH=/bin/bash

TARGETS=$(patsubst %,%.tx,${IMAGEMAGICK_UTILS})

DEFAULT_OPTIONS=--sym-args 0 1 10 --sym-args 0 1 2 --sym-files 1 8 --sym-stdout
COMPARE_OPTIONS=--sym-files 2 8 --sym-args 0 1 10 --sym-args 0 1 2 --sym-stdout
COMPOSITE_OPTIONS=--sym-args 0 1 10 --sym-args 0 1 2 --sym-files 3 8 --sym-stdout
CONJURE_OPTIONS=--sym-args 0 1 10 --sym-files 1 8 --sym-stdout
CONVERT_OPTIONS=--sym-args 0 1 10 --sym-args 0 1 2 --sym-file 0 1 8 --sym-args 0 1 10 --sym-args 0 1 2 --sym-file 0 1 2 --sym-stdout
MAGICK_OPTIONS=--sym-arg 7 --sym-args 0 1 10 --sym-args 0 1 2 --sym-files 1 8 --sym-stdout
MAGICK_SCRIPT_OPTIONS=--sym-stdin 16 --sym-stdout
MONTAGE_OPTIONS=--sym-files 1 8 --sym-args 0 1 10 --sym-args 0 1 2 --sym-file 1 8 --sym-stdout
STREAM_OPTIONS=--sym-args 0 1 10 --sym-args 0 1 2 --sym-files 2 8 --sym-stdout

# Note that we are excluding magick itself and magick-script
DEFAULT_EXPERIMENT_SET=${IMAGEMAGICK_UTILS}

DEFAULT_EXPERIMENT_TYPE_SET=.klee1 .klee2 .tx1 .tx2 .tx3 .tx4 .tx5 .tx6 .klee11 .tx11 .tx12 .tx13 .tx14

EXPERIMENT_CSV=experiment.csv

EXPERIMENT_TIMEOUT=10

EXPERIMENT_MEMORY=13000

# From http://klee.github.io/docs/coreutils-experiments/ with
# --search, --max-time, and --max-memory excluded, to be defined in
# the targets. These we originally --max-time=3600. and
# --max-memory=1000, respectively.
KLEE_IMAGEMAGICK_BASIC_OPTIONS=--simplify-sym-indices --write-cvcs --write-cov --output-module \
	--disable-inlining --optimize --use-forked-solver \
	--use-cex-cache --posix-runtime \
	--allow-external-sym-calls \
	--max-sym-array-size=4096 --max-instruction-time=30. \
	--watchdog --max-memory-inhibit=false --max-static-fork-pct=1 \
	--max-static-solve-pct=1 --max-static-cpfork-pct=1 --switch-type=internal \
	--randomize-fork \
	--use-batching-search --batch-instructions=10000

# The defaults from http://klee.github.io/docs/coreutils-experiments/
KLEE_IMAGEMAGICK_DEFAULT_OPTIONS=--search=random-path --search=nurs:covnew -only-output-states-covering-new

LLBMC_EXPERIMENT_CSV=llbmc-${EXPERIMENT_CSV}

# The first target (default)
all: ${TARGETS} ${KLEE_TARGETS} ${STPKLEE_TARGETS}

include ../Makefile.common

build: ${IMAGEMAGICK_TARGET} ${IMAGEMAGICK_COV_DIR} test.env

test.env:
	env -i ${BASH} -c '( source testing-env.sh ; env > test.env )'

${IMAGEMAGICK_TARGET}:
	@echo =================================================================
	@echo Building ImageMagick
	@echo =================================================================
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib/x86_64-linux-gnu
	rm -rf ${IMAGEMAGICK_LLVM_DIR}
	mkdir ${IMAGEMAGICK_LLVM_DIR}
	( cd ${IMAGEMAGICK_LLVM_DIR}; \
		export LLVM_COMPILER=clang; \
		export PATH=${EXTRA_PATH}:$$PATH; \
		export CC=${WHOLE_PROGRAM_LLVM}/wllvm; \
		export CXX=${WHOLE_PROGRAM_LLVM}/wllvm++; \
		unset C_INCLUDE_PATH; \
		unset CPLUS_INCLUDE_PATH; \
		../configure CFLAGS="-g" ; \
		make \
	)
	@echo =================================================================
	@echo Creating whole-program bitcode file for magick
	@echo =================================================================
	( cd ${IMAGEMAGICK_BITCODE_DIR} ; \
		${WHOLE_PROGRAM_LLVM}/extract-bc -m magick ; \
		llvm-link magick.bc ../../MagickCore/.*.o.bc ../../coders/.*.o.bc > ${IMAGEMAGICK_BITCODE} ; \
		for PROGRAM in ${IMAGEMAGICK_UTILS} ; do \
			ln -sf ${IMAGEMAGICK_BITCODE} $$PROGRAM ; \
		done \
	)

${IMAGEMAGICK_COV_DIR}:
	@echo =================================================================
	@echo Building ImageMagick with coverage profiling ...
	@echo =================================================================
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib/x86_64-linux-gnu
	rm -rf ${IMAGEMAGICK_COV_DIR}	
	mkdir ${IMAGEMAGICK_COV_DIR}
	( cd ${IMAGEMAGICK_COV_DIR}; \
		export PATH=${EXTRA_PATH}:$$PATH; \
		export CC=clang; \
		export CXX=clang++; \
		unset C_INCLUDE_PATH; \
		unset CPLUS_INCLUDE_PATH; \
		../configure CFLAGS="-g -fprofile-arcs -ftest-coverage" ; \
		make \
	)
	( cd ${IMAGEMAGICK_COV_DIR}/utilities/.libs ; \
		for PROGRAM in ${IMAGEMAGICK_UTILS} ; do \
			ln -sf magick $$PROGRAM ; \
		done \
	)

%.klee1 : 
	KLEE_IMAGEMAGICK_OPTIONS="--search=dfs -only-output-states-covering-new" OUTPUT_DIR=${CURDIR}/$@ make $*.klee

%.klee2 : 
	KLEE_IMAGEMAGICK_OPTIONS="--search=random-path --search=nurs:covnew -only-output-states-covering-new" OUTPUT_DIR=${CURDIR}/$@ make $*.klee

%.tx1 : 
	KLEE_IMAGEMAGICK_OPTIONS="--search=dfs -only-output-states-covering-new" OUTPUT_DIR=${CURDIR}/$@ make $*.tx

%.tx2 : 
	KLEE_IMAGEMAGICK_OPTIONS="--search=dfs -max-subsumption-failure=3 -only-output-states-covering-new" OUTPUT_DIR=${CURDIR}/$@ make $*.tx

%.tx3 : 
	KLEE_IMAGEMAGICK_OPTIONS="--search=random-path --search=nurs:covnew -only-output-states-covering-new" OUTPUT_DIR=${CURDIR}/$@ make $*.tx

%.tx4 : 
	KLEE_IMAGEMAGICK_OPTIONS="--search=random-path --search=nurs:covnew -max-subsumption-failure=3 -only-output-states-covering-new" OUTPUT_DIR=${CURDIR}/$@ make $*.tx

%.tx5 : 
	KLEE_IMAGEMAGICK_OPTIONS="--search=dfs -exact-address-interpolant -only-output-states-covering-new" OUTPUT_DIR=${CURDIR}/$@ make $*.tx

%.tx6 : 
	KLEE_IMAGEMAGICK_OPTIONS="--search=dfs -max-subsumption-failure=3 -exact-address-interpolant -only-output-states-covering-new" OUTPUT_DIR=${CURDIR}/$@ make $*.tx

# The following targets are run with -write-paths and without
# -only-output-states-covering-new for comparing tree sizes using
# utils/treecmp.c.
%.klee11 : 
	KLEE_IMAGEMAGICK_OPTIONS="--search=dfs -write-paths" OUTPUT_DIR=${CURDIR}/$@ make $*.klee

%.tx11 :
	KLEE_IMAGEMAGICK_OPTIONS="--search=dfs -write-paths" OUTPUT_DIR=${CURDIR}/$@ make $*.klee

%.tx12 :
	KLEE_IMAGEMAGICK_OPTIONS="--search=dfs -max-subsumption-failure=3 -write-paths" OUTPUT_DIR=${CURDIR}/$@ make $*.tx

%.tx13 : 
	KLEE_IMAGEMAGICK_OPTIONS="--search=dfs -exact-address-interpolant -write-paths" OUTPUT_DIR=${CURDIR}/$@ make $*.tx

%.tx14 : 
	KLEE_IMAGEMAGICK_OPTIONS="--search=dfs -max-subsumption-failure=3 -exact-address-interpolant -write-paths" OUTPUT_DIR=${CURDIR}/$@ make $*.tx

###############################################################################
# The following targets can be used for running experiments. The
# conducted experiments involved different option settings of KLEE and
# Tracer-X which can be seen in %.klee... and %.tx... targets.
###############################################################################
%.tx:
	if [ -z "$$OUTPUT_DIR" ] ; then \
		OUTPUT_DIR=${CURDIR}/$@ ; \
	fi ; \
	if [ -z "$$KLEE_IMAGEMAGICK_OPTIONS" ] ; then \
		KLEE_IMAGEMAGICK_OPTIONS="${KLEE_IMAGEMAGICK_DEFAULT_OPTIONS}" ; \
	fi ; \
	KLEE_IMAGEMAGICK_OPTIONS="$$KLEE_IMAGEMAGICK_OPTIONS --libc=uclibc --max-time=${EXPERIMENT_TIMEOUT} --max-memory=${EXPERIMENT_MEMORY}" SOLVER_FLAGS="-solver-backend=z3" OUTPUT_DIR=$$OUTPUT_DIR BITCODE_DIR=${IMAGEMAGICK_BITCODE_DIR} make $*.klee-out

%.klee:
	if [ -z "$$OUTPUT_DIR" ] ; then \
		OUTPUT_DIR=${CURDIR}/$@ ; \
	fi ; \
	if [ -z "$$KLEE_IMAGEMAGICK_OPTIONS" ] ; then \
		KLEE_IMAGEMAGICK_OPTIONS="${KLEE_IMAGEMAGICK_DEFAULT_OPTIONS}" ; \
	fi ; \
	KLEE_IMAGEMAGICK_OPTIONS="$$KLEE_IMAGEMAGICK_OPTIONS --libc=uclibc --max-time=${EXPERIMENT_TIMEOUT} --max-memory=${EXPERIMENT_MEMORY}" SOLVER_FLAGS="-solver-backend=z3 -no-interpolation" OUTPUT_DIR=$$OUTPUT_DIR BITCODE_DIR=${IMAGEMAGICK_BITCODE_DIR} make $*.klee-out

%.stpklee:
	if [ -z "$$OUTPUT_DIR" ] ; then \
		OUTPUT_DIR=${CURDIR}/$@ ; \
	fi ; \
	if [ -z "$$KLEE_IMAGEMAGICK_OPTIONS" ] ; then \
		KLEE_IMAGEMAGICK_OPTIONS="${KLEE_IMAGEMAGICK_DEFAULT_OPTIONS}" ; \
	fi ; \
	KLEE_IMAGEMAGICK_OPTIONS="$$KLEE_IMAGEMAGICK_OPTIONS --libc=uclibc --max-time=${EXPERIMENT_TIMEOUT} --max-memory=${EXPERIMENT_MEMORY}" SOLVER_FLAGS="-solver-backend=stp" OUTPUT_DIR=$$OUTPUT_DIR BITCODE_DIR=${IMAGEMAGICK_BITCODE_DIR} make $*.klee-out

%.klee-out: build
	# The testing sandbox reconstructed here is from
	# http://www.doc.ic.ac.uk/~cristic/klee/klee-cu-sandbox.html
	@echo =================================================================
	@echo Creating the sandbox ...
	@echo =================================================================
	mkdir sandbox
	( cd sandbox ; \
		ln -s /dev/null a ; \
		ln -s /dev/random b ; \
		echo "#!/bin/sh" > c ; \
		echo "echo \"Hello world!\"" >> c ; \
		chmod +x c ; \
		touch d ; \
		mkdir e \
	)
	@echo =================================================================
	@echo Running KLEE on $* ...
	@echo =================================================================
	if [ -z "$$PROGRAM_OPTIONS" ] ; then \
		if [ $* = "compare" ]; then \
			PROGRAM_OPTIONS="${COMPARE_OPTIONS}" ; \
		elif [ $* = "composite" ]; then \
			PROGRAM_OPTIONS="${COMPOSITE_OPTIONS}" ; \
		elif [ $* = "conjure" ]; then \
			PROGRAM_OPTIONS="${CONJURE_OPTIONS}" ; \
		elif [ $* = "convert" ]; then \
			PROGRAM_OPTIONS="${CONVERT_OPTIONS}" ; \
		elif [ $* = "magick" ]; then \
			PROGRAM_OPTIONS="${MAGICK_OPTIONS}" ; \
		elif [ $* = "magick-script" ]; then \
			PROGRAM_OPTIONS="${MAGICK_SCRIPT_OPTIONS}" ; \
		elif [ $* = "montage" ]; then \
			PROGRAM_OPTIONS="${MONTAGE_OPTIONS}" ; \
		elif [ $* = "stream" ]; then \
			PROGRAM_OPTIONS="${STREAM_OPTIONS}" ; \
		else \
			PROGRAM_OPTIONS="${DEFAULT_OPTIONS}" ; \
		fi ; \
	fi ; \
	if [ -z "$$OUTPUT_DIR" ] ; then \
		OUTPUT_DIR="${CURDIR}/$@" ; \
	fi ; \
	rm -rf $$OUTPUT_DIR; \
	KLEE_IMAGEMAGICK_EXTRA_OPTIONS="$$KLEE_IMAGEMAGICK_OPTIONS"; \
	if [ -z "$$KLEE_IMAGEMAGICK_OPTIONS" ] ; then \
		KLEE_IMAGEMAGICK_EXTRA_OPTIONS="${KLEE_IMAGEMAGICK_DEFAULT_OPTIONS}" ; \
	fi ; \
	PROGRAM_OPTIONS="$$PROGRAM_OPTIONS -max-fail 1" ; \
	LD_LIBRARY_PATH=${EXTRA_LD_LIBRARY_PATH} time ${KLEE} ${KLEE_IMAGEMAGICK_BASIC_OPTIONS} $$KLEE_IMAGEMAGICK_EXTRA_OPTIONS $$SOLVER_FLAGS --subsumed-test --environ=${CURDIR}/test.env --run-in=sandbox -output-dir=$$OUTPUT_DIR $$BITCODE_DIR/$* $$PROGRAM_OPTIONS -max-fail 1 ; \
	echo -n "$$PROGRAM_OPTIONS" > $$OUTPUT_DIR/ProgramOptions.txt ; \
	# Delete the sandbox
	rm -rf sandbox sandbox.temps
	${CURDIR}/../utils/imagemagickcov.sh "$*" "$$OUTPUT_DIR" ${IMAGEMAGICK_COV_MAX_TIME}
	GCDA_FILES=`find "${IMAGEMAGICK_COV_DIR}" -name "*.gcda"` ; \
	if [ ! -z "$$GCDA_FILES" ] ; then \
		rm -f $$GCDA_FILES ; \
	fi

clean:
	if [ -z "$$LLBMC_OUTPUT_DIR" ] ; then \
		LLBMC_OUTPUT_DIR="${CURDIR}/LLBMC_OUTPUT" ; \
	fi ; \
	rm -rf *.klee* *.tx* *.stpklee* *.klee-out *.llbmc* ${EXPERIMENT_CSV} small-${EXPERIMENT_CSV} ${EXPERIMENT_CSV}.only small-${EXPERIMENT_CSV}.only ${LLBMC_EXPERIMENT_CSV} ${LLBMC_EXPERIMENT_CSV}.tmp $$LLBMC_OUTPUT_DIR sandbox sandbox.temps test.env

realclean: clean
	rm -rf ${IMAGEMAGICK_LLVM_DIR} ${IMAGEMAGICK_COV_DIR}

core-experiment:
	if [ -z "$$EXPERIMENT_SET" ]  ; then \
		EXPERIMENT_SET="${DEFAULT_EXPERIMENT_SET}" ; \
	fi ; \
	if [ -z "$$EXPERIMENT_TYPE_SET" ] ; then \
		EXPERIMENT_TYPE_SET="${DEFAULT_EXPERIMENT_TYPE_SET}" ; \
	fi ; \
	for program in $$EXPERIMENT_SET ; \
	do \
		for ext in $$EXPERIMENT_TYPE_SET ; \
		do \
			make $$program$$ext ;    \
		done ; \
	done

${EXPERIMENT_CSV}.only:
	( cd ../utils ; CC=${CC} make treecmp ) 
	echo "Directory,Program Options,klee-stats Time (s),KLEE info Elapsed (s), #instructions,#error,#completed paths,#error paths,#subsumed paths,#program exit paths,Average branching depth of completed paths,Average branching depth of subsumption paths,Average instructions of completed paths, Average instructions of subsumed paths,Icov,Bcov,llvm-cov line coverage,LOC,Time for actual solver calls in subsumption check (ms),Number of solver calls for subsumption check,Number of solver calls for subsumption check that resulted in subsumption failure,Average table entries per subsumption checkpoint,Average solver calls per subsumption check,Number of subsumption checks,KLEE paths saved,Tracer-X and not KLEE paths,KLEE and not Tracer-X paths,KLEE paths before last Tracer-X path (-1 if unreached)" >> $@
	if [ -z "$$EXPERIMENT_SET" ]  ; then \
		EXPERIMENT_SET="${DEFAULT_EXPERIMENT_SET}" ; \
	fi ; \
	if [ -z "$$EXPERIMENT_TYPE_SET" ] ; then \
		EXPERIMENT_TYPE_SET="${DEFAULT_EXPERIMENT_TYPE_SET}" ; \
	fi ; \
	for ext in $$EXPERIMENT_TYPE_SET ; \
	do \
		for program in $$EXPERIMENT_SET ; \
		do \
			if [ -d $$program$$ext ]; then \
				printf "$$program$$ext," >> $@; \
				cat $$program$$ext/ProgramOptions.txt >> $@; \
				printf ", " >> $@; \
				klee-stats $$program$$ext | sed '4q;d' | cut -d '|' -f4  | tr -d '\n' >> $@; \
				printf ", " >> $@; \
				grep "Elapsed:" ${CURDIR}/$$program$$ext/info | cut -d' ' -f2 |  sed 's/:/ /g;' | awk '{print $$3" "$$2" "$$1}' | awk '{print $$1+$$2*60+$$3*3600}' | tr -d '\n' >> $@; \
				printf ", " >> $@; \
				grep "KLEE: done: total instructions" ${CURDIR}/$$program$$ext/info | sed -E  's/(.*)=(.*)/\2/'| tr -d '\n' >> $@; \
				printf ", " >> $@; \
				echo -n `ls $$program$$ext | grep err | wc -l` >> $@ ; \
				printf ", " >> $@; \
				grep "KLEE: done: completed paths" ${CURDIR}/$$program$$ext/info | sed -E  's/(.*)=(.*)/\2/' | tr -d ', among which\n' >> $@; \
				printf ", " >> $@; \
				grep "KLEE: done:     error paths" ${CURDIR}/$$program$$ext/info | sed -E  's/(.*)=(.*)/\2/' | tr -d '\n' >> $@; \
				printf ", " >> $@; \
				grep "KLEE: done:     subsumed paths" ${CURDIR}/$$program$$ext/info | sed -E  's/(.*)=(.*)/\2/' | tr -d '\n' >> $@; \
				printf ", " >> $@; \
				grep "KLEE: done:     program exit paths" ${CURDIR}/$$program$$ext/info | sed -E  's/(.*)=(.*)/\2/' | tr -d '\n' >> $@; \
				printf ", " >> $@; \
				grep "average branching depth of completed paths" ${CURDIR}/$$program$$ext/info | sed -E  's/(.*)=(.*)/\2/' | tr -d '\n' >> $@; \
				printf ", " >> $@; \
				grep "average branching depth of subsumed paths" ${CURDIR}/$$program$$ext/info | sed -E  's/(.*)=(.*)/\2/' | tr -d '\n' >> $@; \
				printf ", " >> $@; \
				grep "average instructions of completed paths" ${CURDIR}/$$program$$ext/info | sed -E  's/(.*)=(.*)/\2/' | tr -d '\n' >> $@; \
				printf ", " >> $@; \
				grep "average instructions of subsumed paths" ${CURDIR}/$$program$$ext/info | sed -E  's/(.*)=(.*)/\2/' | tr -d '\n' >> $@; \
				printf ", " >> $@; \
				klee-stats $$program$$ext | sed '4q;d' | cut -d '|' -f5 | tr -d '\n' >> $@; \
				printf ", " >> $@; \
				klee-stats $$program$$ext | sed '4q;d' | cut -d '|' -f6 | tr -d '\n' >> $@; \
				printf ", " >> $@; \
				if [ -e $$program$$ext/LcovLog.txt ]; then \
					cat $$program$$ext/LcovLog.txt | tr -d '\n' >> $@; \
				fi ; \
				printf ", " >> $@; \
				if [ -e $$program$$ext/SLocCountLog.txt ]; then \
					cat ${CURDIR}/$$program$$ext/SLocCountLog.txt | tr -d '\n' >> $@; \
				fi ; \
				printf ", " >> $@; \
				grep "Time for actual solver calls in subsumption check (ms)" ${CURDIR}/$$program$$ext/info | sed -E  's/(.*)=(.*)/\2/' | tr -d '\n' >> $@; \
				printf ", " >> $@; \
				grep "Number of solver calls for subsumption check" ${CURDIR}/$$program$$ext/info | sed -E  's/(.*)=(.*)/\2/' | sed -E  's/(.*)\((.*)/\1/'| tr -d '\n' >> $@; \
				printf ", " >> $@; \
				grep "Number of solver calls for subsumption check" ${CURDIR}/$$program$$ext/info | sed -E  's/(.*)=(.*)/\2/' | sed -E  's/(.*)\((.*)/\2/' | tr -d ')\n' >> $@; \
				printf ", " >> $@; \
				grep "Average table entries per subsumption checkpoint" ${CURDIR}/$$program$$ext/info | sed -E  's/(.*)=(.*)/\2/' | tr -d '\n' >> $@; \
				printf ", " >> $@; \
				grep "Average solver calls per subsumption check" ${CURDIR}/$$program$$ext/info | sed -E  's/(.*)=(.*)/\2/' | tr -d '\n' >> $@; \
				printf ", " >> $@; \
				grep "KLEE: done:     Number of subsumption checks" ${CURDIR}/$$program$$ext/info | sed -E  's/(.*)=(.*)/\2/' | tr -d '\n' >> $@; \
				printf ", " >> $@; \
				if [ $$ext = .tx11 -o $$ext = .tx12 -o $$ext = .tx13 -o $$ext = .tx14 ] ;  then \
					( ../utils/treecmp $$program.klee11 $$program$$ext | sed -e 's/\s/\,/g'  ) >> $@ ; \
				else \
					printf "\n" >> $@; \
				fi ; \
			fi ; \
		done ; \
	done
	rm -f small-$@
	cut --delimiter=, --fields="1,3,5-10,15-18" $@ > small-$@

# This target is used for collecting data from experimental results
# into experiment.csv and smaller small-experiment.csv with less
# columns.
${EXPERIMENT_CSV}: core-experiment
	make ${EXPERIMENT_CSV}.only
	mv ${EXPERIMENT_CSV}.only ${EXPERIMENT_CSV}
	if [ -e small-${EXPERIMENT_CSV}.only ] ; then \
		rm -f small-${EXPERIMENT_CSV} ; \
		mv small-${EXPERIMENT_CSV}.only small-${EXPERIMENT_CSV} ; \
	fi

# This target is used for running the experiment with LLBMC. Arguments
# in PROGRAM_OPTIONS have lengths matching that of KLEE runs. For
# example, Klee runs with --sym-args 0 1 10 --sym-args 0 1 2 which
# defined in DEFAULT_OPTIONS, means the program is invoked with 0 or 1
# long symbolic argument of length 10 and 0 or 1 short symbolic
# argument of length 2. This is translated into "xxxxxxxxxx xx".
#
# LLBMC uses STP as backend, so we do 'ulimit -s unlimited' as
# recommended in http://klee.github.io/build-stp/
%.llbmc :
	( \
		if [ -z "$$PROGRAM_OPTIONS" ] ; then \
			if [ $* = "compare" ]; then \
				PROGRAM_OPTIONS="x x xxxxxxxxxx xx" ; \
			elif [ $* = "composite" ]; then \
				PROGRAM_OPTIONS="xxxxxxxxxx xx x x x" ; \
			elif [ $* = "conjure" ]; then \
				PROGRAM_OPTIONS="xxxxxxxxxx x" ; \
			elif [ $* = "convert" ]; then \
				PROGRAM_OPTIONS="xxxxxxxxxx xx x" ; \
			elif [ $* = "magick" ]; then \
				PROGRAM_OPTIONS="xxxxxxx xxxxxxxxxx xx x" ; \
			elif [ $* = "magick-script" ]; then \
				PROGRAM_OPTIONS="" ; \
			elif [ $* = "montage" ]; then \
				PROGRAM_OPTIONS="x xxxxxxxxxx xx x" ; \
			elif [ $* = "stream" ]; then \
				PROGRAM_OPTIONS="xxxxxxxxxx xx x x" ; \
			else \
				PROGRAM_OPTIONS="xxxxxxxxxx xx x" ; \
			fi ; \
		fi ; \
		if [ -z "$$LLBMC_OUTPUT_DIR" ]; then \
			LLBMC_OUTPUT_DIR="${CURDIR}/LLBMC_OUTPUT" ; \
		fi ; \
		if [ ! -d "$$LLBMC_OUTPUT_DIR" ]; then \
			mkdir "$$LLBMC_OUTPUT_DIR" ; \
		fi ; \
		if [ -z "$$LLBMC_OPTIONS" ] ; then \
			if [ -z "${LLBMC_OPTIONS}" ] ; then \
				LLBMC_OPTIONS="${LLBMC_DEFAULT_OPTIONS}" ; \
			else \
				LLBMC_OPTIONS="${LLBMC_OPTIONS}" ; \
			fi ; \
		fi ; \
		if [ -z "${EXPERIMENT_TIMEOUT}" ] ; then \
			LLBMC_TIMEOUT="${LLBMC_DEFAULT_TIMEOUT}" ; \
		else \
			LLBMC_TIMEOUT="${EXPERIMENT_TIMEOUT}" ; \
		fi ; \
		ulimit -s unlimited ; \
		PATH=${EXTRA_PATH}:$$PATH timeout $$LLBMC_TIMEOUT llbmc $$LLBMC_OPTIONS -arguments="$$PROGRAM_OPTIONS" -smtlib2 ${IMAGEMAGICK_BITCODE_DIR}/$* > "$$LLBMC_OUTPUT_DIR"/$*.smt2 ; \
		PATH=${EXTRA_PATH}:$$PATH timeout $$LLBMC_TIMEOUT llbmc $$LLBMC_OPTIONS -arguments="$$PROGRAM_OPTIONS" ${IMAGEMAGICK_BITCODE_DIR}/$* > "$$LLBMC_OUTPUT_DIR"/$* ; \
                if [ $$? = "124" ] ; then \
                        touch "$$LLBMC_OUTPUT_DIR"/$*.timeout ; \
                fi \
	)

llbmc-core-experiment:
	if [ -z "$$EXPERIMENT_SET" ]  ; then \
		EXPERIMENT_SET="${DEFAULT_EXPERIMENT_SET}" ; \
	fi ; \
	for program in $$EXPERIMENT_SET ; do \
		make $$program.llbmc ; \
	done

# This target is used for collecting data from ImageMagick LLBMC
# experiment into ${LLBMC_EXPERIMENT_CSV} file.  It will create both
# ${LLBMC_EXPERIMENT_CSV} and ${EXPERIMENT_CSV}, the second containing
# data of Tracer-X and KLEE runs on the main function bitcode only,
# for comparison to LLBMC.
${LLBMC_EXPERIMENT_CSV}: llbmc-core-experiment
	if [ -z "$$EXPERIMENT_SET" ] ; then \
		EXPERIMENT_SET=$(subst .tx,,${TARGETS}) ; \
	fi ; \
	make ${LLBMC_EXPERIMENT_CSV}.only

${LLBMC_EXPERIMENT_CSV}.header:
	echo "Program,Timed out,Time for performing transformations on LLVM IR,Time for performing logical encoding,Time for performing formula transformations,Time for running SMT solver (STP with MiniSat), Total time,Error status" >> ${LLBMC_EXPERIMENT_CSV}

${LLBMC_EXPERIMENT_CSV}.body:
	if [ -z "$$EXPERIMENT_SET" ] ; then \
		EXPERIMENT_SET="$(subst .tx,,${TARGETS})" ; \
	fi ; \
	if [ -z "$$LLBMC_OUTPUT_DIR" ] ; then \
		LLBMC_OUTPUT_DIR="${CURDIR}/LLBMC_OUTPUT" ; \
	else \
		LLBMC_OUTPUT_DIR_FILENAME=$$(basename "$$LLBMC_OUTPUT_DIR") ; \
		LLBMC_OUTPUT_DIR_EXTENSION=$${LLBMC_OUTPUT_DIR_FILENAME##*.} ; \
	fi ; \
	for program in $$EXPERIMENT_SET ; \
	do \
		if [ -e "$$LLBMC_OUTPUT_DIR/$$program" ]; then \
			printf "$$program.$$LLBMC_OUTPUT_DIR_EXTENSION," >> ${LLBMC_EXPERIMENT_CSV}; \
                        if [ -e "$$LLBMC_OUTPUT_DIR/$$program.timeout" ] ; then \
                                echo -n "Yes," >> ${LLBMC_EXPERIMENT_CSV} ; \
                        else \
                                echo -n "No," >> ${LLBMC_EXPERIMENT_CSV} ; \
                        fi ; \
			grep "Performing transformations on LLVM IR" "$$LLBMC_OUTPUT_DIR/$$program" | tr -d 'Performing transformations on LLVM IR sec [DONE]\n' |  sed -r 's/^.{3}//' >> ${LLBMC_EXPERIMENT_CSV}; \
			printf ", " >> ${LLBMC_EXPERIMENT_CSV}; \
			grep "Performing logical encoding" "$$LLBMC_OUTPUT_DIR/$$program" | tr -d 'Performing logical encoding sec [DONE]\n' |  sed -r 's/^.{3}//' >> ${LLBMC_EXPERIMENT_CSV}; \
			printf ", " >> ${LLBMC_EXPERIMENT_CSV}; \
			grep "Performing formula transformations" "$$LLBMC_OUTPUT_DIR/$$program" | tr -d 'Performing formula transformations sec [DONE]\n' |  sed -r 's/^.{3}//' >> ${LLBMC_EXPERIMENT_CSV}; \
			printf ", " >> ${LLBMC_EXPERIMENT_CSV}; \
			grep "Running SMT solver (STP with MiniSat)" "$$LLBMC_OUTPUT_DIR/$$program" | tr -d 'Running SMT solver (STP with MiniSat) sec [DONE]\n' |  sed -r 's/^.{3}//' >> ${LLBMC_EXPERIMENT_CSV}; \
			printf ", " >> ${LLBMC_EXPERIMENT_CSV}; \
			awk '/[DONE]/{i++}i==5' "$$LLBMC_OUTPUT_DIR/$$program" | tr -d  "sec [DONE] Result:=\n" >> ${LLBMC_EXPERIMENT_CSV}; \
			printf ", " >> ${LLBMC_EXPERIMENT_CSV}; \
			grep "Error detected." "$$LLBMC_OUTPUT_DIR/$$program" | tr -d  "\n"  >> ${LLBMC_EXPERIMENT_CSV}; \
			printf "\n" >> ${LLBMC_EXPERIMENT_CSV}; \
		fi ; \
	done

${LLBMC_EXPERIMENT_CSV}.only:
	if [ ! -z "$$LLBMC_OUTPUT_DIR_SET" ] ; then \
		rm -f ${LLBMC_EXPERIMENT_CSV} ${LLBMC_EXPERIMENT_CSV}.tmp ; \
		make ${LLBMC_EXPERIMENT_CSV}.header ; \
		mv ${LLBMC_EXPERIMENT_CSV} ${LLBMC_EXPERIMENT_CSV}.tmp ; \
		for LLBMC_OUTPUT_DIR in $$LLBMC_OUTPUT_DIR_SET ; \
		do \
			LLBMC_OUTPUT_DIR_FILENAME=$$(basename "$$LLBMC_OUTPUT_DIR") ; \
			LLBMC_OUTPUT_DIR="$$LLBMC_OUTPUT_DIR" EXPERIMENT_SET="$${LLBMC_OUTPUT_DIR_FILENAME%.*}" make ${LLBMC_EXPERIMENT_CSV}.body ; \
			cat ${LLBMC_EXPERIMENT_CSV} >> ${LLBMC_EXPERIMENT_CSV}.tmp ; \
			rm -f ${LLBMC_EXPERIMENT_CSV} ; \
		done ; \
		mv ${LLBMC_EXPERIMENT_CSV}.tmp ${LLBMC_EXPERIMENT_CSV} ; \
	else \
		make ${LLBMC_EXPERIMENT_CSV}.header ; \
		mv ${LLBMC_EXPERIMENT_CSV} ${LLBMC_EXPERIMENT_CSV}.tmp ; \
		make ${LLBMC_EXPERIMENT_CSV}.body ; \
		cat ${LLBMC_EXPERIMENT_CSV} >> ${LLBMC_EXPERIMENT_CSV}.tmp ; \
		rm -f ${LLBMC_EXPERIMENT_CSV} ; \
		mv ${LLBMC_EXPERIMENT_CSV}.tmp ${LLBMC_EXPERIMENT_CSV} ; \
	fi
