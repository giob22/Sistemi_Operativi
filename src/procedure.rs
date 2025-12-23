use std::{
    clone, f32::consts::E, fs, io::{self, Write}, path::{Path, PathBuf}, thread, time::Duration, vec
};
#[derive(Clone, Copy)]
pub enum FiltroPercorso {
    Dir,
    File,
}

pub fn stampa_file_ricorsiva(path: &std::path::PathBuf, space: String) {
    // per rendere più carina la stampa utilizziamo degli spazi per identificare i file in una cartella
    let space: String = String::from(space);
    // iniziamo con il trovare un modo per leggere il nome di tutti i file contenuti in una cartella
    let percorsi_iterator = std::fs::read_dir(&path).expect("Errore nella lettura della cartella");
    // iteriamo sui file trovati
    for x in percorsi_iterator {
        // NOTA: ogni singola voce è un result, quindi devo sempre controllare che sia andata a buon fine l'operazione
        let file = x.expect("Impossibile leggere questo file specifico");

        // otteniamo il path
        let path = file.path();
        let file_name = file.file_name();
        let mut extension: String;
        if path.is_dir() {
            println!(
                "{space}\u{1F4C1} {}",
                path.file_name().expect("Errore").display()
            );
            let mut next_space = space.clone();
            next_space.push_str("  ");
            stampa_file_ricorsiva(&path, next_space);
        } else {
            match infer::get_from_path(&path) {
                Ok(Some(tipo)) => {
                    // tipo contiene info sull'estensione corretta di un file e il MIME type
                    extension = tipo.extension().to_string();
                    //println!("{tipo} e estensione: {extension}");
                }
                Ok(None) => {
                    // in questo caso infer::get_from_Path non riesce a rilevare il formato del file, quindi probabilmente è testo o codice o binario,
                    // quindi a seconda dell'estensione nel nome del file lo classifichiamo
                    match path.extension() {
                        Some(tipo) => {
                            extension = String::from("[");
                            extension.push_str(&tipo.to_string_lossy().to_string());
                            extension.push(']');
                        }
                        None => {
                            extension = String::from("NONE");
                        }
                    }
                }
                Err(e) => {
                    println!("ERRORE: {e}");
                    extension = String::from("NONE");
                }
            }

            // stampiamo il percorso a video
            println!(
                "{space}\u{1F4C4}[{}] e ha formato: {extension}",
                file_name.display()
            );
            // spostiamo il file nella cartella giusta
        }
    }
}

pub fn ottieni_percorsi(cartella: &PathBuf, filtro: FiltroPercorso) -> Vec<PathBuf> {
    let mut vettore = Vec::new();

    let percorsi_iterator =
        std::fs::read_dir(cartella).expect("Errore nella lettura della cartella");

    for x in percorsi_iterator {
        let path_file = x.expect("file non raggiungibile").path();

        match filtro {
            FiltroPercorso::Dir => {
                if path_file.is_dir() {
                    vettore.push(path_file.clone());
                    vettore.append(&mut ottieni_percorsi(&path_file, filtro));
                }
            }
            FiltroPercorso::File => {
                if path_file.is_file() {
                    vettore.push(path_file);
                } else {
                    vettore.append(&mut ottieni_percorsi(&path_file, filtro));
                }
            }
        }
    }
    vettore
}


pub fn move_files(file_paths: &Vec<std::path::PathBuf>, path_base: &PathBuf) {

    let mut CartelleCreate : Vec<String> = Vec::new();


    let path_base_string = String::from(
        path_base
            .to_str()
            .expect("Errore path base non compatibile"),
    );

    let mut risposta = String::new();

    println!("Sicuro di voler spostare tutti file all'interno di : \"{path_base_string}\"");
    io::stdout().flush().unwrap();

    io::stdin()
        .read_line(&mut risposta)
        .expect("Failed to read line");

    print!("{}", risposta);
    if risposta.trim() == "s" || risposta.trim() == "y" || risposta.trim() == "si" || risposta.trim() == "yes" {
           
        for i in 0..file_paths.len() {
            
            let nome_cartella = String::from(file_paths[i].extension().expect("Errore1").to_str().expect("Errore"));

            

            let mut final_path = String::new();
            final_path.push_str(path_base.to_str().expect("Errore"));
            final_path.push_str("/");

            final_path.push_str(&nome_cartella);
            
            
            if !CartelleCreate.contains(&nome_cartella){
                fs::create_dir_all(&final_path).expect("Errore Creare Cartella");
                CartelleCreate.push(nome_cartella);
                
                // println!("1: {:?}", final_path);

            }
            
            final_path.push_str("/");
            final_path.push_str(
                file_paths[i]
                .file_name()
                .expect("Errore")
                .to_str()
                    .expect("Errore"),
                );


                fs::rename(file_paths[i].clone(), final_path).expect("Errore Nel Muovere");
        }
    }

}


pub fn cancella_cartelle_svuotate_ricorsiva(dir_paths: &PathBuf){
    let percorsi_iterator = std::fs::read_dir(&dir_paths).expect("Errore nella lettura della cartella");
    for i in percorsi_iterator {
        let file = i.expect("Errore path");
        if file.path().is_dir() {
            cancella_cartelle_svuotate_ricorsiva(&file.path());
        }
    }
    let percorsi_iterator = std::fs::read_dir(&dir_paths).expect("Errore nella lettura della cartella");
    if percorsi_iterator.count() == 0 {
        fs::remove_dir(dir_paths.clone()).expect("Errore durante cancellazione delle cartelle vuote");
    }
}


/*
let mut final_path;
if extension == "pdf" {
    // costruisco la destinazione finale
    final_path = String::from("./cartella_prove/pdf/");
    final_path.push_str(&file_name.to_string_lossy().to_string());
    //println!("{}", path.display());
    std::fs::rename(path, final_path).expect("Errore");
}else if extension == "png" {
    final_path = String::from("./cartella_prove/immagini/");
    final_path.push_str(&file_name.to_string_lossy().to_string());
    //println!("{}", path.display());
    std::fs::rename(path, final_path).expect("Errore");
}
*/
