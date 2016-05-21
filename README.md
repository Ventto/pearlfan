# Ventilo

Contrôle d'un ventilateur USB DreamCheeky.

## Dépendances

* libusb
* netpbm

## Compilation

Depuis la racine du projet, lancez `make`.

## Execution

Pour executer un des programmes, se positionner dans le dossier, puis appeller
l'executable avec en paramètre le chemin du fichier de configuration.

Par exemple:

```bash
$ cd ./src/userapp/libusb_app/
$ ./test config
```

## Charger le driver

Un script utilitaire pour charger le module est disponible dans
`./src/driver/script.sh`.

## Auteurs

* Thomas Venries
* Franklin Mathieu
