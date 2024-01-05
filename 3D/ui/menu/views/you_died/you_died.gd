extends Control

func _ready():
	MainLogic.finish_timer_died()
	MusicController.play_amogus_music()
	$AnimationPlayer.play("you_died_anim")
	await $AnimationPlayer.animation_finished
	Input.set_mouse_mode(Input.MOUSE_MODE_VISIBLE)

func _on_retry_button_up():
	MainLogic.reset_all()
	SceneTransitions.change_scene("res://main.tscn")

func _on_exit_button_up():
	SceneTransitions.change_scene("res://ui/menu/menu.tscn")
