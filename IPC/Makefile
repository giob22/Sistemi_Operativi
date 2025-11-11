all: publish

publish:
	mdbook build &&\
	cp -r ./book/* /home/giovanni/Desktop/uni_ubuntu/SistemiOperativi/IPC_appunti &&\
	cd /home/giovanni/Desktop/uni_ubuntu/SistemiOperativi/IPC_appunti &&\
	git add . &&\
	git commit -m "aggiornamento automatico" &&\
	git push &&\
	cd ~/Desktop/SO/Sistemi_Operativi/IPC