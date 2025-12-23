use std::{
    arch::x86_64, fs, io::{self, Write}, path::{self, PathBuf}, string
};

use crate::procedure::*;

mod procedure;

fn main() {
    // inserimento del path riferito alla cartella
    print!("Inserisci il path della cartella da ordinare (di base): ");
    io::stdout().flush().unwrap(); // necessario flushare altrimenti non viene stampata la stringa
    let mut path_string = String::new();
    io::stdin()
        .read_line(&mut path_string)
        .expect("Failed to read line");
    let path_base = PathBuf::from(path_string.trim());

    /* //! per il finto caricamento
    println!("Controllo la cartella...");
    let mut loading:[char; 10];
    loading = [' ';10];

    let mut loading_i: usize = 0;
    let mut barra: String;
    let mut percentuale: i8;

    for i in 0..5 {
        barra = loading.iter().collect();
        percentuale = i * 20;
        print!("Caricamento: [{}] %{percentuale}\r", barra );
        io::stdout().flush().unwrap();
        loading[loading_i] = '#';
        loading[loading_i + 1] = '#';
        loading_i += 2;
        thread::sleep(Duration::from_secs(1));
    }
    percentuale = 100;
    barra = loading.iter().collect();
    println!("\rCaricamento: [{}] %{percentuale}  {loading_i}", barra );
    */
    println!("{path_base:?}");

    // ! per stampare tutti i path di file e directory di una cartella
    /*if path_base.exists() && path_base.is_dir(){
        procedure::stampa_file_ricorsiva(&path_base, String::from(""));
    }else{
        println!("ERRORE: {path_base:?} è un path non valido");
    }*/

    // creiamo la procedura per costruire un array per i file e uno per le directory
    let dir_paths: Vec<PathBuf> = ottieni_percorsi(&path_base, procedure::FiltroPercorso::Dir);
    let file_paths: Vec<PathBuf> = ottieni_percorsi(&path_base, procedure::FiltroPercorso::File);

    // todo implementare un modo per riordinare i file nelle cartelle, DA PENSARCI ANCORA...

    

    // stampa_file_ricorsiva(&path_base, String::from(""));

    println!("{:?} {:?}", dir_paths[0].read_dir().expect("ciao").next().expect("ciao2"), dir_paths[0]);
    
    move_files(&file_paths, &path_base);

    

    // println!("{:?} | {:?}", file_paths[0].extension(), file_paths[0]);
    //println!("--------------DOPO L'ESECUZIONE----------------");
    //procedure::stampa_file_ricorsiva(&path_base, String::from(""));
}
