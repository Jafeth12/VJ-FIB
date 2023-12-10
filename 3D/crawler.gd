class_name Crawler extends GenericEnemy

var attack_ended: bool = false

func _ready():
	$sprite.connect("animation_finished", on_animation_finished)

func get_new_alpha(current_alpha: float, delta: float) -> float:
	var dir = 1
	match enemy_state:
		EnemyState.WANDER:
			return current_alpha + dir * SPEED * delta
	return alpha

func update_animation() -> void:
	match enemy_state:
		EnemyState.WAIT:
			if $sprite.animation != "idle":
				$sprite.play("idle")
		EnemyState.WANDER:
			if $sprite.animation != "walk":
				$sprite.play("walk")
		EnemyState.ATTACK:
			if $sprite.animation != "attack":
				$sprite.play("attack")
		EnemyState.DEAD:
			if $sprite.animation != "die":
				$sprite.play("die")

func should_attack() -> bool:
	return Input.is_action_just_pressed("god_mode")

func is_attack_finished() -> bool:
	var ae_copy: bool = attack_ended
	attack_ended = false
	return ae_copy

func on_animation_finished() -> void:
	match $sprite.animation:
		"attack":
			attack_ended = true
