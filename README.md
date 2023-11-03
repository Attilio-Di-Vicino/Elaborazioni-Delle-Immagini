# Elaborazioni delle immagini

Questo progetto consiste in una serie di elaborazioni delle immagini implementate in C++ utilizzando la libreria OpenCV. Le elaborazioni attualmente implementate includono:

- **Rilevamento dei bordi**: utilizzando algoritmi come Canny per individuare i bordi presenti nell'immagine.
- **Rilevamento dei corner**: utilizzando algoritmi come Harris per individuare i corner presenti nell'immagine.
- **Trasformazione di Hough per il rilevamento di linee e cerchi**: utilizzo della trasformazione di Hough per identificare linee rette e cerchi presenti nell'immagine.
- **Sogliatura**: Calcolo di soglie ottimali per la segmentazione utilizzando il metodo di Otsu singola soglia e multi soglia
- **Altri algoritmi di segmentazione**: inclusione di tecniche come region growing per la segmentazione dell'immagine in base a criteri di omogeneità.
- **Algoritmi classici**: implementazione di algoritmi classici di elaborazione delle immagini, come split and merge per la segmentazione gerarchica dell'immagine.
- **Segmentazione dell'immagine tramite clustering**: applicazione di algoritmi di clustering come k-means e mean-shift per raggruppare i pixel dell'immagine in segmenti omogenei.

Queste funzionalità offrono diverse possibilità di analisi e manipolazione delle immagini, consentendo di ottenere risultati interessanti e utili per varie applicazioni.

Per utilizzare il progetto, è necessario avere installata la libreria OpenCV e compilare il codice sorgente C++ utilizzando un compilatore compatibile.

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

## Compilazione OpenCV

Per utilizzare le elaborazioni delle immagini, è necessario compilare il file sorgente corrispondente all'elaborazione desiderata. Ad esempio, per compilare il file sorgente relativo al rilevamento dei bordi, eseguire il seguente comando da terminale:

```
g++ edge_detection.cpp -o edge_detection `pkg-config --cflags --libs opencv4`
```

Questo comando compilerà il file sorgente `edge_detection.cpp` e genererà l'eseguibile `edge_detection`. Per eseguire l'elaborazione, utilizzare il comando:

```
./edge_detection <nome_immagine>
```

Dove `<nome_immagine>` è il nome del file immagine su cui eseguire l'elaborazione.

## Compilazione personalizzata

Vedere il README.md del seguente link:

https://github.com/Attilio-Di-Vicino/Elaborazioni-Delle-Immagini/tree/main/src
