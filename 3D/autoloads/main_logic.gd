extends Node

enum LEVEL { LEVEL1 = 1, LEVEL2 = 2, END = 3 }

@onready var current_level: LEVEL = LEVEL.LEVEL1

func get_current_level() -> LEVEL:
	if current_level == LEVEL.END:
		current_level = LEVEL.LEVEL1
	return current_level

func go_to_next_level() -> LEVEL:
	match current_level:
		LEVEL.LEVEL1:
			current_level = LEVEL.LEVEL2
		LEVEL.LEVEL2:
			current_level = LEVEL.END
	return current_level

func reset_to_start() -> void:
	current_level = LEVEL.LEVEL1
