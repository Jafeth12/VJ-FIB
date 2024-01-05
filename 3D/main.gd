extends Node

@onready var hud = $CanvasLayer/HUD
var timer = Timer.new()

var level1 = preload("res://levels/level1.tscn")
var level2 = preload("res://levels/level2.tscn")

func _ready():
	$Player.connect("level_ended", _on_level_ended)
	match MainLogic.get_current_level():
		MainLogic.LEVEL.LEVEL1:
			add_child(level1.instantiate())
		MainLogic.LEVEL.LEVEL2:
			add_child(level2.instantiate())
		MainLogic.LEVEL.END:
			pass
	MusicController.play_level_music()
	MainLogic.resume_timer()

func _on_level_ended() -> void:
	MainLogic.save_player_state($Player.player_get_state())
	MainLogic.pause_timer()
	if MainLogic.go_to_next_level() == MainLogic.LEVEL.END:
		SceneTransitions.change_scene("res://ui/menu/views/you_won/you_won.tscn")
	else:
		SceneTransitions.change_scene("res://main.tscn")

func _on_player_died():
	MainLogic.finish_timer_died()
	MainLogic.reset_player_state()
	var timer = Timer.new()
	timer.one_shot = true
	add_child(timer)
	timer.start(2)
	await timer.timeout
	timer.queue_free()
	SceneTransitions.change_scene("res://ui/menu/views/you_died/you_died.tscn")
