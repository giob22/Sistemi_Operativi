

pub fn stampa_file_ricorsiva(path: &std::path::PathBuf, space: String){
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
        let mut extension: String;
        if path.is_dir() {
            println!("{space}\u{1F4C1} {}", path.file_name().expect("Errore").display());
            let mut next_space = space.clone();
            next_space.push_str("  "); 
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
                            extension = String::from("[");
                            extension.push_str(&tipo.to_string_lossy().to_string());
                            extension.push(']'); 
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
            println!("{space}\u{1F4C4}[{}] e ha formato: {extension}", file_name.display());
            // spostiamo il file nella cartella giusta
            
        }
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