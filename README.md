# RPGSheetGenerator

RPGSheetGenerator is a character sheet generator for Dungeons & Dragons (D&D). This project allows you to create characters with randomly generated attributes and apply racial bonuses based on the chosen race.

## Features

- Random generation of character attributes using the 4d6 method, dropping the lowest die.
- Automatic application of racial bonuses to character attributes.
- Support for various races and character classes.

## Installation

1. Clone the repository:
    ```bash
    git clone https://github.com/giacomotrinca/RPGSheetGenerator.git
    ```
2. Navigate to the project directory:
    ```bash
    cd RPGSheetGenerator
    ```

## Usage

Example usage of the character generator:

```python
import library as lib

# Define races
human = lib.Race("Human", {'strength': 1, 'dexterity': 1, 'constitution': 1, 'intelligence': 1, 'wisdom': 1, 'charisma': 1})
elf = lib.Race("Elf", {'dexterity': 2, 'intelligence': 1})
half_elf = lib.Race("Half-Elf", {'charisma': 2, 'strength': 1, 'dexterity': 1})
dwarf = lib.Race("Dwarf", {'constitution': 2, 'strength': 1})
tiefling = lib.Race("Tiefling", {'intelligence': 1, 'charisma': 2})

# Define character classes
fighter = lib.CharacterClass("Fighter", 10)

# Create an example character
character = lib.Character("Arya", race=human, character_class=fighter, level=1)
print(character)
print(character.print_attributes())