import random

class Character:
    def __init__(self, name, race, character_class, level=1):
        self.name = name
        self.race = race
        self.character_class = character_class
        self.level = level
        self.attributes = self.roll_attributes()
        self.hit_points = self.calculate_hit_points()

    def roll_attributes(self):
        attributes = {}
        for attr in ['strength', 'dexterity', 'constitution', 'intelligence', 'wisdom', 'charisma']:
            attributes[attr] = self.roll_4d6_drop_lowest()
        return attributes

    def roll_4d6_drop_lowest(self):
        rolls = [random.randint(1, 6) for _ in range(4)]
        return sum(sorted(rolls)[1:])

    def calculate_hit_points(self):
        # Placeholder for hit points calculation logic
        return 10 + self.level

    def __str__(self):
        return f"{self.name}, the {self.race.name} {self.character_class.name} (Level {self.level})"

    def print_attributes(self):
        attributes_str = "\n".join([f"{key.capitalize()}: {value}" for key, value in self.attributes.items()])
        return f"Attributes:\n{attributes_str}"

class Race:
    def __init__(self, name, bonuses):
        self.name = name
        self.bonuses = bonuses

class CharacterClass:
    def __init__(self, name, hit_die):
        self.name = name
        self.hit_die = hit_die