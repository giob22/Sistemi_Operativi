all: publish

publish:
	cd ../ &&\
	git add .
	git commit -m "update" &&\
	git push &&\
	mdbook build &&\
	cp -r ./book/* /home/giovanni/Desktop/uni_ubuntu/SistemiOperativi/Appunti/IPC_appunti &&\
	cd /home/giovanni/Desktop/uni_ubuntu/SistemiOperativi/Appunti/IPC_appunti &&\
	git add . &&\
	git commit -m "aggiornamento automatico per appunti" &&\
	git push &&\
	cd ~/Desktop/SO/Sistemi_Operativi/IPC