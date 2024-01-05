extends Control

func _ready():
	var time = MainLogic.finish_timer_won()
	$VBoxContainer/time.text = str(snapped(time, 0.01)) + " seconds"
	$AnimationPlayer.play("you_won_anim")
	await $AnimationPlayer.animation_finished

func _on_return_button_up():
	SceneTransitions.change_scene("res://ui/menu/menu.tscn")
