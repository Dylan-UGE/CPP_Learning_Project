# Se familiariser avec l'existant

## A- Exécution

Compilez et lancez le programme.

Allez dans le fichier `tower_sim.cpp` et recherchez la fonction responsable de gérer les inputs du programme.
Sur quelle touche faut-il appuyer pour ajouter un avion ?
> Avec la touche ``c`` 

Comment faire pour quitter le programme ? 
> À l'aide des touches ``q`` ou ``x``

A quoi sert la touche 'F' ?
> À mettre en plein écran

Ajoutez un avion à la simulation et attendez.
Que est le comportement de l'avion ?
> L'avion de pose, va se garer pour faire l'entretien, repart puis recommence.

Quelles informations s'affichent dans la console ?
> KL6289 is now landing... \
  now servicing KL6289... \
  done servicing KL6289 \
  KL6289 lift off

Ajoutez maintenant quatre avions d'un coup dans la simulation.
Que fait chacun des avions ?
> Trois avions atterrissent et vont se garer pour l'entretien, chacun dans une place différente. Puis quand une place se libère et qu'un avion redécolle le quatrième avion atterrit pour faire la même chose


## B- Analyse du code

Listez les classes du programme à la racine du dossier src/.
Pour chacune d'entre elle, expliquez ce qu'elle représente et son rôle dans le programme.

> - **class Aircraft :** classe qui représente un avion.
> - **class AirportType :**
> - **class Airport :** classe gérant un aréoport.
> - **class Terminal :** classe gérant un terminal.
> - **class TowerSimulation :** classe principale de notre projet.
> - **class Tower :** classe aiguillant les avions.
> - **class WaypointType :** classe représentant un point de contrôle

\
Pour les classes `Tower`, `Aircaft`, `Airport` et `Terminal`, listez leurs fonctions-membre publiques et expliquez précisément à quoi elles servent.
Réalisez ensuite un schéma présentant comment ces différentes classes intéragissent ensemble.
> - **class Tower :** \
    - get_instructions = fonctions qui donnent les instructions à un avion, en fonction de s'il est dans les airs ou dans un terminal \
    - arrived_at_terminal = fonction appelé lorsqu'un avion atterrit pour lui fournir un terminal.

> - **class Aircraft :** \
    - get_flight_num = renvoie le numéro du vol. \
    - distance_to = fonction qui renvoie la position de l'avion par rapport à un point.
    - display = dessine l'avion sur la fenêtre. \
    - move = déplace l'avion.

> - **class Airport :** \
    - get_tower = renvoie l'objet de type Tower associé à l'aéroport. \
    - display = affiche l'aéroport sur la fenêtre. \
    - move = permet de faire avancer la progression de tous les terminaux associés à l'aéroport.

> - **class Terminal :** \
    - in_use = fonction qui permet de savoir si un terminal est en cours d'utilisation ou non. \
    - is_servicing = permet de savoir si un avion est entrain d'être entretenir ou non. \
    - assign_craft = assigne un avion au terminal. \
    - start_service = commence l'entretien pour l'avion associé. \
    - finish_service = termine l'entretien pour l'avion associé. \
    - move = fait avancer la progression de l'entretien. 

\
Quelles classes et fonctions sont impliquées dans la génération du chemin d'un avion ? 
> Les classes Tower avec la fonction get_instructions, Airport avec la fonction start_path et AirportType avec la fonction terminal_to_air.

Quel conteneur de la librairie standard a été choisi pour représenter le chemin ?
Expliquez les intérêts de ce choix.
> Le conteneur choisi est une ``std::deque``. L'intérêt de ce choix est qu'on a les mêmes avantages qu'une std::queue mais qu'on peut ajouter et retirer des deux côtés.

## C- Bidouillons ! 

1) Déterminez à quel endroit du code sont définies les vitesses maximales et accélération de chaque avion.
Le Concorde est censé pouvoir voler plus vite que les autres avions.
Modifiez le programme pour tenir compte de cela.
    > Ces informations sont définis au niveau de la fonction-membre ``init_aircraft_types`` de la stucture AircraftType.

2) Identifiez quelle variable contrôle le framerate de la simulation.\
Le framerate correspond au temps de rafraichissement du programme, c'est-à-dire le nombre de fois où les éléments du programme seront mis à jour (ajout de nouvel avion à la simulation, déplacement, etc) en une seconde.\
Ajoutez deux nouveaux inputs au programme permettant d'augmenter ou de diminuer cette valeur.
Essayez maintenant de mettre en pause le programme en manipulant ce framerate. Que se passe-t-il ?\
Ajoutez une nouvelle fonctionnalité au programme pour mettre le programme en pause, et qui ne passe pas par le framerate.
    > Le changement du framerate se fait au moyen de la variable ``GL::ticks_per_sec`` defini dans ``GL/open_interface.hpp`` et pour gérer les input on fait appel à ``GL::keystrokes.emplace``. 
    >
    > Si on essaye de mettre le programme en pause en manipulant le framerate pour le mettre à 0, le programme s'arrête subitement.

3) Identifiez quelle variable contrôle le temps de débarquement des avions et doublez-le.
    > La variable ``SERVICE_CYCLES`` qui se trouve dans ``config.hpp`` permet de contrôler le temps de débarquement des avions.

4) Lorsqu'un avion a décollé, il réattérit peu de temps après.
Assurez-vous qu'à la place, il soit supprimé de la `move_queue`.\
Pour tester, il suffit de dézoomer et de vérifier que les avions suffisament éloignés ne bougent plus.
Indices :\
A quel endroit pouvez-vous savoir que l'avion doit être supprimé ?\
Pourquoi n'est-il pas sûr de procéder au retrait de l'avion dans cette fonction ?
A quel endroit de la callstack pourriez-vous le faire à la place ?\
Que devez-vous modifier pour transmettre l'information de la première à la seconde fonction ?
    > Seul l'avion lui même permet de savoir s'il est arrivé à un waypoint final mais il ne peut être supprimé qu'à partir de la fonction ``timer`` de ``GL/opengl_interface.cpp``. Pour ça on crée un flag qui indique que l'avion peut être supprimé et qu'on met à vrai après que l'avion est sur le point de redécollé.

5) Lorsqu'un objet de type `Displayable` est créé, il faut ajouter celui-ci manuellement dans la liste des objets à afficher.
Il faut également penser à le supprimer de cette liste avant de le détruire.
Faites en sorte que l'ajout et la suppression de `display_queue` soit "automatiquement gérée" lorsqu'un `Displayable` est créé ou détruit.\
Essayez maintenant de supprimer complètement l'avion du programme lorsque vous le retirez de la `move_queue`.\
En dézoomant, vous devriez maintenant constater que les avions disparaissent maintenant de l'écran.
    > On modifie le constructeur ainsi que le destructeur de l'objet.

6) La tour de contrôle a besoin de stocker pour tout `Aircraft` le `Terminal` qui lui est actuellement attribué, afin de pouvoir le libérer une fois que l'avion décolle.
Cette information est actuellement enregistrée dans un `std::vector<std::pair<const Aircraft*, size_t>>` (size_t représentant l'indice du terminal).
Cela fait que la recherche du terminal associé à un avion est réalisée en temps linéaire, par rapport au nombre total de terminaux.
Cela n'est pas grave tant que ce nombre est petit, mais pour préparer l'avenir, on aimerait bien remplacer le vector par un conteneur qui garantira des opérations efficaces, même s'il y a beaucoup de terminaux.\
Modifiez le code afin d'utiliser un conteneur STL plus adapté. Normalement, à la fin, la fonction `find_craft_and_terminal(const Aicraft&)` ne devrait plus être nécessaire.
    > on remplace le ``std::vector<std::pair<const Aircraft*, size_t>>`` par un ``std::unordered_map<const Aircraft*, size_t>``

## D- Théorie

1) Comment a-t-on fait pour que seule la classe `Tower` puisse réserver un terminal de l'aéroport ?

2) En regardant le contenu de la fonction `void Aircraft::turn(Point3D direction)`, pourquoi selon-vous ne sommes-nous pas passer par une réference constante ?
Pourquoi n'est-il pas possible d'éviter la copie du `Point3D` passé en paramètre ?

## E- Bonus

Le temps qui s'écoule dans la simulation dépend du framerate du programme.
La fonction move() n'utilise pas le vrai temps. Faites en sorte que si.
Par conséquent, lorsque vous augmentez le framerate, la simulation s'exécute plus rapidement, et si vous le diminuez, celle-ci s'exécute plus lentement.

Dans la plupart des jeux ou logiciels que vous utilisez, lorsque le framerate diminue, vous ne le ressentez quasiment pas (en tout cas, tant que celui-ci ne diminue pas trop).
Pour avoir ce type de résultat, les fonctions d'update prennent généralement en paramètre le temps qui s'est écoulé depuis la dernière frame, et l'utilise pour calculer le mouvement des entités.

Recherchez sur Internet comment obtenir le temps courant en C++ et arrangez-vous pour calculer le dt (delta time) qui s'écoule entre deux frames.
Lorsque le programme tourne bien, celui-ci devrait être quasiment égale à 1/framerate.
Cependant, si le programme se met à ramer et que la callback de glutTimerFunc est appelée en retard (oui oui, c'est possible), alors votre dt devrait être supérieur à 1/framerate.

Passez ensuite cette valeur à la fonction `move` des `DynamicObject`, et utilisez-la pour calculer les nouvelles positions de chaque avion.
Vérifiez maintenant en exécutant le programme que, lorsque augmentez le framerate du programme, vous n'augmentez pas la vitesse de la simulation.

Ajoutez ensuite deux nouveaux inputs permettant d'accélérer ou de ralentir la simulation.
