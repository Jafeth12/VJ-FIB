extends Node

enum LEVEL { LEVEL1 = 1, LEVEL2 = 2 }

@onready var current_level: LEVEL = LEVEL.LEVEL2

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass

func get_current_level() -> LEVEL:
	return current_level
