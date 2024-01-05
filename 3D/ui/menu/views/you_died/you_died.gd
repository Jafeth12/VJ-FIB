extends Control

func _ready():
	MainLogic.finish_timer_died()
	#MusicController.music_stop()
	#$amogus.play()
	$AnimationPlayer.play("you_died_anim")
	await $AnimationPlayer.animation_finished

func _on_retry_button_up():
	MainLogic.reset_all()
	#$amogus.stop()
	SceneTransitions.change_scene("res://main.tscn")

func _on_exit_button_up():
	$amogus.stop()
	#MusicController.play_menu_music()
	SceneTransitions.change_scene("res://ui/menu/menu.tscn")
