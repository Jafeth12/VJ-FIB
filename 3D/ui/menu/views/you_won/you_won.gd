extends Control

func _ready():
	var time = MainLogic.finish_timer_won()
	$VBoxContainer/time.text = MainLogic.format_time(time)
	$AnimationPlayer.play("you_won_anim")
	await $AnimationPlayer.animation_finished
	Input.set_mouse_mode(Input.MOUSE_MODE_VISIBLE)

func _on_return_button_up():
	SceneTransitions.change_scene("res://ui/menu/menu.tscn")
