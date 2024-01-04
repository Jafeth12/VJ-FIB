extends Control

func _ready():
	$AnimationPlayer.play("you_won_anim")
	await $AnimationPlayer.animation_finished

func _on_return_button_up():
	SceneTransitions.change_scene("res://ui/menu/menu.tscn")
