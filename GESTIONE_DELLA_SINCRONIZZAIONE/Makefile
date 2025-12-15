all: publish

publish:
	git add .
	git commit -m "update" &&\
	git push &&\
	mdbook build &&\
	cp -r ./book/* /home/giovanni/Desktop/uni_ubuntu/SistemiOperativi/Appunti/synch_management &&\
	cd /home/giovanni/Desktop/uni_ubuntu/SistemiOperativi/Appunti &&\
	git add . &&\
	git commit -m "aggiornamento automatico per appunti" &&\
	git push &&\
	cd ~/Desktop/SO/Sistemi_Operativi/GESTIONE_DELLA_SINCRONIZZAIONE