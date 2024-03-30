# Einfaches neuronales Netz zur Erkennung von Ziffern

Implementierung eines einfachen neuronalen Netzes zur Erkennung von Ziffern.
Der Algorithmus basiert auf dem, im Buch von Tariq Rashid [^1] vorgestellten Algorithmus.  Der Autor hat auch den Python-Code zu seinem Buch in Github abgelegt [^2].

Unter mynn ist eine Cpp Implementierung von diesem Algorithmus zu finden.

Um dieses neuronale Netz praktisch zu testen und zu nutzen zu können, wird unter mynnwebcam das in mynn erstellte und trainierte neuronale Netz genutzt, um per Webcam einzelne Ziffern zu erkennen.
Dieses Programm läuft unter Windows und Linux, sowie als App auf einem Android Device
„Screenshots“

Als Framework für beide Programme wird QT in der Version 5.15.2 genutzt. [^3]. 
Da sich die Schnittstelle zwischen QML und den Multimedia Klassen in QT6 stark geändert hat, kann QT in der Version 6.x nicht für mynnwebcam genutzt werden.

## MYNN
### Erklärung der Klasse

#### Hinweise für die Compilierung der Software
Neben dem QT Framework, wird für Vector-Operationen, wie Transpose, die EigenLib [^4] [^5] genutzt. Hier muss der Include-Pfad in der Datei mynn_eigenKlasse.pro angepasst werden: <br>
``` 
z.B. INCLUDEPATH += C:\work\EigenLib\eigen-3.4.0
```

Die Software sollte als Release gebaut werden, da ansonsten die Berechnung des neuronalen Netzes deutlich länger dauert.

#### Nutzung der Software
Die MNIST Trainings- und Testfiles können im cvs Format von [^6] heruntergeladen werden.<br>
<br>
mynn.exe bzw. mynn hat folgende Eingangsparameter:<br>
``` 
 -t Trainingsdatensatz
 --cs berechnet die Scorecard mit dem übergebenen Testdatensatz
 --wih gelernte Input – Hiddenlayer Werte
 --who gelernte Hidden – Output Werte
 --swih speichert die gelernten „Input – Hiddenlayer „Werte ab
 --swho speichert die gelernten „Hidden – Outputlayer“ Werte ab
``` 

##### Beispiel:
Trainieren und testen vom neuronalen Netz:<br>
``` 
mynn -t mnist_train.csv –cs mnist_test.csv
``` 
Trainieren und testen vom neuronalen Netz inkl abspeichnern der gelernten Werte:<br>
``` 
mynn -t mnist_train.csv –cs mnist_test.csv --swih wih.cvs --swho who.cvs
```

# MYNN-Webcam
In diesem Beispiel wird die in LINK erstellte Klasse mynn, etwas angepasst, genutzt um Ziffern per Webcam bzw. der Smartphone-Kamera zu erkennen.

## MYNN-APP
Als Basis für die Software, dient das Beispiel von Stephen Quan [^9] zur Nutzung der QAbstractVideoFilter Klasse von QT.
Auch hier wird die EigenLib [^4] genutzt.
Zur Bildaufbereitung wird die OpenCV Libary [^7] verwendet. Getestet wurde die Software unter Windows und Android mit der OpenCV Version 4.5.2 siehe [^8] bzw. [^9]. Unter Linux wurde, die mit der Linux-Distribution ausgelieferte, OpenCV Version genutzt. 

### Erklärung der Klassen

#### Hinweise für die Compilierung der Software

##### Anpassung der *.pro Datei

Je nach dem für welches Betriebssytem, die Software erstellt werden soll, müssen die Variablen, angepasst werden.

Anpassung Include-Pfad für die EigenLib:<br>
``` 
z.B. INCLUDEPATH += C:\work\EigenLib\eigen-3.4.0
```
Anpassung der OPENCV_ANDROID_SDK_PATH Variable
```
z.B. OPENCV_ANDROID_SDK_PATH = /home/thomas/Spielwiese/OpenCV-android-sdk_4_5_4/
```

**Hinweis für Windows:**<br>
Damit die OpenCV Libaries beim Ausführen der Software gefunden werden, muss die Windows Umgebungsvariable PATH muss auf die OpenCV Binaries gesetzt werden.<br>
Beispiel:<br>
```
Path=C:\OpenCV-MinGW-Build-OpenCV-4.5.2-x64\x64\mingw\bin
```
 
**Hinweis für Linux:**<br>
Hier wurden die mit der Linux-Distribution ausgelieferte OpenCV Version genutzt.
Ggf. müssen die Variablen INCLUDEPATH und LIBS für OpenCV in mynn_eigenKlasse.pro angepasst werden.


##### Anpassung QT Creator Einstellungen:

Für das Deployment der wih.cvs & who.cvs Dateien, muss im QT Creator unter Einstellung zur Ausführung „install“ bei Make arguments angeben werden. Damit werden diese Dateinen in Build-Verzeichnis kopiert.

<p align="center">
<img src="doc/QT_Install.png">
</p>

Hinweis für Android:<br>
Als Target-Architektur wird arm64-v8a verwendet.<br>
Deshalb darf im QT Creator unter Projekt Build Einstellungen, nur die arm64-v8a ABI ausgewählt sein.
<p align="center">
<img src="doc/QT_Android_arm64.png">
</p>
 


### Referenzen

[^1]: Buch: Neuronale Netze selbst programmieren von Tariq Rashid <br>https://dpunkt.de/produkt/neuronale-netze-selbst-programmieren

[^2]: https://github.com/makeyourownneuralnetwork/makeyourownneuralnetwork

[^3]: https://www.qt.io/ 

[^4]: https://eigen.tuxfamily.org

[^5]: https://gitlab.com/libeigen/eigen/-/releases/3.4.0

[^6]: https://pjreddie.com/projects/mnist-in-csv/

[^7]: https://opencv.org/

[^8]: https://opencv.org/releases/

[^9]: https://github.com/huihut/OpenCV-MinGW-Build 

[^10]: https://github.com/stephenquan/MyVideoFilterApp



