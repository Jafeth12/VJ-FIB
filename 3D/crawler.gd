class_name Crawler extends GenericEnemy

var attack_ended: bool = false

func _ready():
	$sprite.connect("animation_finished", on_animation_finished)

# OVERRIDE de ENTITY
func entity_get_new_alpha(current_alpha: float, delta: float) -> float:
	match enemy_state:
		EnemyState.WANDER:
			return current_alpha + enemy_dir * SPEED * delta
	return alpha

# OVERRIDE de ENEMY
func enemy_update_animation() -> void:
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

# OVERRIDE de ENEMY
func enemy_should_attack() -> bool:
	return Input.is_action_just_pressed("dbg_enemies_attack")

# OVERRIDE de ENEMY
func enemy_should_die() -> bool:
	return Input.is_action_just_pressed("dbg_enemies_die")

# OVERRIDE de ENEMY
func enemy_is_attack_finished() -> bool:
	var ae_copy: bool = attack_ended
	attack_ended = false
	return ae_copy

func on_animation_finished() -> void:
	match $sprite.animation:
		"attack":
			attack_ended = true
