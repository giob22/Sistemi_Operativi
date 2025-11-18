all: publish

publish:
	cd ../ &&\
	git add .
	git commit -m "update" &&\
	git push &&\
	mdbook build &&\
	cp -r ./book/* /home/giovanni/Desktop/uni_ubuntu/SistemiOperativi/Appunti/syscall_fork_exec &&\
	cd /home/giovanni/Desktop/uni_ubuntu/SistemiOperativi/Appunti/syscall_fork_exec &&\
	git add . &&\
	git commit -m "aggiornamento automatico per appunti" &&\
	git push &&\
	cd ~/Desktop/SO/Sistemi_Operativi/syscall-fork-exec