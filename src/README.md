# src

La cartella `src` contiene i codici sorgente per l'elaborazione delle immagini, basati sulla libreria OpenCV, e alcune implementazioni personalizzate all'interno della cartella `Algorithm`.

### Installazione

Per compilare i file `.cpp` è necessario eseguire alcune procedure. 

1. Aprire un terminale fino ad arrivare alla directory `src`. 

2. Dare i permessi ai file `.sh` tramite i seguenti comandi da terminale:

    ```
    chmod +x z-compile-alias.sh
    chmod +x z-compile.sh
    ```

3. Eseguire il file `z-compile-alias.sh` tramite il seguente comando da terminale:

    ```
    ./z-compile-alias.sh
    ```

4. Compilare i file `.cpp` tramite il comando:

    ```
    cv file.cpp
    ```

Questo genererà l'eseguibile `a.out`, che può essere eseguito tramite il comando:

    ```
    nome_immagine
    ```

Ad esempio, per eseguire l'immagine `lena`, eseguire il seguente comando:

    ```
    lena
    ```