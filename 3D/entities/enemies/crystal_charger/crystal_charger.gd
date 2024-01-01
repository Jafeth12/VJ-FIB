class_name CrystalCharger extends GenericEnemy

var cc_attack_finished: bool = false

func _ready():
	$sprite.play("idle")
	cc_attack_finished = false
	ENEMY_INIT_SHIELD = 200
	ENEMY_INIT_HEALTH = 50
	enemy_init_bars($SubViewport/ShieldBar3D, $SubViewport/HealthBar3D)
	super()

# ===== REIMPLEMENTACIONES DE ENTITY =====

func entity_should_jump() -> bool:
	return false # Este personaje no salta

func entity_jump() -> float:
	return 0 # Nunca saltará

func entity_get_new_alpha(current_alpha: float, direction: EntityDirection, delta: float) -> float:
	return current_alpha # Este personaje no se mueve

func entity_get_new_direction(current_direction: EntityDirection) -> EntityDirection:
	# TODO: Apuntar para donde esté el jugador
	return EntityDirection.LEFT


# ===== REIMPLEMENTACIONES DE ENEMY =====

func enemy_update_animation() -> void:
	match enemy_state:
		EnemyState.WAIT:
			if $sprite.animation != "idle":
				$sprite.play("idle")
		EnemyState.WANDER:
			if $sprite.animation != "idle":
				$sprite.play("idle")
		EnemyState.ATTACK:
			if $sprite.animation != "idle":
				$sprite.play("idle")
		EnemyState.DEAD:
			if $sprite.animation != "die":
				$sprite.play("die")

func enemy_should_activate() -> bool:
	return true

func enemy_should_attack() -> bool:
	# TODO: Atacar cuando el jugador esté cerca
	return false

func enemy_is_attack_finished() -> bool:
	var ae_copy: bool = cc_attack_finished
	cc_attack_finished = false
	return ae_copy
