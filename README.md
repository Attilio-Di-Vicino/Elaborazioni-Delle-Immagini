# Elaborazioni delle immagini

Questo progetto consiste in una serie di elaborazioni delle immagini implementate in C++ utilizzando la libreria OpenCV. Le elaborazioni attualmente implementate includono il rilevamento dei bordi, la segmentazione dell'immagine tramite clustering, la trasformazione di Hough per il rilevamento di linee e la trasformazione di Hough probabilistica per il rilevamento di segmenti.

## Installazione di OpenCV

Per poter utilizzare le elaborazioni delle immagini è necessario installare OpenCV. Al momento della stesura di questo documento (4 maggio 2023), la versione di OpenCV disponibile è la 4.5.4.

Per installare OpenCV, seguire i seguenti passaggi:

1. Aprire un terminale e installare i pacchetti necessari tramite il comando:

    ```
    sudo apt-get install build-essential cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
    ```

2. Clonare il repository OpenCV da GitHub tramite il comando:

    ```
    git clone https://github.com/opencv/opencv.git
    ```

3. Posizionarsi nella directory appena clonata tramite il comando:

    ```
    cd opencv
    ```

4. Creare una nuova directory `build` tramite il comando:

    ```
    mkdir build
    ```

5. Posizionarsi nella directory `build` tramite il comando:

    ```
    cd build
    ```

6. Configurare l'installazione di OpenCV tramite il comando:

    ```
    cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local ..
    ```

7. Compilare OpenCV tramite il comando:

    ```
    make -j4
    ```

8. Installare OpenCV tramite il comando:

    ```
    sudo make install
    ```

9. Verificare che l'installazione sia stata effettuata correttamente tramite il comando:

    ```
    pkg-config --modversion opencv4
    ```

    Questo comando dovrebbe restituire la versione di OpenCV installata.

## Utilizzo delle elaborazioni delle immagini

Per utilizzare le elaborazioni delle immagini, è necessario compilare il file sorgente corrispondente all'elaborazione desiderata. Ad esempio, per compilare il file sorgente relativo al rilevamento dei bordi, eseguire il seguente comando da terminale:

```
g++ edge_detection.cpp -o edge_detection `pkg-config --cflags --libs opencv4`
```

Questo comando compilerà il file sorgente `edge_detection.cpp` e genererà l'eseguibile `edge_detection`. Per eseguire l'elaborazione, utilizzare il comando:

```
./edge_detection <nome_immagine>
```

Dove `<nome_immagine>` è il nome del file immagine su cui eseguire l'elaborazione.