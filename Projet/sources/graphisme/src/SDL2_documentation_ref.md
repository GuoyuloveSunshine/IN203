# SDL2-C++ : encapsulation C++ de la bibliotheque SDL 2

****
###### Instructions
**Fleches du haut et du bas ou bouton souris gauche ou droit** pour defiler ligne a ligne
**Pagedown et Pageup** pour defiler rapidement
**home** pour revenir a la premiere ligne
**end** pour afficher les vingt-cinq dernieres lignes
****

## Introduction
====

Simple Directmedia Layer (SDL) est une bibliotheque ecrite en C qui permet un acces *bas niveau* au son, au clavier, a la souris, au joystick, et au hardware graphique via OpenGL et Direct3D. Cette bibliotheque est utilisee par les logiciels permettant de visualiser de la video, les emulateurs, des jeux (en particuliers ceux publies par Valve utilisent SDL...). 

SDL est supportee officiellement par Windows, Mac OS X, Linux, iOS et androïd. D'autres plateformes sont egalement plus ou moins supportees.

SDL est ecrit en langage C, est supporte nativement par le C++ et il existe des api pour divers langages dont C# et Python.

La version 2.0 de cette librairie est sous license zlib, c'est a dire qu'on peut l'utiliser gratuitement dans n'importe quel logiciel.

Vous trouverez un wiki sur cette bibliotheque sur le lien suivant : https://github.com/libsdl-org/SDL/wiki

La bibliotheque proposee ici se repose sur SDL 2.0 et profite de l'avantage offert par le C++ pour simplifier l'appel a SDL. De plus, il a ete rajoute certaines fonctionnalites non presentes dans SDL 2 (dont le texte multi-ligne avec justification) pour faciliter la mise au point du projet.

Cette bibliotheque est amenee a evoluer, et si vous voulez proposer **une nouvelle fonctionnalite ou une amelioration de cette bibliotheque**, vous êtes les bienvenues !

## Presentation de SDL2-C++
====

### Initialisation et finalisation
----

Afin d'initialiser certains parametres correctement ainsi que la bibliotheque SDL 2, il est necessaire d'appeler une fonction d'initialisation. De même, afin de nettoyer correctement la memoire prise par le programme, il est conseille de finaliser la session graphique. Cela se fait a l'aide de deux fonctions qu'on trouve dans le header sdl2.hpp

....
> sdl2::init(argc, args) : 
permet d'initialiser le contexte SDL 2. Prends les argc et argv de la fonction main.
> sdl2::finalise()       : 
finalise la session en nettoyant proprement la memoire
....

**Attention**, il faut s'assurer que tous les objets SDL 2 crees pendant la session graphique soient detruits avant l'appel de la fonction finalise, sous peine d'obtenir des erreurs memoires. La solution la plus simple pour cela est d'initialiser sdl 2 et le finaliser dans la fonction main puis gerer les objets sdl 2 dans des fonctions, mais surtout pas dans la fonction main elle-même !

Ainsi, le fichier contenant la fonction main() ressemblera probablement aux lignes suivantes :

....
> # include "sdl2/sdl2.hpp"
> // debut gere l'affichage
> void debut()
> {
>     ...
> }
> // Fonction principale : que init, debut et finalize d'appeles !
> int main(int argc, char* args[])
> {
>     sdl2::init(argc, args);
>     debut();
>     sdl2::finalize();
>     return EXIT_SUCCESS;
> }
....

### Creation d'une fenêtre graphique
----
Pour creer ensuite une fenêtre graphique, il suffit de la creer de la maniere suivante :

....
> sdl2::window fenêtre(titre, dimension[, position, a afficher])
....

où les parametres entre [ et  ] sont optionnels. Le titre est le texte qui sera affiche dans la barre de la fenêtre (et non dans la fenêtre). Quant a la dimension, c'est un tableau a deux valeurs entieres donnant respectivement le nombre de pixel en largeur et en hauteur. Ainsi, on pourra creer une fenêtre contenant 800 × 600 pixels comme suit :

....
> window fenêtre("Ceci est le titre de ma fenêtre", {800,600});
....

Il est possible d'interroger une fenêtre pour connaître sa dimension :

....
> auto [w,h] = fenêtre.dimensions();
....

où w recevra le nombre de pixel en longueur et h le nombre de pixel en hauteur.

Enfin, on peut effacer tout le contenu d'une fenêtre avec la fonction cls (CLear Screen) qui par defaut rendra un fond blanc. On peut donner a la fonction cls une autre couleur de fond en lui precisant la couleur sous la forme de quatre entiers variant entre 0 et 255, donnant respectivement les composants rouge, vert, bleu et alpha (ce dernier servant a la transparence). Par exemple, pour effacer la fenêtre avec un fond noir :

....
> fenêtre.cls({0,0,0,255});
....

En ce qui concerne l'affichage des divers types d'objets sur la fenêtre, on gere tout cela au travers d'un flux d'entree sur la fenêtre, un peut comme les std::ostream. Les objets peuvent être crees au vol ou bien a l'avance. Un exemple, dont les divers composants seront plus clairs est le suivant :

....
> fenêtre << sdl2::rectangle({10,10},{20,10}, {0xFF,0xF0,0,0xFF}, true)
>         << sdl2::line({60,100}, {20,150}, {0,0,0xFF,0xFF}) << sdl2::flush;
....

##### Remarque 1 

Une fenêtre graphique ne possede pas un repere direct comme en mathematiques. En particuliers, le point (0,0) se trouve en haut a gauche de la fenêtre et l'axe des y est dirige vers le bas (l'axe des x est dirige vers la droite). L'origine de cette convention provient des ecrans a tube cathodique dont le canon a electron balayait l'ecran du haut vers le bas. 

##### Remarque 2

Lorsque vous commencerez a vouloir afficher des "choses" sur la fenêtre, vous serez peut-être surpris de constater que rien ne s'affiche. C'est que vous avez probablement oublie de mettre a jour la fenêtre grace a sdl2::flush qui se gere sur le flot d'entree de la fenêtre (voir l'exemple plus haut).

### Primitives graphiques
----

Quelques primitives graphiques sont proposees par SDL 2 : segments de droites, rectangles pleins ou non, images, texte multi-lignes formates ou non. 

#### Les segments de droites 
____

On peut definir et afficher un segment de droite en lui donnant les deux points d'extremites et la couleur du segment. Par exemple :

....
> sdl2::line segment({0,0},{255,255}, {0xFF,0,0,0xFF});
....

definit un segment de droite allant de la coordonnee (0,0) a la cordonnee (255,255) et dont la couleur est ici rouge.

Pour afficher le segment, il suffit de le mettre dans le flux d'entree de la fenêtre de la maniere suivante :

....
> fenêtre << segment;
....

On peut bien entendu afficher un segment cree au vol, au moment de l'affichage :

....
> fenêtre << sdl2::line({0,0},{255,255}, {0xFF,0,0,0xFF});
....

#### Les rectangles
____

Le rectangle est defini par la position de son coin superieur gauche (celui ayant les coordonnees minimales), sa dimension (nombre de pixel en largeur et hauteur), sa couleur et si il est plein ou non. Pour ce dernier parametre, il est possible de l'omettre, et dans ce cas le rectangle sera plein.

Par exemple :

....
> // Rectangle plein en (0,0) de dimension (256,128) et de couleur cyan
> sdl2::rectange r1({0,0}, {256,128}, {0x00,0xFF,0xFF,0xFF});
> // Rectangle creux en (256,128) de dimension (128,64) et de couleur verte
> sdl2::rectangle r2({256,128}, {128,64}, {0x00, 0xFF, 0x00, 0xFF});
....

Pour l'afficher, on utilise egalement le flux d'entree :

....
> // On affiche les deux rectangle dans la fenêtre
> fenêtre << r1 << r2;
....

Il est egalement possible de creer les rectangles au vol durant l'affichage par le flux d'entree.

#### Les images
____

On peut charger des images aux formats png  (*Portable Netword Graphic*) et jpg (*Joint Photographic Experts Group*) et les afficher a l'ecran. Il est neanmoins, pour des raisons propres a la bibliotheque SDL (utilisee pour cette bibliotheque) necessaire de preciser la fenêtre pour laquelle l'image est destinee.

Ainsi, pour charger une image en png ou jpg (la bibliotheque reconnait automatiquement le format), on ecrira :
....
> sdl2::image homer("data/homer.jpg", fenêtre);
....

Il est possible de definir une couleur comme couleur pour la transparence en parametre supplementaire. Par exemple, on aurait pu preciser que la couleur noire est en fait une couleur transparente de la maniere suivante :

....
> sdl2::image homer("data/homer.jpg", fenêtre, {0x00,0x00,0x00,0x00});
....

Une fois l'image chargee, il est possible de positionner l'image sur la fenêtre (toujours par rapport au coin superieur gauche de l'image), de redimensionner l'image, d'effectuer une rotation dont le centre et l'angle est precise, de renverser l'image par symetrie horizontale ou verticale ou bien de simplement connaître la dimension de l'image.

Par exemple :
....
> // On positionne homer en (310,110)
> homer.at(310,110);
> // On redimensionne l'image pour être de taille 128 x 128 (deformation de l'image)
> homer.stretch({128,128});
> // On effectue une symetrie axiale selon l'axe Oy
> homer.flip(sdl2::image::horizontal);
> // On effectue une rotation de centre (64,64) (le milieu de l'image) et d'angle 45°
> homer.rotate({64,64}, 45);
....

Enfin, il est possible de rajouter un filtre de couleur pour *teinter* l'image :
....
> // Teinte l'image d'une couleur jaunie
> homer.color_filter({0xFF,0xF0,0xA0,0xFF});
....

En fait, le filtre consiste a effectuer une operation binaire **and** sur chaque composant couleur.

Enfin, l'affichage se fait toujours a l'aide du flux d'entree :
....
> fenêtre << homer;
....

Bien sûr, il est egalement possible de creer, modifier et afficher au vol une image dans le flux d'entree. Par exemple :
....
> fenêtre << sdl2::image("data/homer.jpg", fenêtre).at(310,110).stretch({128,128});
....

#### Les textes
____

Les textes sont en fait des images constituee a partir de chaînes de caracteres et de fontes vectorielles. Ces chaînes supportent l'unicode et il est donc possible d'utiliser l'UTF 8 (accents compris donc) pour afficher du texte. Puisque les textes sont des images, *il est possible d'effectuer toutes les manipulations qu'on peut faire sur une image avec un texte*. Ainsi, on peut effectuer une rotation, un filtre, redimensionner, etc.

Mais avant de pouvoir afficher du texte, il va falloir prealablement charger des fontes vectorielles. Avec la bibliotheque est livree (leur copyright le permet) des fontes compatibles avec l'UTF 8 (elles ne le sont pas toutes). Pour charger une fonte, il suffit de charger le fichier contenant la fonte et preciser la taille voulue avec cette fonte (n'oubliez pas que c'est du vectoriel).

Ainsi, pour charger par exemple deux fontes avec des taille differentes :
....
> // Charge une fonte "grasse" avec une taille de 28 pixels
> sdl2::font fonte_titre("./data/Lato-Bold.ttf", 28);
> // Charge une fonte "maigre" avec une taille de 28 pixels.
> sdl2::font fonte_texte("./data/Lato-Thin.ttf", 18);
....

Une fois une fonte (ou plusieurs fontes chargees), on peut definir du texte a afficher. Trois façons de faire sont disponibles.

**Si votre texte est court et tient sur une ligne**, il suffit de donner le texte, la fonte utilisee, la fenêtre destinataire( comme pour les images ) et l'encre utilisee pour ecrire le texte :

Exemple pour un texte court, avec divers traitement issue de l'image :
....
> sdl2::texte titre ("Console d'application elaboree", fonte_titre, fenêtre, {0x00,0xFF,0xFF,0xFF});
> // Positionnement du texte sur la fenêtre
> titre.at(10,310);
> // La dimension permet de calculer le centre de l'image
> auto [wph, hph] = titre.dimension();
> // Rotation d'angle 30° dont le centre est au milieu du texte
> titre.rotate({wph/2,hph/2}, 30);
....

**Si votre texte est long et que vous souhaitez l'afficher sur plusieurs lignes**, deux choix s'offrent a vous.
Soit vous laissez la bibliotheque s'adapter a la hauteur de la fenêtre est afficher sur une certaine largeur votre texte sur plusieurs lignes, soit vous contrôlez la dimension occupee par votre texte. Dans les deux cas, vous pouvez contrôler la couleur du papier et de l'encre, la justification du texte (gauche, droite ou centree), ainsi que la marge a gauche et a droite:

....
> sdl2::texte(const std::string& phrase, const font& fnt, window& win, int maxwidth, int maxheight,
>             const color& encre, const color& papier, std::int32_t justification = left_justify,
>             int marge_width = 5, int marge_height = 5) :
Affiche le texte contenu dans *phrase* avec la fonte *fnt* a destination de la fenêtre *win*, avec une largeur maximale de *maxwidth* et une hauteur maximale de *maxheight* pixel pour l'affichage. Le texte sera affiche sur fond *papier* avec une couleur *encre* et sera justifie selon la valeur de *justification* (a gauche par defaut). La marge a gauche et a droite sont definis par *marge_width* et *marge_height* (valant cinq par defaut)
> sdl2::texte(const std::string& phrase, const font& fnt, window& win, int maxwidth,
>             const color& encre, const color& papier, std::int32_t justification = left_justify,
>             int marge_width = 5, int marge_height = 0 )
Même principe que pour le constructeur precedent, sauf qu'on n'impose par une hauteur maximale (qui sera du coup le nombre de pixels en hauteur contenus dans la fenêtre).
....

Par exemple :
....
> sdl2::texte doc("Ceci est de la documentation tres poussee. On peut faire des sauts de ligne, etc.\n"
>                 "Comme on peut le voir ici !", fonte_texte, fenêtre, 320, 200, {0xFF,0xFF,0x00,0xFF},
>                 {0x00,0x00,0xFF,0xFF});
> doc.at(400, 360);
> // Remarque , on connait la dimension de l'image = (maxwidth x maxheight) = (320 x 200)
> doc.rotate({160,100}, 5);
> fenêtre << doc;
....

Remarquons que les retours a la ligne sont pris en compte dans le texte passe en argument.

#### Texte formate

Si l'affichage de texte propose ci-dessus est suffisant dans bien des cas, il montrera tres vite ses limites si on veut afficher un texte long qui ne peut être contenu sur la fenêtre entiere, possedant divers formats au sein du texte (titre, chapître, italique, gras, etc.) et dont on veut afficher qu'une partie a l'ecran.

C'est ici qu'intervient les textes formates. Les textes formates, sont des textes lus dans un fichier dont le format texte est largement inspire du standard **markdown**. Il ne pretend pas être aussi general que lui mais permet deja une belle presentation du texte. Ce que vous voyez a l'ecran actuellement est affiche avec un texte formate defini dans la bibliotheque.

Un fichier texte au format lu par la bibliotheque utilisera les balises suivantes :
....
Un simple diese suivi d'un espace  et d'un texte designe un titre (heading1)
> # Je suis un titre
# Je suis un titre
Un double dieses suivi d'un espace  et d'un texte designe un chapître (heading2)
> ## Je suis un chapître
## Je suis un chapître
Un triple diese suivi d'un espace et d'un texte designe une section (heading3)
> ### Je suis une section
### Je suis une section
Un quadruple diese suivi d'un espace designe une sous-section (heading4)
> #### Je suis une sous-section
#### Je suis une sous-section
Avec cinq dieses suivi d'un espace, une paragraphe (ou autre si on veut)
> ##### Je suis un paragraphe
##### Je suis un paragraphe
Avec six dieses suivi d'un espace, un sous-paragraphe
> ###### Je suis un sous-paragraphe
###### Je suis un sous-paragraphe
Avec un symbole superieur suivi d'un espace, une ligne de code :
> > #include <iostream> // Ceci est du code
> #include <iostream> // Ceci est du code
Une partie du texte commençant par un simple asterisque (colle au premier mot) et se terminant par un simple asterisque (colle au dernier mot) designe une section du texte en *italique*
> *Ceci est en italique*
*Ceci est en italique*
De même, si on met deux asterisques, on aura une section du code en **gras** :
> **Partie du texte en gras**
**Partie du texte en gras**
Et enfin avec trois asterisques, on sera en ***gras et italique*** :
> ***Partie du texte en gras et en italique***
***Partie du texte en gras et en italique***
Une ligne avec quatre asterisques seules designe une ligne epaisse de couleur rouge)
> En dessous, une ligne rouge epaisse
> ****
****
Une ligne avec quatre symboles = designe une ligne un peu moins epaisse et jaune
> Un ligne jaune un peu moins epaisse :
> ====
====
Une ligne avec quatre symboles - designe une ligne encore moins epaisse et cyan
> Une ligne assez fine et cyan :
> ----
----
Une ligne avec quatre symboles _ designe une ligne fine et blanche :
> Une ligne fine et blanche :
> ____
____
Enfin, une ligne avec quatre points designe une ligne fine et grise :
> Une ligne fine et grise :
> ....
....
....

On peut definir plusieurs textes formates (avec des styles differents si on veut). Comme pour un texte, on peut donner soit la fenêtre, soit une sous-zone d'une fenêtre pour afficher le texte formate. Il faut egalement preciser le repertoire (absolu ou relatif) où se trouvent les fontes a charger :

....
> sdl2::formated_text( const window& win, const std::string& path_for_font)
Defini un texte formate qui s'affichera sur toute la fenêtre et qui ira charger ses fontes dans le path donne par *path_for_font*.
> sdl2::formated_text( const std::array<std::int32_t, 4>& z, const std::string& path_for_font)
Defini un texte formate qui s'affichera dans une sous-zone d'une fenêtre (a preciser dans une autre fonction), dont les coordonnees superieurs gauches sont (z[0],z[1]) et dont la dimension est (z[2],z[3]).
....

Par exemple :
...
> sdl2::formated_text fmt_text( fenêtre, "./data");
...

Par defaut, un formateur de texte formatera le texte avec des styles et des fontes par defaut (que vous voyez actuellement a l'ecran), mais il est possible de modifier ces styles. Tout d'abord, chaque formateur de texte contient son propre manager de fontes qui permet de charger dans un dictionnaire les differentes fontes utilisees par les styles :
....
> fmt_text.font_manager.load_and_add(const std::string &key, 
>                                    const std::string &fonte, std::uint8_t size)
chargera en tant que *key* la fonte issue du fichier *fonte* avec la taille *size*.
....

Par exemple :
....
> fmt_text.font_manager.load_and_add("italic", "./data/Lato-Italic.ttf", 16);
va charger la fonte *Lato-Italic.ttf* en tant que clef *italic* avec une taille de 16.
....

Il est bien sûr tout a fait possible d'ecraser d'anciennes fontes avec de nouvelles en donnant la même clef.

Enfin, on peut modifier certaines parties de certains style a loisir. Pour acceder au format d'un style, on utiliser la methode *get_format* :
....
> auto& fmt = fmt_text.get_format(formated_text::style::italic);
> // Definition d'une fonte pour le style italic
> fmt.pt_fonte = fmt.font_manager["italic"];
> // Definition de la couleur du fond pour le style italique (orange):
> fmt.papier   = sdl2::color{0xFF,0xFF,0x00,0xFF};
> // Definition de l'encre pour le style italique (gris clair):
> fmt.encre    = sdl2::color{0xA0,0xA0,0xA0,0xFF};
....

Les differents styles sont : texte, italic, bold, italic_bold, heading_1, heading_2, heading_3, heading_4, heading_5, heading_6 et code

Une fois le style defini (mais ce n'est pas obligatoire, on peut garder le style par defaut), il faut alors charger le fichier contenant le texte formate (comme explique ci-dessus) puis preciser  a quelle ligne on veut commencer :
....
> fmt_text.load_simplified_markdown("SDL2_documentation.md");
> fmt_text.begin_at_line(fenêtre, 0);
....

Il ne reste plus qu'a afficher le resultat dans la fenêtre a l'aide des flots d'entree :
....
> // Remarque : le flush n'est necessaire que si on n'a rien d'autre a afficher
> //            Il doit être la derniere commande apres qu'on est tout "afficher"
> //            afin de mettre a jour la fenêtre a l'ecran
> fenêtre << fmt_text << sdl2::flush;
....

Et si on veut changer de ligne, il suffit de rappeler la methode *begin_at_line* :
....
> // Pour afficher les dix dernieres lignes
> fmt_text.begin_at_line(fenêtre, fmt_text.nb_lines()-10);
....

puis de reafficher le texte dans la fenêtre (apres avoir fait un cls...)
....
> fenêtre.cls({0x00,0x00,0x00,0xFF});
> fenêtre << fmt_text << sdl2::flush;
....

### Les evenements

un evenement est la souris qui se deplace, une touche du clavier enfoncee, un bouton de la souris pressee, etc.

Les evenements sous SDL sont geres a l'aide d'une queue où sont empilees tous les evenements ayant eu lieu depuis la derniere fois où des tests ont ete executes sur les evenements.

Pour gerer les evenements, il faut creer une queue d'evenement :
....
> sdl2::event_queue queue;
....

Puis creer une boucle "infinie" où on va gerer les evenements en les analysant un par un :
....
> bool quitter = false;
> while (not quitter)
> {
>     auto events = queue.pull();
>     for (const auto& e : events )
>     {
>         ...
>     }
> }
....

Dans la boucle, on s'occupera bien sûr des evenements mais aussi de l'affichage si besoin, du son si il y en a, etc.

Pour les evenements, on a donc une boucle parcourant tous les evenements qui ont eu lieu depuis le dernier appel a la fonction *pull* et en verifiant quel est le type d'evenement :
....
> ...
> for ( const auto&  e : events )
> {
>      // Si l'utilisateur a clique sur la croix pour fermer la fenêtre
>      if (e->kind_of_event() == sdl2::event::quit) quitter = true;
>      if (e->kind_of_event() == sdl2::event::key_down)
>      {
>           ...
>      }
> }
....

a chaque type d'evenement correspond une classe qui herite d'un type generique *event*

Les differents types d'evenements et la classe correspondante sont :

**sdl2::event::quit** : A lieu quand l'utilisateur clique sur la croix pour fermer la fenêtre. La classe qui lui correspond est *sdl2::event_quit*, qui ne possede aucune methode specifique...

**sdl2::event::mouse_motion** : a lieu quand l'utilisateur bouge la souris (dans la fenêtre). La classe qui lui correspond est *sdl2::event_mouse_motion*. On peut interroger un objet de cette classe pour avoir le mouvement relatif de la souris depuis la derniere position lue ou bien la position de la souris dans la fenêtre. Par exemple :
....
> if (e->kind_of_event() == sdl2::event::mouse_motion)
> {
>      auto& mouse_mouv = dynamic_cast<sdl2::event_mouse_motion&>(*e);
>      // Retourne le deplacement de la souris depuis la derniere lecture de sa position
>      auto [dx,dy] = mouse_mouv.get_relative_movement();
>      // Retourne la position de la souris dans la fenêtre
>      auto [px,py] = mouse_mouv.position();
> }
....

**sdl2::event::mouse_button** : a lieu quand l'utilisateur appuie sur/releve son doigt d'un bouton de la souris. La classe correspondante est *sdl2::mouse_button*. Elle permet de savoir quel bouton a ete appuye ou releve, son etat (appuye ou releve), la position de la souris lorsque l'action a ete faite sur le bouton. Par exemple :
....
> if (e->kind_of_event() == sdl2::event::mouse_button)
> {
>      auto& bouton = dynamic_cast<sdl2::event_mouse_button&>(*e);
>      // Si on a clique sur le bouton gauche (left)
>      if (bouton.button() == sdl2::event_mouse_button::left)
>      {
>          // A quelle position on a actionne le bouton gauche de la souris
>          auto [px,py] = bouton.position();
>          if (bouton.state() == sdl2::event_mouse_button::pressed) 
>          {
>             // Nombre de fois où le bouton a ete appuye rapidement (double, triple, ... cliques )    
>             auto nombre_de_clics = bouton.number_of_clicks();
>              ...    
>          }
>          // Si on a retire le doigt du bouton :
>          if (bouton.state() == sdl2::event_mouse_button::released)
>          ...
>      }
> }
....
Les autres boutons possibles sont : *right*, *middle*, *x1* et *x2* (pour les souris de joueurs). Attention a la porte de ces valeurs (elles sont definies dans *sdl2::event_mouse_button*).

**sdl2::event::mouse_wheel** : a lieu quand l'utilisateur action la roue se trouvant au centre de la souris. La classe n'a pas encore ete programme. Sûrement dans une prochaine livraison.

**sdl2::event::key** : Cet evenement a lieu quand une touche du clavier change d'etat (devient appuyee ou non appuyee). La classe correspondante est *sdl2::event_keyboard*. On peut interroger cette classe pour savoir le **code** de la touche du clavier concerne (si elle n'a pas d'equivalent ascii en lettre, chiffre ou symbole) ou bien avoir le code ascii si celui-ci existe ! Comme pour la souris, on peut egalement savoir combien de fois une touche a ete frappee rapidement et si la touche a ete pressee ( *pressed* ) ou relachee ( *released* ).
Par exemple :
....
> if (e->kind_of_event() == sdl2::event::key)
> {
>      auto& key = dynamic_cast<sdl2::event_keyboard&>(*e);
>      if (key.state() == sdl2::event_keyboard::pressed)
>      {
>          if ( (key.ascii_code() == 'q') quitter = true;
>          // Si l'utilisateur a actionne la fleche du bas :
>          if (key.code() == sdl2::event_keyboard::down)
>          {
>               auto nombre_fois_frappe = key.repeat();
>               ...
>          }
>      }    
> }
....
Les autres touches dont il n'existe pas de symbole ascii et definis dans la bibliotheque sont (attention a la porte avec *sdl2::event_keyboard*) : F1, F2, F3, F4, F5, F6, F7, ..., F12, printscreen, scrolllock, insert, home, pageup, pagedown, end, right, left, down et up.

**sdl2::event::touch_finger** : Quand l'utilisateur bouge ou appuie son doigt sur l'ecran tactile (si il y en a un). L'evenement creee est de type *sdl2::event_touch*. On peut interroger une instance de cette classe pour connaître le deplacement normalise du doigt (entre -1. et 1.) ou bien la position normalise du doigt par rapport a la fenêtre (entre 0 et 1.). Remarquez que toutes ces valeurs sont des reels simple precision. On peut egalement connaître la pression exercee par le doigt par une valeur normalisee entre 0 et 1 (la encore une reel simple precision).
Il est possible de connaître le type d'evenement associer au peripherique tactile grace a la methode *type_of_event()* : cela peut être de type sdl2::event::finger_motion, sdl2::event::finger_down, sdl2::event::finger_up.
Exemple :
....
> if (e->kind_of_event() == sdl2::event::touch_finger)
> {
>     auto& touch_ev = dynamic_cast<sdl2::event_touch&>(*e);
>     if (touch_ev.pression() > 0.f)
>     {
>          auto [dx,dy] = touch_ev.displacement();
>          auto [px,py] = touch_ev.where();
>          ....
>     }
....

## Pour finir

N'hesitez pas a regarder les deux exemples fournis (le lecteur de document et le test) ainsi que les sources pour vous approprier le code !