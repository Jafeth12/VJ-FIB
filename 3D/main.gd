extends Node

@onready var hud = $CanvasLayer/HUD
var timer = Timer.new()

func _ready():
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
