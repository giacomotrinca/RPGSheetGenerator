import library as lib

human = lib.Race("Human",
                 {"strength": 1, 
                  "dexterity": 1, 
                  "constitution": 1, 
                  "intelligence": 1, 
                  "wisdom": 1, 
                  "charisma": 1})
fighter = lib.CharacterClass("Fighter", 10)
character = lib.Character("Arya", race=human, character_class=fighter, level=1)
print(character)
print(character.print_attributes())