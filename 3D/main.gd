extends Node

@onready var hud = $CanvasLayer/HUD
var timer = Timer.new()

var level1 = preload("res://levels/level1.tscn")
var level2 = preload("res://levels/level2.tscn")


func _ready():
	var level = null
	match MainLogic.get_current_level():
		MainLogic.LEVEL.LEVEL1:
			level = level1.instantiate()
		MainLogic.LEVEL.LEVEL2:
			level = level2.instantiate()
	add_child(level)
	MusicController.play_level_music()
	pass

func _process(_delta):
	pass

# -----------------------------

func _on_player_died():
	var timer = Timer.new()
	timer.one_shot = true
	add_child(timer)
	timer.start(2)
	await timer.timeout
	timer.queue_free()
	SceneTransitions.change_scene("res://ui/menu/views/you_died/you_died.tscn")
