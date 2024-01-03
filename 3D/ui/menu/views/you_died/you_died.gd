extends Control

func _ready():
	$AnimationPlayer.play("you_died_anim")
	await $AnimationPlayer.animation_finished

func _on_retry_button_up():
	print("Hola hola")
	SceneTransitions.change_scene("res://main.tscn")

func _on_exit_button_up():
	SceneTransitions.change_scene("res://ui/menu/menu.tscn")
