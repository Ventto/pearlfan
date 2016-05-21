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

## Format du fichier de configuration

Un fichier de configuration est un fichier texte comprenant au plus
8 lignes (le ventilateur ne supportant que 8 images successives)

Une ligne est du format suivant: `+chemin+x/y/z`

où `chemin` est un chemin vers un fichier `.pbm` valide,
et où `x`, `y` et `z` sont les modes d'ouverture, de fermeture,
et de transition:

<expliquer les modes ici>

## Auteurs

* Thomas Venries
* Franklin Mathieu
