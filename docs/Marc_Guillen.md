# Marc Guillén
## Individual Contribution

 - **EncounterNode class**: In blue alert the player progress by defeating a map of encounters which contains the type of building the encounter is, the difficulty of the encounter, the deck the enemy AI will have and the rewards the player will obtain by defeating the encounter.  This values can be edited by the designer by changing the values of an xml.

![xml_encounter](Readme_Files/nodes.png)

- **EncounterTree class**: it's a factory of EncounterNodes that creates, destroy and it's able to draw a path between them in three different colors blue meaning the available ways to continue progressing, red meaning unaccessible paths and green meaning conquered paths.

![Blue Alert Strategy Map](Readme_Files/strategy.jpg)

- **Transition Manager**: during this project I did a research about transitions and developed a transition manager that includes multiple ways of transitioning such as fades, dissolves, wipes and camera translations. You can check the research repository [here](https://marcgs96.github.io/Camera-Transitions-Research/). 

- **Troop Deploy UI**: With the help of Alex Morales and Laia Martinez we designed and developed the troop deploying UI in the combat scene, this UI is able to show the player cards, the current energy available the cost of each cards and an internal cooldown for each troop depending of their cost and the current energy.

![Blue Alert combat ui](Readme_Files/combat_bar.gif)