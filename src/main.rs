use std::path::PathBuf;



// rendiamo la chiamata ricorsiva
fn stampa_file_ricorsiva(path: &std::path::PathBuf, space: String){
    // per rendere più carina la stampa utilizziamo degli spazi per identificare i file in una cartella
    let space:String = String::from(space);    
    // iniziamo con il trovare un modo per leggere il nome di tutti i file contenuti in una cartella
    let percorsi_iterator = std::fs::read_dir(&path).expect("Errore nella lettura della cartella");
    // iteriamo sui file trovati
    for x in percorsi_iterator{
        // NOTA: ogni singola voce è un result, quindi devo sempre controllare che sia andata a buon fine l'operazione
        let file = x.expect("Impossibile leggere questo file specifico");
        
        // otteniamo il path
        let path = file.path();
        let file_name = file.file_name();
        let extension: String;
        if path.is_dir() {
            println!("{space}\u{1F4C1} {:?}", path.file_name().expect("Errore"));
            let mut next_space = space.clone();
            next_space.push(' '); 
            stampa_file_ricorsiva(&path, next_space);
        }else{

            match infer::get_from_path(&path) {
                Ok(Some(tipo)) => {
                    // tipo contiene info sull'estensione corretta di un file e il MIME type
                    extension = tipo.extension().to_string();
                    //println!("{tipo} e estensione: {extension}");
                    
                }
                Ok(None) => {
                    // in questo caso infer::get_from_Path non riesce a rilevare il formato del file, quindi probabilmente è testo o codice o binario,
                    // quindi a seconda dell'estensione nel nome del file lo classifichiamo
                    match path.extension(){
                        Some(tipo) => {
                            extension = tipo.to_string_lossy().to_string();
                        }
                        None =>{
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
            
            println!("{space}\u{1F4C4}[{file_name:?}] e ha formato: {extension}");
        }
    }
    
}


fn main() {
    let path = PathBuf::from("./cartella_prove");
    if path.exists(){
        stampa_file_ricorsiva(&path, String::from(""));
    }else{
        println!("ERRORE DIRECTORY INIZIALE NON VALIDA");
    }
}
