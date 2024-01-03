extends Control

func _ready():
	$AnimationPlayer.play("you_died_anim")
	await $AnimationPlayer.animation_finished
