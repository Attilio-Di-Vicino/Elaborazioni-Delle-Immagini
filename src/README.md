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

4. Aprire un nuvo terminale fino ad arrivare alla directory `src`.

5. Compilare i file `.cpp` tramite il comando:

    ```
    cv file.cpp
    ```

6. Questo genererà l'eseguibile `a.out`, che può essere eseguito tramite il comando:

    ```
    nome_immagine
    ```

7. Ad esempio, per eseguire il file a.out con l'immagine `lena`, eseguire il seguente comando:

    ```
    lena
    ```

### Eliminazione alias

Se si vogliono cancellare gli alias generati tramile il file `z-compile-alias.sh`:

1. Aprire un nuovo terminale

2. Digitare il comando:

    ```
    nano ~/.bashrc
    ```
3. Trovare gli alias generati

4. Cancellarli manualmente dal file sul terminale

5. Salvare le operaizoni premendo `Ctrl x` e successivamente `y` 