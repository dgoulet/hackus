#!/bin/bash

quote[0]="Il y a 10 types de personnes dans le monde : celles qui comprennent le binaire, et celles qui ne le comprennent pas."
quote[1]="When Life Gives You Questions, Google has Answers"
quote[2]="If at first you don't succeed; call it version 1.0"
quote[3]="I'm not anti-social; I'm just not user friendly "
quote[4]="I would love to change the world, but they won't give me the source code"
quote[5]="A computer lets you make more mistakes faster than any invention in human history - with the possible exceptions of handguns and tequila."
quote[6]="My software never has bugs. It just develops random features."
quote[7]="Windows has detected you do not have a keyboard. Press 'F9' to continue."
quote[8]="Roses are #FF0000. Violets are #0000FF. All my base. Are belong to you."
quote[9]="Enter any 11-digit prime number to continue..."
quote[10]="Better to be a geek than an idiot."
quote[11]="Les mots de passe sont comme les sous-vetements. On ne devrait pas les laisser trainer la ou des personnes pourraient les voir. On devrait en changer regulierement. On ne devrait pas les preter a des inconnus."
quote[12]="Les filles sont comme les noms de domaines, celles que jaime sont deja prises."
quote[13]="Le manuel disait Necessite Windows XP ou mieux. Jai donc installe Linux."
quote[14]="Les gens disent que si on joue les CD de Microsoft a lenvers, on entend des sons sataniques. Mais ce nest rien, parce que si on le joue a lendroit, cela installe Windows"
quote[15]="La vitesse du son est definie par la distance entre la porte et lordinateur, divisée par lintervalle de temps necessaire pour fermer le media player, et remonter ton pantalon quand ta mere crie OH MON DIEU MAIS QUEST-CE QUE TU FAIS"
quote[16]="Un ordinateur permet de faire des erreurs plus rapidement que nimporte quelle autre invention humaine, a lexception pres des pistolets et de la tequila."
quote[17]="Mes logiciels nont jamais de bug. Ils developpent juste certaines fonctions aleatoires."
quote[18]="Si vous ne reussissez pas du premier coup, appelez ça version 1.0."

rnumber=$((RANDOM%18))
echo ${quote[$rnumber]}
